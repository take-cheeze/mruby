/*
** gc.c - garbage collector for mruby
**
** See Copyright Notice in mruby.h
*/

#include <string.h>
#include <stdlib.h>
#include <mruby.h>
#include <mruby/array.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/hash.h>
#include <mruby/proc.h>
#include <mruby/range.h>
#include <mruby/string.h>
#include <mruby/variable.h>
#include <mruby/gc.h>
#include <mruby/error.h>
#include <mruby/throw.h>
#include "value_array.h"

struct free_obj {
  MRB_OBJECT_HEADER;
  struct RVALUE *next;
};

typedef struct RVALUE {
  union {
    struct free_obj free;
    struct RBasic basic;
    struct RObject object;
    struct RClass klass;
    struct RString string;
    struct RArray array;
    struct RHash hash;
    struct RRange range;
    struct RData data;
    struct RProc proc;
    struct REnv env;
    struct RException exc;
#ifdef MRB_WORD_BOXING
    struct RFloat floatv;
    struct RCptr cptr;
#endif
  } as;
} RVALUE;

#ifdef GC_PROFILE
#include <stdio.h>
#include <sys/time.h>

static double program_invoke_time = 0;
static double gc_time = 0;
static double gc_total_time = 0;

static double
gettimeofday_time(void)
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec + tv.tv_usec * 1e-6;
}

#define GC_INVOKE_TIME_REPORT(with) do {\
  fprintf(stderr, "%s\n", with);\
  fprintf(stderr, "gc_invoke: %19.3f\n", gettimeofday_time() - program_invoke_time);\
  fprintf(stderr, "is_generational: %d\n", is_generational(gc));\
  fprintf(stderr, "is_major_gc: %d\n", is_major_gc(gc));\
} while(0)

#define GC_TIME_START do {\
  gc_time = gettimeofday_time();\
} while(0)

#define GC_TIME_STOP_AND_REPORT do {\
  gc_time = gettimeofday_time() - gc_time;\
  gc_total_time += gc_time;\
  fprintf(stderr, "gc_state: %d\n", gc->state);\
  fprintf(stderr, "live: %zu\n", gc->live);\
  fprintf(stderr, "majorgc_old_threshold: %zu\n", gc->majorgc_old_threshold);\
  fprintf(stderr, "gc_threshold: %zu\n", gc->threshold);\
  fprintf(stderr, "gc_time: %30.20f\n", gc_time);\
  fprintf(stderr, "gc_total_time: %30.20f\n\n", gc_total_time);\
} while(0)
#else
#define GC_INVOKE_TIME_REPORT(s)
#define GC_TIME_START
#define GC_TIME_STOP_AND_REPORT
#endif

#ifdef GC_DEBUG
#define DEBUG(x) (x)
#else
#define DEBUG(x)
#endif

#ifndef MRB_HEAP_PAGE_SIZE
#define MRB_HEAP_PAGE_SIZE 1024
#endif

#define GC_STEP_SIZE 1024

#define objects(p) ((RVALUE *)p->objects)

enum { REF_COUNT_MAX = 7 };

MRB_API void*
mrb_realloc_simple(mrb_state *mrb, void *p,  size_t len)
{
  void *p2;

  p2 = (mrb->allocf)(mrb, p, len, mrb->allocf_ud);
  if (!p2 && len > 0 && mrb->gc.heaps) {
    mrb_full_gc(mrb);
    p2 = (mrb->allocf)(mrb, p, len, mrb->allocf_ud);
  }

  return p2;
}

MRB_API void*
mrb_realloc(mrb_state *mrb, void *p, size_t len)
{
  void *p2;

  p2 = mrb_realloc_simple(mrb, p, len);
  if (len == 0) return p2;
  if (p2 == NULL) {
    if (mrb->gc.out_of_memory) {
      mrb_exc_raise(mrb, mrb_obj_value(mrb->nomem_err));
      /* mrb_panic(mrb); */
    }
    else {
      mrb->gc.out_of_memory = TRUE;
      mrb_exc_raise(mrb, mrb_obj_value(mrb->nomem_err));
    }
  }
  else {
    mrb->gc.out_of_memory = FALSE;
  }

  return p2;
}

MRB_API void*
mrb_malloc(mrb_state *mrb, size_t len)
{
  return mrb_realloc(mrb, 0, len);
}

MRB_API void*
mrb_malloc_simple(mrb_state *mrb, size_t len)
{
  return mrb_realloc_simple(mrb, 0, len);
}

