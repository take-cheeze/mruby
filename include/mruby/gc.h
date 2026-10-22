/**
** @file mruby/gc.h - garbage collector for mruby
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

#define MRB_EACH_OBJ_OK 0
#define MRB_EACH_OBJ_BREAK 1
typedef int (mrb_each_object_callback)(struct mrb_state *mrb, struct RBasic *obj, void *data);
void mrb_objspace_each_objects(struct mrb_state *mrb, mrb_each_object_callback *callback, void *data);
size_t mrb_objspace_page_slot_size(void);
MRB_API void mrb_free_context(struct mrb_state *mrb, struct mrb_context *c);

#ifndef MRB_GC_ARENA_SIZE
#define MRB_GC_ARENA_SIZE 100
#endif

#ifndef MRB_GRAY_STACK_SIZE
#define MRB_GRAY_STACK_SIZE 1024
#endif

typedef enum {
  MRB_GC_STATE_ROOT = 0,
  MRB_GC_STATE_MARK,
  MRB_GC_STATE_SWEEP
} mrb_gc_state;

typedef struct mrb_gc {
  struct mrb_heap_page *heaps;     /* all heaps pages */
  struct mrb_heap_page *free_heaps;/* heaps for allocation */
  struct mrb_heap_page *sweeps;    /* page where sweep starts */
  struct mrb_heap_region *regions;  /* contiguous heap regions */
  struct RBasic *gray_stack[MRB_GRAY_STACK_SIZE]; /* stack of gray objects */
  size_t gray_stack_top;           /* top index of gray stack */
  mrb_bool gray_overflow:1;        /* gray stack overflowed; needs heap rescan */
  size_t live;                     /* count of live objects */
  size_t live_after_mark;          /* old generation objects */
  size_t tt_live[MRB_TT_MAXDEFINE];   /* live objects, by mrb_vtype */
  size_t tt_allocs[MRB_TT_MAXDEFINE]; /* cumulative allocations, by mrb_vtype */
  size_t threshold;                /* threshold to start GC */
  size_t oldgen_threshold;         /* threshold to kick major GC */
  mrb_gc_state state;              /* current state of gc */
  int interval_ratio;
  int step_ratio;
  int current_white_part :2;       /* make white object by white_part */
  mrb_bool iterating     :1;       /* currently iterating over objects */
  mrb_bool disabled      :1;       /* GC disabled */
  mrb_bool generational  :1;       /* generational GC mode */
  mrb_bool full          :1;       /* major GC mode */
  mrb_bool out_of_memory :1;       /* out-of-memory error occurred */

#ifdef MRB_GC_FIXED_ARENA
  struct RBasic *arena[MRB_GC_ARENA_SIZE]; /* GC protection array */
#else
  struct RBasic **arena;                   /* GC protection array */
  int arena_capa;                          /* size of protection array */
#endif
  int arena_idx;
} mrb_gc;

MRB_API mrb_bool mrb_object_dead_p(struct mrb_state *mrb, struct RBasic *object);
MRB_API int mrb_gc_add_region(struct mrb_state *mrb, void *start, size_t size);

/*
 * Makes a frozen String persistent (ADR 0546): the sweep leaves it alone until
 * mrb_close, and the mark does not traverse it. Raises unless the String is
 * frozen, of the String class and has no shared or fshared buffer, since those
 * are the only references a String can hold to another collectable object.
 */
MRB_API void mrb_obj_set_persistent(struct mrb_state *mrb, mrb_value obj);

/*
 * Snapshot the GC's per-type live/allocation counters into caller-owned
 * arrays of MRB_TT_MAXDEFINE size_t entries each (index by enum mrb_vtype,
 * e.g. MRB_TT_STRING). live_out reports currently-live objects of that type;
 * alloc_out reports cumulative allocations since mrb_open(). Either pointer
 * may be NULL to skip that half. O(MRB_TT_MAXDEFINE) -- no heap walk, no GC
 * triggered: the counters are maintained incrementally by mrb_obj_alloc()
 * and the sweep phase's obj_free(), so this is safe to call every frame.
 */
MRB_API void mrb_gc_type_counts(struct mrb_state *mrb, size_t *live_out, size_t *alloc_out);

#define MRB_GC_RED 7

MRB_END_DECL

#endif  /* MRUBY_GC_H */
