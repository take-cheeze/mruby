/*
 * Thread: a Ruby thread is a libuv thread (uv_thread_create, the library mruby-uv binds) running its block
 * on its own context (mrb_fiber_new gives the context; its frame is the block's).
 *
 * The VM is not thread-safe, so a global lock (GVL) is held while a thread runs Ruby code. It is released only
 * where a thread can block: Thread#join. There is no preemption: a thread that does not block keeps the VM, as
 * a thread that never joins or waits would in a non-preemptive scheduler.
 *
 * The execution state of a thread is saved when it releases the GVL and loaded when it takes the GVL again:
 * mrb->c (its context), mrb->jmp (its error jump), mrb->exc and mrb->errinfo, and its GC arena entries. The
 * arena entries of a thread that is not running are kept in an array of its record, which the GC marks.
 */
#include <mruby.h>
#include <mruby/array.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/error.h>
#include <mruby/object.h>
#include <mruby/proc.h>
#include <mruby/variable.h>
#include <uv.h>
#include <string.h>

typedef struct thr {
  mrb_value self;          /* the Thread object */
  mrb_value proc;          /* the block */
  mrb_value args;          /* its arguments */
  mrb_value fiber;         /* the fiber whose context runs the block (marks the context) */
  mrb_value value;         /* the block's value, once done */
  mrb_value exc;           /* the exception the block raised, or nil */
  mrb_value arena_keep;    /* the arena entries of this thread while it does not hold the GVL, or nil */
  int started;
  int done;
  int joined;
  int self_set;             /* the record's Thread object exists (self is valid) */
  uv_thread_t th;
  /* execution state, valid while the thread does not hold the GVL */
  struct mrb_context *cxt;
  struct mrb_jmpbuf *jmp;
  struct RObject *exc_state;
  struct RObject *errinfo;
  mrb_int errinfo_ci_depth;
} thr;

static mrb_state *G;                /* the one VM all threads share */
static uv_mutex_t gvl;
static int gvl_ready = 0;
static thr *holder = NULL;          /* the thread holding the GVL */
static thr *main_thr = NULL;

static void
thr_mark(mrb_state *mrb, void *p)
{
  thr *t = (thr*)p;
  if (!t) return;
  mrb_gc_mark_value(mrb, t->self);
  mrb_gc_mark_value(mrb, t->proc);
  mrb_gc_mark_value(mrb, t->args);
  mrb_gc_mark_value(mrb, t->fiber);
  mrb_gc_mark_value(mrb, t->value);
  mrb_gc_mark_value(mrb, t->exc);
  mrb_gc_mark_value(mrb, t->arena_keep);
}

static void
thr_free(mrb_state *mrb, void *p)
{
  if (p && p != main_thr) mrb_free(mrb, p);
}

/* A value stored into a thread record: the record's Thread object must see it under the incremental GC (a write
   barrier), since the record is C memory the GC scans through the object. */
static void
thr_barrier(mrb_state *mrb, thr *t, mrb_value v)
{
  if (t->self_set && !mrb_immediate_p(v)) {
    mrb_field_write_barrier_value(mrb, mrb_basic_ptr(t->self), v);
  }
}

static const struct mrb_data_type thr_type = { .struct_name = "Thread", .dfree = thr_free, .mark = thr_mark };

/* The execution state of t is the VM's, while t holds the GVL: save it when t gives the GVL up. */
static void
vm_save(mrb_state *mrb, thr *t)
{
  mrb_int i, n = mrb->gc.arena_idx;
  mrb_value keep = mrb_ary_new_capa(mrb, n);

  for (i = 0; i < n; i++) {
    mrb_ary_push(mrb, keep, mrb_obj_value((void*)mrb->gc.arena[i]));
  }
  t->arena_keep = keep;
  thr_barrier(mrb, t, keep);
  mrb->gc.arena_idx = 0;
  t->cxt = mrb->c;
  t->jmp = mrb->jmp;
  t->exc_state = mrb->exc;
  t->errinfo = mrb->errinfo;
  t->errinfo_ci_depth = mrb->errinfo_ci_depth;
}

/* Load t's state into the VM when it takes the GVL. */
static void
vm_load(mrb_state *mrb, thr *t)
{
  mrb_int i, n = 0;

  mrb->c = t->cxt;
  mrb->jmp = t->jmp;
  mrb->exc = t->exc_state;
  mrb->errinfo = t->errinfo;
  mrb->errinfo_ci_depth = t->errinfo_ci_depth;
  mrb->gc.arena_idx = 0;
  if (mrb_array_p(t->arena_keep)) {
    n = RARRAY_LEN(t->arena_keep);
    for (i = 0; i < n; i++) {
      mrb_gc_protect(mrb, mrb_ary_ref(mrb, t->arena_keep, i));
    }
  }
  t->arena_keep = mrb_nil_value();
}

static void
gvl_release(mrb_state *mrb, thr *me)
{
  vm_save(mrb, me);
  holder = NULL;
  uv_mutex_unlock(&gvl);
}