MRB_API void*
mrb_calloc(mrb_state *mrb, size_t nelem, size_t len)
{
  void *p;

  if (nelem > 0 && len > 0 &&
      nelem <= SIZE_MAX / len) {
    size_t size;
    size = nelem * len;
    p = mrb_malloc(mrb, size);

    memset(p, 0, size);
  }
  else {
    p = NULL;
  }

  return p;
}

MRB_API void
mrb_free(mrb_state *mrb, void *p)
{
  (mrb->allocf)(mrb, p, 0, mrb->allocf_ud);
}

MRB_API mrb_bool
mrb_object_dead_p(mrb_state *mrb, struct RBasic *object) {
  return object->tt == MRB_TT_FREE;
}

static void
link_heap_page(mrb_gc *gc, mrb_heap_page *page)
{
  page->next = gc->heaps;
  if (gc->heaps)
    gc->heaps->prev = page;
  gc->heaps = page;
}

static void
unlink_heap_page(mrb_gc *gc, mrb_heap_page *page)
{
  if (page->prev) page->prev->next = page->next;
  if (page->next) page->next->prev = page->prev;
  if (gc->heaps == page) gc->heaps = page->next;
  page->prev = NULL;
  page->next = NULL;
}

static void
add_heap(mrb_state *mrb, mrb_gc *gc)
{
  mrb_heap_page *page = (mrb_heap_page *)mrb_malloc(
      mrb, sizeof(mrb_heap_page) + MRB_HEAP_PAGE_SIZE * sizeof(RVALUE));
  RVALUE *p, *e;

  for (p = objects(page), e=p+MRB_HEAP_PAGE_SIZE; p<e; p++) {
    p->as.free.tt = MRB_TT_FREE;
    p->as.free.next = gc->freelist;
    gc->freelist = p;
  }
  page->mark_bits = NULL;

  link_heap_page(gc, page);
}

#define DEFAULT_GC_INTERVAL_RATIO 200
#define DEFAULT_GC_STEP_RATIO 200
#define DEFAULT_MAJOR_GC_INC_RATIO 200
#define is_generational(gc) ((gc)->generational)
#define is_major_gc(gc) (is_generational(gc) && (gc)->full)
#define is_minor_gc(gc) (is_generational(gc) && !(gc)->full)

void
mrb_gc_init(mrb_state *mrb, mrb_gc *gc)
{
#ifndef MRB_GC_FIXED_ARENA
  gc->arena = (struct RBasic**)mrb_malloc(mrb, sizeof(struct RBasic*)*MRB_GC_ARENA_SIZE);
  gc->arena_capa = MRB_GC_ARENA_SIZE;
#endif

  gc->heaps = NULL;
  gc->freelist = NULL;
  gc->live = 0;
  add_heap(mrb, gc);

#ifdef GC_PROFILE
  program_invoke_time = gettimeofday_time();
#endif
}

static void obj_free(mrb_state *mrb, struct RBasic *obj, int end);

void
free_heap(mrb_state *mrb, mrb_gc *gc)
{
  mrb_heap_page *page = gc->heaps;
  mrb_heap_page *tmp;
  RVALUE *p, *e;

  while (page) {
    tmp = page;
    page = page->next;
    for (p = objects(tmp), e=p+MRB_HEAP_PAGE_SIZE; p<e; p++) {
      if (p->as.free.tt != MRB_TT_FREE)
        obj_free(mrb, &p->as.basic, TRUE);
    }
    mrb_free(mrb, tmp->mark_bits);
    mrb_free(mrb, tmp);
  }
}

void
mrb_gc_destroy(mrb_state *mrb, mrb_gc *gc)
{
  free_heap(mrb, gc);
  mrb_free(mrb, gc->heap_pages_table);
#ifndef MRB_GC_FIXED_ARENA
  mrb_free(mrb, gc->arena);
#endif
}

static void
gc_protect(mrb_state *mrb, mrb_gc *gc, struct RBasic *p)
{
#ifdef MRB_GC_FIXED_ARENA
  if (gc->arena_idx >= MRB_GC_ARENA_SIZE) {
    /* arena overflow error */
    gc->arena_idx = MRB_GC_ARENA_SIZE - 4; /* force room in arena */
    mrb_exc_raise(mrb, mrb_obj_value(mrb->arena_err));
  }
#else
  if (gc->arena_idx >= gc->arena_capa) {
    /* extend arena */
    gc->arena_capa = (int)(gc->arena_capa * 1.5);
    gc->arena = (struct RBasic**)mrb_realloc(mrb, gc->arena, sizeof(struct RBasic*)*gc->arena_capa);
  }
#endif
  gc->arena[gc->arena_idx++] = p;
  mrb_obj_inc_ref(mrb, p);
}

