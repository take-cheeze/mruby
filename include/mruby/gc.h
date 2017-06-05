/*
** mruby/gc.h - garbage collector for mruby
**
** See Copyright Notice in mruby.h
*/

#ifndef MRUBY_GC_H
#define MRUBY_GC_H

#include "common.h"

/**
 * Uncommon memory management stuffs.
 */
MRB_BEGIN_DECL


struct mrb_state;

typedef enum MRB_EACH_OBJ_STATE {
  MRB_EACH_OBJ_OK = 0,
  MRB_EACH_OBJ_BREAK = 1,
} MRB_EACH_OBJ_STATE;
typedef MRB_EACH_OBJ_STATE (mrb_each_object_callback)(struct mrb_state *mrb, struct RBasic *obj, void *data);
void mrb_objspace_each_objects(struct mrb_state *mrb, mrb_each_object_callback *callback, void *data);
MRB_API void mrb_free_context(struct mrb_state *mrb, struct mrb_context *c);

#ifndef MRB_GC_ARENA_SIZE
#define MRB_GC_ARENA_SIZE 100
#endif

typedef struct mrb_heap_page {
  struct mrb_heap_page *prev;
  struct mrb_heap_page *next;
  uint8_t *mark_bits;
  void *objects[];
} mrb_heap_page;

typedef struct mrb_gc {
  mrb_heap_page *heaps;                /* heaps for GC */
  size_t live; /* count of live objects */
#ifdef MRB_GC_FIXED_ARENA
  struct RBasic *arena[MRB_GC_ARENA_SIZE]; /* GC protection array */
#else
  struct RBasic **arena;                   /* GC protection array */
  int arena_capa;
#endif
  int arena_idx;

  struct RVALUE *freelist;

#ifndef MRB_REF_COUNT_AUTO_PERMANENT
  struct kh_rc *strict_counts;
#endif

  mrb_bool iterating     :1;
  mrb_bool disabled      :1;
  mrb_bool out_of_memory :1;
} mrb_gc;

MRB_API mrb_bool
mrb_object_dead_p(struct mrb_state *mrb, struct RBasic *object);

MRB_END_DECL

#endif  /* MRUBY_GC_H */