static void
gvl_acquire(mrb_state *mrb, thr *me)
{
  uv_mutex_lock(&gvl);
  holder = me;
  vm_load(mrb, me);
}

static mrb_value
run_block(mrb_state *mrb, mrb_value self)
{
  thr *t = (thr*)DATA_PTR(self);
  return mrb_yield_argv(mrb, t->proc, (mrb_int)RARRAY_LEN(t->args), RARRAY_PTR(t->args));
}

/* The body of an OS thread: it takes the GVL, runs the block in its own context and gives the GVL up. */
static void
thread_entry(void *arg)
{
  thr *t = (thr*)arg;
  mrb_state *mrb = G;
  mrb_bool state = FALSE;
  mrb_value res;

  uv_mutex_lock(&gvl);
  holder = t;
  mrb->c = t->cxt;
  mrb->jmp = NULL;
  mrb->exc = NULL;
  mrb->errinfo = NULL;
  mrb->errinfo_ci_depth = 0;
  mrb->gc.arena_idx = 0;

  res = mrb_protect(mrb, run_block, t->self, &state);
  if (state) {
    t->exc = res;
  }
  else {
    t->value = res;
  }
  thr_barrier(mrb, t, res);
  t->done = 1;
  holder = NULL;
  uv_mutex_unlock(&gvl);
}

static void
gvl_init(mrb_state *mrb)
{
  if (gvl_ready) return;
  G = mrb;
  uv_mutex_init(&gvl);
  gvl_ready = 1;
  /* the main thread holds the GVL from the first Thread */
  main_thr = (thr*)mrb_calloc(mrb, 1, sizeof(thr));
  main_thr->self = mrb_nil_value();
  main_thr->proc = mrb_nil_value();
  main_thr->args = mrb_nil_value();
  main_thr->fiber = mrb_nil_value();
  main_thr->value = mrb_nil_value();
  main_thr->exc = mrb_nil_value();
  main_thr->arena_keep = mrb_nil_value();
  main_thr->started = 1;
  main_thr->joined = 1;
  {
    struct RClass *c = mrb_class_get(mrb, "Thread");
    mrb_value self = mrb_obj_value((void*)mrb_obj_alloc(mrb, MRB_TT_CDATA, c));
    DATA_PTR(self) = main_thr;
    DATA_TYPE(self) = &thr_type;
    main_thr->self = self;
    main_thr->self_set = 1;
    mrb_gc_register(mrb, self);
  }
  uv_mutex_lock(&gvl);
  holder = main_thr;
}

static mrb_value
thr_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_value blk = mrb_nil_value();
  mrb_value *argv = NULL;
  mrb_int argc = 0;
  thr *t;
  int rc;

  mrb_get_args(mrb, "*&", &argv, &argc, &blk);
  if (mrb_nil_p(blk)) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "must be called with a block");
  }
  gvl_init(mrb);

  t = (thr*)mrb_calloc(mrb, 1, sizeof(thr));
  t->self = self;
  t->self_set = 1;
  t->proc = blk;
  t->args = mrb_ary_new_from_values(mrb, argc, argv);
  thr_barrier(mrb, t, blk);
  thr_barrier(mrb, t, t->args);
  t->value = mrb_nil_value();
  t->exc = mrb_nil_value();
  t->arena_keep = mrb_nil_value();
  t->fiber = mrb_fiber_new(mrb, mrb_proc_ptr(blk));
  thr_barrier(mrb, t, t->fiber);
  t->cxt = ((struct RFiber*)mrb_ptr(t->fiber))->cxt;
  t->exc_state = NULL;
  t->errinfo = NULL;
  DATA_PTR(self) = t;
  DATA_TYPE(self) = &thr_type;

  mrb_gc_register(mrb, self);
  rc = uv_thread_create(&t->th, thread_entry, t);
  if (rc != 0) {
    mrb_gc_unregister(mrb, self);
    mrb_raisef(mrb, E_RUNTIME_ERROR, "can't create thread: %S", mrb_str_new_cstr(mrb, uv_strerror(rc)));
  }
  t->started = 1;
  return self;
}

static thr*
thr_of(mrb_state *mrb, mrb_value self)
{
  thr *t = (thr*)DATA_PTR(self);
  if (!t) mrb_raise(mrb, E_RUNTIME_ERROR, "uninitialized thread");
  return t;
}

static mrb_value
thr_join(mrb_state *mrb, mrb_value self)
{
  thr *t = thr_of(mrb, self);
  thr *me = holder;

  if (t == me) {
    mrb_raise(mrb, mrb_class_get(mrb, "ThreadError"), "Target thread must not be current thread");
  }
  if (!t->joined) {
    if (me) gvl_release(mrb, me);
    uv_thread_join(&t->th);
    if (me) gvl_acquire(mrb, me);
    t->joined = 1;
    mrb_gc_unregister(mrb, self);
  }
  if (!mrb_nil_p(t->exc)) {
    mrb_exc_raise(mrb, t->exc);
  }
  return self;
}