/* mrb_gc_protect() leaves the object in the arena */
MRB_API void
mrb_gc_protect(mrb_state *mrb, mrb_value obj)
{
  if (mrb_immediate_p(obj)) return;
  gc_protect(mrb, &mrb->gc, mrb_basic_ptr(obj));
}

#define GC_ROOT_NAME "_gc_root_"

/* mrb_gc_register() keeps the object from GC.

   Register your object when it's exported to C world,
   without reference from Ruby world, e.g. callback
   arguments.  Don't forget to remove the obejct using
   mrb_gc_unregister, otherwise your object will leak.
*/

MRB_API void
mrb_gc_register(mrb_state *mrb, mrb_value obj)
{
  mrb_sym root = mrb_intern_lit(mrb, GC_ROOT_NAME);
  mrb_value table = mrb_gv_get(mrb, root);

  if (mrb_nil_p(table) || mrb_type(table) != MRB_TT_ARRAY) {
    table = mrb_ary_new(mrb);
    mrb_gv_set(mrb, root, table);
  }
  mrb_ary_push(mrb, table, obj);
}

/* mrb_gc_unregister() removes the object from GC root. */
MRB_API void
mrb_gc_unregister(mrb_state *mrb, mrb_value obj)
{
  mrb_sym root = mrb_intern_lit(mrb, GC_ROOT_NAME);
  mrb_value table = mrb_gv_get(mrb, root);
  struct RArray *a;
  mrb_int i;

  if (mrb_nil_p(table)) return;
  if (mrb_type(table) != MRB_TT_ARRAY) {
    mrb_gv_set(mrb, root, mrb_nil_value());
    return;
  }
  a = mrb_ary_ptr(table);
  mrb_ary_modify(mrb, a);
  for (i = 0; i < a->len; i++) {
    if (mrb_obj_eq(mrb, a->ptr[i], obj)) {
      a->len--;
      values_move(&a->ptr[i], &a->ptr[i + 1], a->len - i);
      break;
    }
  }
}

MRB_API struct RBasic*
mrb_obj_alloc(mrb_state *mrb, enum mrb_vtype ttype, struct RClass *cls)
{
  struct RBasic *p;
  static const RVALUE RVALUE_zero = { { { MRB_TT_FALSE } } };
  mrb_gc *gc = &mrb->gc;

  if (cls) {
    enum mrb_vtype tt;

    switch (cls->tt) {
    case MRB_TT_CLASS:
    case MRB_TT_SCLASS:
    case MRB_TT_MODULE:
    case MRB_TT_ENV:
      break;
    default:
      mrb_raise(mrb, E_TYPE_ERROR, "allocation failure");
    }
    tt = MRB_INSTANCE_TT(cls);
    if (tt != MRB_TT_FALSE &&
        ttype != MRB_TT_SCLASS &&
        ttype != MRB_TT_ICLASS &&
        ttype != MRB_TT_ENV &&
        ttype != tt) {
      mrb_raisef(mrb, E_TYPE_ERROR, "allocation failure of %S", mrb_obj_value(cls));
    }
  }

#ifdef MRB_GC_STRESS
  mrb_full_gc(mrb);
#endif
  if (!gc->freelist) { add_heap(mrb, gc); }

  p = (struct RBasic*)gc->freelist;
  gc->freelist = gc->freelist->as.free.next;

  gc->live++;
  *(RVALUE *)p = RVALUE_zero;
  p->tt = ttype;
  gc_protect(mrb, gc, p);
  mrb_obj_ref_init(mrb, p->c, cls);
  p->ref_count = 0;
  return p;
}

static void
mark_context_stack(mrb_state *mrb, struct mrb_context *c)
{
  size_t i;
  size_t e;
  mrb_value nil;
  int nregs;

  if (c->stack == NULL) return;
  e = c->stack - c->stbase;
  if (c->ci) {
    nregs = c->ci->argc + 2;
    if (c->ci->nregs > nregs)
      nregs = c->ci->nregs;
    e += nregs;
  }
  if (c->stbase + e > c->stend) e = c->stend - c->stbase;
  for (i=0; i<e; i++) {
    mrb_value v = c->stbase[i];

    if (!mrb_immediate_p(v)) {
      mrb_gc_mark(mrb, mrb_basic_ptr(v));
    }
  }
  e = c->stend - c->stbase;
  nil = mrb_nil_value();
  for (; i<e; i++) {
    c->stbase[i] = nil;
  }
}

static void
mark_context(mrb_state *mrb, struct mrb_context *c)
{
  int i;
  mrb_callinfo *ci;

  /* mark stack */
  mark_context_stack(mrb, c);

  /* mark VM stack */
  if (c->cibase) {
    for (ci = c->cibase; ci <= c->ci; ci++) {
      mrb_gc_mark(mrb, (struct RBasic*)ci->env);
      mrb_gc_mark(mrb, (struct RBasic*)ci->proc);
      mrb_gc_mark(mrb, (struct RBasic*)ci->target_class);
    }
  }
  /* mark ensure stack */
  for (i=0; i<c->esize; i++) {
    if (c->ensure[i] == NULL) break;
    mrb_gc_mark(mrb, (struct RBasic*)c->ensure[i]);
  }
  /* mark fibers */
  mrb_gc_mark(mrb, (struct RBasic*)c->fib);
  if (c->prev) {
    mark_context(mrb, c->prev);
  }
}

static void
gc_mark_children(mrb_state *mrb, mrb_gc *gc, struct RBasic *obj)
{
  mrb_gc_mark(mrb, (struct RBasic*)obj->c);
  switch (obj->tt) {
  case MRB_TT_ICLASS:
    {
      struct RClass *c = (struct RClass*)obj;
      if (MRB_FLAG_TEST(c, MRB_FLAG_IS_ORIGIN))
        mrb_gc_mark_mt(mrb, c);
      mrb_gc_mark(mrb, (struct RBasic*)((struct RClass*)obj)->super);
    }
    break;

  case MRB_TT_CLASS:
  case MRB_TT_MODULE:
  case MRB_TT_SCLASS:
    {
      struct RClass *c = (struct RClass*)obj;

      mrb_gc_mark_mt(mrb, c);
      mrb_gc_mark(mrb, (struct RBasic*)c->super);
    }
    /* fall through */

  case MRB_TT_OBJECT:
  case MRB_TT_DATA:
  case MRB_TT_EXCEPTION:
    mrb_gc_mark_iv(mrb, (struct RObject*)obj);
    break;

  case MRB_TT_PROC:
    {
      struct RProc *p = (struct RProc*)obj;

      mrb_gc_mark(mrb, (struct RBasic*)p->env);
      mrb_gc_mark(mrb, (struct RBasic*)p->target_class);
    }
    break;

  case MRB_TT_ENV:
    {
      struct REnv *e = (struct REnv*)obj;
      mrb_int i, len;

      if (MRB_ENV_STACK_SHARED_P(e)) {
        if (e->cxt.c->fib) {
          mrb_gc_mark(mrb, (struct RBasic*)e->cxt.c->fib);
        }
        break;
      }
      len = MRB_ENV_STACK_LEN(e);
      for (i=0; i<len; i++) {
        mrb_gc_mark_value(mrb, e->stack[i]);
      }
    }
    break;

  case MRB_TT_FIBER:
    {
      struct mrb_context *c = ((struct RFiber*)obj)->cxt;

      if (c) mark_context(mrb, c);
    }
    break;

  case MRB_TT_ARRAY:
    {
      struct RArray *a = (struct RArray*)obj;
      size_t i, e;

      for (i=0,e=a->len; i<e; i++) {
        mrb_gc_mark_value(mrb, a->ptr[i]);
      }
    }
    break;

  case MRB_TT_HASH:
    mrb_gc_mark_iv(mrb, (struct RObject*)obj);
    mrb_gc_mark_hash(mrb, (struct RHash*)obj);
    break;

  case MRB_TT_STRING:
    break;

  case MRB_TT_RANGE:
    {
      struct RRange *r = (struct RRange*)obj;

      if (r->edges) {
        mrb_gc_mark_value(mrb, r->edges->beg);
        mrb_gc_mark_value(mrb, r->edges->end);
      }
    }
    break;

  default:
    break;
  }
}