static mrb_value
thr_value(mrb_state *mrb, mrb_value self)
{
  thr *t;
  thr_join(mrb, self);
  t = thr_of(mrb, self);
  return t->value;
}

static mrb_value
thr_current(mrb_state *mrb, mrb_value klass)
{
  (void)klass;
  if (!holder) return mrb_nil_value();
  return holder->self;
}


/* Mutex: a uv_mutex. A thread that must wait for it gives the GVL up while it waits (so the holder can run). */
typedef struct mtx {
  uv_mutex_t m;
  thr *owner;               /* the thread holding it, or NULL */
  int inited;
} mtx;

static void
mtx_free(mrb_state *mrb, void *p)
{
  mtx *x = (mtx*)p;
  if (x) {
    if (x->inited) uv_mutex_destroy(&x->m);
    mrb_free(mrb, x);
  }
}

static const struct mrb_data_type mtx_type = { .struct_name = "Mutex", .dfree = mtx_free, .mark = NULL };

static mtx*
mtx_of(mrb_state *mrb, mrb_value self)
{
  mtx *x = (mtx*)DATA_PTR(self);
  if (!x) mrb_raise(mrb, E_RUNTIME_ERROR, "uninitialized mutex");
  return x;
}

static mrb_value
mtx_initialize(mrb_state *mrb, mrb_value self)
{
  mtx *x = (mtx*)mrb_calloc(mrb, 1, sizeof(mtx));
  gvl_init(mrb);
  if (uv_mutex_init(&x->m) != 0) {
    mrb_free(mrb, x);
    mrb_raise(mrb, E_RUNTIME_ERROR, "can't create mutex");
  }
  x->inited = 1;
  DATA_PTR(self) = x;
  DATA_TYPE(self) = &mtx_type;
  return self;
}

static mrb_value
mtx_lock(mrb_state *mrb, mrb_value self)
{
  mtx *x = mtx_of(mrb, self);
  thr *me = holder;

  if (x->owner == me) {
    mrb_raise(mrb, mrb_class_get(mrb, "ThreadError"), "deadlock; recursive locking");
  }
  if (uv_mutex_trylock(&x->m) != 0) {
    if (me) gvl_release(mrb, me);
    uv_mutex_lock(&x->m);
    if (me) gvl_acquire(mrb, me);
  }
  x->owner = me;
  return self;
}

static mrb_value
mtx_try_lock(mrb_state *mrb, mrb_value self)
{
  mtx *x = mtx_of(mrb, self);
  if (uv_mutex_trylock(&x->m) != 0) return mrb_false_value();
  x->owner = holder;
  return mrb_true_value();
}

static mrb_value
mtx_unlock(mrb_state *mrb, mrb_value self)
{
  mtx *x = mtx_of(mrb, self);
  if (x->owner != holder) {
    mrb_raise(mrb, mrb_class_get(mrb, "ThreadError"), "Attempt to unlock a mutex which is not locked");
  }
  x->owner = NULL;
  uv_mutex_unlock(&x->m);
  return self;
}

static mrb_value
mtx_locked_p(mrb_state *mrb, mrb_value self)
{
  mtx *x = mtx_of(mrb, self);
  return mrb_bool_value(x->owner != NULL);
}

static mrb_value
mtx_owned_p(mrb_state *mrb, mrb_value self)
{
  mtx *x = mtx_of(mrb, self);
  return mrb_bool_value(x->owner != NULL && x->owner == holder);
}

void
mrb_mruby_uv_thread_gem_init(mrb_state *mrb)
{
  mrb_define_class(mrb, "ThreadError", mrb_class_get(mrb, "StandardError"));
  struct RClass *c = mrb_define_class(mrb, "Thread", mrb->object_class);
  MRB_SET_INSTANCE_TT(c, MRB_TT_CDATA);
  mrb_define_method(mrb, c, "initialize", thr_initialize, MRB_ARGS_ANY() | MRB_ARGS_BLOCK());
  mrb_define_method(mrb, c, "join", thr_join, MRB_ARGS_NONE());
  mrb_define_method(mrb, c, "value", thr_value, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, c, "current", thr_current, MRB_ARGS_NONE());
  c = mrb_define_class(mrb, "Mutex", mrb->object_class);
  MRB_SET_INSTANCE_TT(c, MRB_TT_CDATA);
  mrb_define_method(mrb, c, "initialize", mtx_initialize, MRB_ARGS_NONE());
  mrb_define_method(mrb, c, "lock", mtx_lock, MRB_ARGS_NONE());
  mrb_define_method(mrb, c, "try_lock", mtx_try_lock, MRB_ARGS_NONE());
  mrb_define_method(mrb, c, "unlock", mtx_unlock, MRB_ARGS_NONE());
  mrb_define_method(mrb, c, "locked?", mtx_locked_p, MRB_ARGS_NONE());
  mrb_define_method(mrb, c, "owned?", mtx_owned_p, MRB_ARGS_NONE());
}

void
mrb_mruby_uv_thread_gem_final(mrb_state *mrb)
{
  (void)mrb;
}