MRB_API void
mrb_gc_mark(mrb_state *mrb, struct RBasic *obj)
{
  mrb_heap_page *page = NULL;
  RVALUE const *rv = (RVALUE const*)obj;

  if (!obj) return;

  // should be binary search
  for (int i = 0; i < mrb->gc.heap_pages_count; ++i ) {
    RVALUE *region = objects(mrb->gc.heap_pages_table[i]);
    if (region <= rv && rv < region + MRB_HEAP_PAGE_SIZE) {
      page = mrb->gc.heap_pages_table[i];
      break;
    }
  }

  mrb_assert(page);

  if (!page->mark_bits) {
    page->mark_bits = (uint8_t*)mrb_malloc(mrb, MRB_HEAP_PAGE_SIZE / 8);
  }

  unsigned const page_offset = rv - objects(page);
  if (page->mark_bits[page_offset / 8] & (1 << (page_offset % 8))) return;
  mrb_assert((obj)->tt != MRB_TT_FREE);
  page->mark_bits[page_offset / 8] |= (1 << (page_offset % 8));
  gc_mark_children(mrb, &mrb->gc, obj);
}

static void
obj_free(mrb_state *mrb, struct RBasic *obj, int end)
{
  DEBUG(fprintf(stderr, "obj_free(%p,tt=%d)\n",obj,obj->tt));

  if (!end) {
    mrb_assert(obj->ref_count == 0 || obj->ref_count == REF_COUNT_MAX);
  }

  mrb_assert(obj->tt != MRB_TT_FREE);

  switch (obj->tt) {
    /* immediate - no mark */
  case MRB_TT_TRUE:
  case MRB_TT_FIXNUM:
  case MRB_TT_SYMBOL:
    /* cannot happen */
    return;

  case MRB_TT_FLOAT:
#ifdef MRB_WORD_BOXING
    break;
#else
    return;
#endif

  case MRB_TT_OBJECT:
    mrb_gc_free_iv(mrb, (struct RObject*)obj, end);
    break;

  case MRB_TT_EXCEPTION:
    mrb_gc_free_iv(mrb, (struct RObject*)obj, end);
    break;

  case MRB_TT_CLASS:
  case MRB_TT_MODULE:
  case MRB_TT_SCLASS: {
    struct RClass *cls = (struct RClass*)obj;
    if (!end) mrb_obj_ref_clear(mrb, cls->super);
    mrb_gc_free_mt(mrb, cls);
    mrb_gc_free_iv(mrb, (struct RObject*)obj, end);
  } break;
  case MRB_TT_ICLASS:
    if (MRB_FLAG_TEST(obj, MRB_FLAG_IS_ORIGIN))
      mrb_gc_free_mt(mrb, (struct RClass*)obj);
    break;
  case MRB_TT_ENV:
    {
      struct REnv *e = (struct REnv*)obj;

      if (MRB_ENV_STACK_SHARED_P(e)) {
        /* cannot be freed */
        return;
      }
      if (!end) {
        int i;
        for (i = 0; i < MRB_ENV_STACK_LEN(e); ++i) {
          mrb_dec_ref(mrb, e->stack[i]);
        }
      }
      mrb_free(mrb, e->stack);
      e->stack = NULL;
    }
    break;

  case MRB_TT_FIBER:
    {
      struct mrb_context *c = ((struct RFiber*)obj)->cxt;

      if (!end && c && c != mrb->root_c) {
        mrb_callinfo *ci = c->ci;
        mrb_callinfo *ce = c->cibase;

        while (ce <= ci) {
          struct REnv *e = ci->env;
          if (e && e->tt != MRB_TT_FREE &&
              e->tt == MRB_TT_ENV && MRB_ENV_STACK_SHARED_P(e)) {
            mrb_env_unshare(mrb, e);
          }
          ci--;
        }
        mrb_free_context(mrb, c);
      }
    }
    break;

  case MRB_TT_ARRAY:
    if (ARY_SHARED_P(obj))
      mrb_ary_decref(mrb, ((struct RArray*)obj)->aux.shared);
    else {
      mrb_int i;
      if (!end) {
        for (i = 0; i < ((struct RArray*)obj)->len; ++i) {
          mrb_dec_ref(mrb, ((struct RArray*)obj)->ptr[i]);
        }
      }
      mrb_free(mrb, ((struct RArray*)obj)->ptr);
    }
    break;

  case MRB_TT_HASH:
    mrb_gc_free_iv(mrb, (struct RObject*)obj, end);
    mrb_gc_free_hash(mrb, (struct RHash*)obj, end);
    break;

  case MRB_TT_STRING:
    mrb_gc_free_str(mrb, (struct RString*)obj);
    break;

  case MRB_TT_PROC:
    {
      struct RProc *p = (struct RProc*)obj;

      if (!end) {
        if (p->env) mrb_obj_dec_ref(mrb, (struct RBasic*)p->env);
        if (p->target_class) mrb_obj_dec_ref(mrb, (struct RBasic*)p->target_class);
      }

      if (!MRB_PROC_CFUNC_P(p) && p->body.irep) {
        mrb_irep_decref(mrb, p->body.irep);
      }
    }
    break;

  case MRB_TT_RANGE:
    {
      struct RRange *r = (struct RRange*)obj;
      mrb_dec_ref(mrb, r->edges->beg);
      mrb_dec_ref(mrb, r->edges->end);
      mrb_free(mrb, r->edges);
    }
    break;

  case MRB_TT_DATA:
    {
      struct RData *d = (struct RData*)obj;
      if (d->type && d->type->dfree) {
        d->type->dfree(mrb, d->data);
      }
      mrb_gc_free_iv(mrb, (struct RObject*)obj, end);
    }
    break;

  default:
    break;
  }
  if (!end && obj->c) {
    mrb_obj_dec_ref(mrb, (struct RBasic*)obj->c);
  }

  obj->tt = MRB_TT_FREE;
  ((struct free_obj*)obj)->next = mrb->gc.freelist;
  mrb->gc.freelist = (struct RVALUE*)obj;
  mrb->gc.live--;
}

static void
root_scan_phase(mrb_state *mrb)
{
  size_t i, e;
  mrb_gc *gc = &mrb->gc;

  mrb_gc_mark_gv(mrb);
  /* mark arena */
  for (i=0,e=gc->arena_idx; i<e; i++) {
    mrb_gc_mark(mrb, gc->arena[i]);
  }
  /* mark class hierarchy */
  mrb_gc_mark(mrb, (struct RBasic*)mrb->object_class);

  /* mark built-in classes */
  mrb_gc_mark(mrb, (struct RBasic*)mrb->class_class);
  mrb_gc_mark(mrb, (struct RBasic*)mrb->module_class);
  mrb_gc_mark(mrb, (struct RBasic*)mrb->proc_class);
  mrb_gc_mark(mrb, (struct RBasic*)mrb->string_class);
  mrb_gc_mark(mrb, (struct RBasic*)mrb->array_class);
  mrb_gc_mark(mrb, (struct RBasic*)mrb->hash_class);

  mrb_gc_mark(mrb, (struct RBasic*)mrb->float_class);
  mrb_gc_mark(mrb, (struct RBasic*)mrb->fixnum_class);
  mrb_gc_mark(mrb, (struct RBasic*)mrb->true_class);
  mrb_gc_mark(mrb, (struct RBasic*)mrb->false_class);
  mrb_gc_mark(mrb, (struct RBasic*)mrb->nil_class);
  mrb_gc_mark(mrb, (struct RBasic*)mrb->symbol_class);
  mrb_gc_mark(mrb, (struct RBasic*)mrb->kernel_module);

  mrb_gc_mark(mrb, (struct RBasic*)mrb->eException_class);
  mrb_gc_mark(mrb, (struct RBasic*)mrb->eStandardError_class);

  /* mark top_self */
  mrb_gc_mark(mrb, (struct RBasic*)mrb->top_self);
  /* mark exception */
  mrb_gc_mark(mrb, (struct RBasic*)mrb->exc);
  /* mark pre-allocated exception */
  mrb_gc_mark(mrb, (struct RBasic*)mrb->nomem_err);
  mrb_gc_mark(mrb, (struct RBasic*)mrb->stack_err);
#ifdef MRB_GC_FIXED_ARENA
  mrb_gc_mark(mrb, (struct RBasic*)mrb->arena_err);
#endif

  mark_context(mrb, mrb->c);
  if (mrb->root_c != mrb->c) {
    mark_context(mrb, mrb->root_c);
  }
}

/* Perform a full gc cycle */
MRB_API void
mrb_full_gc(mrb_state *mrb)
{
  mrb_gc *gc = &mrb->gc;

  if (gc->disabled || gc->iterating || !gc->heaps) return;

  GC_INVOKE_TIME_REPORT("mrb_full_gc()");
  GC_TIME_START;

  mrb_assert(!gc->heap_pages_table);

  gc->heap_pages_count = 0;
  for (mrb_heap_page const *p = gc->heaps; p; p = p->next) { ++gc->heap_pages_count; }

  gc->heap_pages_table = (struct mrb_heap_page**)mrb_malloc(
      mrb, sizeof(struct mrb_heap_page*) * gc->heap_pages_count);
  int idx = 0;
  for (mrb_heap_page *p = gc->heaps; p; p = p->next) {
    gc->heap_pages_table[idx++] = p;
  }

  root_scan_phase(mrb);

  for (mrb_heap_page* page = gc->heaps; page != NULL; ) {
    int free_count = 0;
    mrb_heap_page *need_free = NULL;

    for (int i = 0; i < MRB_HEAP_PAGE_SIZE; ++i) {
      if (!page->mark_bits || !(page->mark_bits[i / 8] & (1 << (i % 8)))) {
        obj_free(mrb, &objects(page)[i].as.basic, FALSE);
      }
      if (objects(page)[i].as.basic.tt == MRB_TT_FREE) free_count++;
    }
    if (page->mark_bits) { mrb_free(mrb, page->mark_bits); }
    if (free_count == MRB_HEAP_PAGE_SIZE) { need_free = page; }

    page = page->next;

    if (need_free) {
      unlink_heap_page(gc, need_free);
      mrb_free(mrb, need_free);
    }
  }

  mrb_free(mrb, gc->heap_pages_table);
  gc->heap_pages_table = NULL;

  GC_TIME_STOP_AND_REPORT;
}

MRB_API void
mrb_garbage_collect(mrb_state *mrb)
{
  mrb_full_gc(mrb);
}

MRB_API int
mrb_gc_arena_save(mrb_state *mrb)
{
  return mrb->gc.arena_idx;
}

MRB_API void
mrb_gc_arena_restore(mrb_state *mrb, int idx)
{
  mrb_gc *gc = &mrb->gc;

  if (idx < gc->arena_idx) {
    for (int i = gc->arena_idx; i > idx; --i) {
      mrb_obj_dec_ref(mrb, gc->arena[i - 1]);
    }
  }

#ifndef MRB_GC_FIXED_ARENA
  int capa = gc->arena_capa;

  if (idx < capa / 2) {
    capa = (int)(capa * 0.66);
    if (capa < MRB_GC_ARENA_SIZE) {
      capa = MRB_GC_ARENA_SIZE;
    }
    if (capa != gc->arena_capa) {
      gc->arena = (struct RBasic**)mrb_realloc(mrb, gc->arena, sizeof(struct RBasic*)*capa);
      gc->arena_capa = capa;
    }
  }
#endif
  gc->arena_idx = idx;
}

/*
 *  call-seq:
 *     GC.start                     -> nil
 *
 *  Initiates full garbage collection.
 *
 */

static mrb_value
gc_start(mrb_state *mrb, mrb_value obj)
{
  mrb_full_gc(mrb);
  return mrb_nil_value();
}

/*
 *  call-seq:
 *     GC.enable    -> true or false
 *
 *  Enables garbage collection, returning <code>true</code> if garbage
 *  collection was previously disabled.
 *
 *     GC.disable   #=> false
 *     GC.enable    #=> true
 *     GC.enable    #=> false
 *
 */

static mrb_value
gc_enable(mrb_state *mrb, mrb_value obj)
{
  mrb_bool old = mrb->gc.disabled;

  mrb->gc.disabled = FALSE;

  return mrb_bool_value(old);
}

/*
 *  call-seq:
 *     GC.disable    -> true or false
 *
 *  Disables garbage collection, returning <code>true</code> if garbage
 *  collection was already disabled.
 *
 *     GC.disable   #=> false
 *     GC.disable   #=> true
 *
 */

static mrb_value
gc_disable(mrb_state *mrb, mrb_value obj)
{
  mrb_bool old = mrb->gc.disabled;

  mrb->gc.disabled = TRUE;

  return mrb_bool_value(old);
}

/*
 *  call-seq:
 *     GC.interval_ratio      -> fixnum
 *
 *  Returns ratio of GC interval. Default value is 200(%).
 *
 */

static mrb_value
gc_interval_ratio_get(mrb_state *mrb, mrb_value obj)
{
  return mrb_fixnum_value(-1);
}

/*
 *  call-seq:
 *     GC.interval_ratio = fixnum    -> nil
 *
 *  Updates ratio of GC interval. Default value is 200(%).
 *  GC start as soon as after end all step of GC if you set 100(%).
 *
 */

static mrb_value
gc_interval_ratio_set(mrb_state *mrb, mrb_value obj)
{
  mrb_int ratio;

  mrb_get_args(mrb, "i", &ratio);
  return mrb_nil_value();
}

/*
 *  call-seq:
 *     GC.step_ratio    -> fixnum
 *
 *  Returns step span ratio of Incremental GC. Default value is 200(%).
 *
 */

static mrb_value
gc_step_ratio_get(mrb_state *mrb, mrb_value obj)
{
  return mrb_fixnum_value(-1);
}

/*
 *  call-seq:
 *     GC.step_ratio = fixnum   -> nil
 *
 *  Updates step span ratio of Incremental GC. Default value is 200(%).
 *  1 step of incrementalGC becomes long if a rate is big.
 *
 */

static mrb_value
gc_step_ratio_set(mrb_state *mrb, mrb_value obj)
{
  mrb_int ratio;

  mrb_get_args(mrb, "i", &ratio);
  return mrb_nil_value();
}

/*
 *  call-seq:
 *     GC.generational_mode -> true or false
 *
 *  Returns generational or normal gc mode.
 *
 */

static mrb_value
gc_generational_mode_get(mrb_state *mrb, mrb_value self)
{
  return mrb_false_value();
}

/*
 *  call-seq:
 *     GC.generational_mode = true or false -> true or false
 *
 *  Changes to generational or normal gc mode.
 *
 */

static mrb_value
gc_generational_mode_set(mrb_state *mrb, mrb_value self)
{
  mrb_bool enable;

  mrb_get_args(mrb, "b", &enable);
  return mrb_false_value();
}


static void
gc_each_objects(mrb_state *mrb, mrb_gc *gc, mrb_each_object_callback *callback, void *data)
{
  mrb_heap_page* page;

  page = gc->heaps;
  while (page != NULL) {
    RVALUE *p;
    int i;

    p = objects(page);
    for (i=0; i < MRB_HEAP_PAGE_SIZE; i++) {
      if ((*callback)(mrb, &p[i].as.basic, data) == MRB_EACH_OBJ_BREAK)
        return;
    }
    page = page->next;
  }
}

void
mrb_objspace_each_objects(mrb_state *mrb, mrb_each_object_callback *callback, void *data)
{
  mrb_bool iterating = mrb->gc.iterating;

  mrb->gc.iterating = TRUE;
  if (iterating) {
    gc_each_objects(mrb, &mrb->gc, callback, data);
  }
  else {
    struct mrb_jmpbuf *prev_jmp = mrb->jmp;
    struct mrb_jmpbuf c_jmp;

    MRB_TRY(&c_jmp) {
      mrb->jmp = &c_jmp;
      gc_each_objects(mrb, &mrb->gc, callback, data);
      mrb->jmp = prev_jmp;
      mrb->gc.iterating = iterating; 
   } MRB_CATCH(&c_jmp) {
      mrb->jmp = prev_jmp;
      mrb->gc.iterating = iterating;
      if (mrb->exc) {
        mrb_value exc = mrb_obj_value(mrb->exc);
        mrb->exc = NULL;
        mrb_exc_raise(mrb, exc);
      }
    } MRB_END_EXC(&c_jmp);
  }
}

MRB_API void
mrb_obj_inc_ref(mrb_state *mrb, struct RBasic *obj)
{
  if (obj->ref_count == REF_COUNT_MAX) { return; }

  mrb_assert(obj->tt != MRB_TT_FREE);

  ++obj->ref_count;
}

MRB_API void
mrb_obj_dec_ref(mrb_state *mrb, struct RBasic *obj)
{
  if (obj->ref_count == REF_COUNT_MAX) { return; }

  mrb_assert(obj->tt != MRB_TT_FREE);

  if (!obj->ref_count) {
    obj_free(mrb, obj, FALSE);
  }
  else {
    --obj->ref_count;
  }
}

MRB_API void
mrb_inc_ref(mrb_state* mrb, mrb_value v)
{
  if (!mrb_immediate_p(v)) {
    mrb_obj_inc_ref(mrb, mrb_basic_ptr(v));
  }
}

MRB_API void
mrb_dec_ref(mrb_state* mrb, mrb_value v)
{
  if (!mrb_immediate_p(v)) {
    mrb_obj_dec_ref(mrb, mrb_basic_ptr(v));
  }
}

void
mrb_init_gc(mrb_state *mrb)
{
  struct RClass *gc;

  gc = mrb_define_module(mrb, "GC");

  mrb_define_class_method(mrb, gc, "start", gc_start, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, gc, "enable", gc_enable, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, gc, "disable", gc_disable, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, gc, "interval_ratio", gc_interval_ratio_get, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, gc, "interval_ratio=", gc_interval_ratio_set, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, gc, "step_ratio", gc_step_ratio_get, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, gc, "step_ratio=", gc_step_ratio_set, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, gc, "generational_mode=", gc_generational_mode_set, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, gc, "generational_mode", gc_generational_mode_get, MRB_ARGS_NONE());
}
