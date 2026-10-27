/**
** @file mruby/class.h - Class class
**
** See Copyright Notice in mruby.h
*/

#ifndef MRUBY_CLASS_H
#define MRUBY_CLASS_H

#include "common.h"

/**
 * Class class
 */
MRB_BEGIN_DECL

struct RClass {
  MRB_OBJECT_HEADER;
  struct iv_tbl *iv;
  struct mrb_mt_tbl *mt;
  struct RClass *super;
};

#define mrb_class_ptr(v)    ((struct RClass*)(mrb_ptr(v)))

MRB_INLINE struct RClass*
mrb_class(mrb_state *mrb, mrb_value v)
{
  if (!mrb_immediate_p(v)) {
    return mrb_obj_ptr(v)->c;
  }

  switch (mrb_type(v)) {
  case MRB_TT_FALSE:
    if (mrb_fixnum(v))
      return mrb->false_class;
    return mrb->nil_class;
  case MRB_TT_TRUE:
    return mrb->true_class;
  case MRB_TT_SYMBOL:
    return mrb->symbol_class;
  case MRB_TT_INTEGER:
    return mrb->integer_class;
#ifndef MRB_NO_FLOAT
  case MRB_TT_FLOAT:
    return mrb->float_class;
#endif
  case MRB_TT_CPTR:
    return mrb->object_class;
  default:
    return NULL;
  }
}

/* flags:
   20:   frozen
   19:   is_prepended
   18:   is_origin
   17:   is_inherited (used by method cache)
   7-16: unused
   6:    prohibit Class#allocate
   0-5:  instance type
*/
#define MRB_FL_CLASS_IS_PREPENDED (1 << 19)
#define MRB_FL_CLASS_IS_ORIGIN (1 << 18)
#define MRB_CLASS_ORIGIN(c) do {\
  if ((c)->flags & MRB_FL_CLASS_IS_PREPENDED) {\
    (c) = (c)->super;\
    while (!((c)->flags & MRB_FL_CLASS_IS_ORIGIN)) {\
      (c) = (c)->super;\
    }\
  }\
} while (0)
#define MRB_FL_CLASS_IS_INHERITED (1 << 17)
#define MRB_INSTANCE_TT_MASK (0x1F)
#define MRB_SET_INSTANCE_TT(c, tt) ((c)->flags = (((c)->flags & ~MRB_INSTANCE_TT_MASK) | (char)(tt)))
#define MRB_INSTANCE_TT(c) (enum mrb_vtype)((c)->flags & MRB_INSTANCE_TT_MASK)
#define MRB_FL_UNDEF_ALLOCATE (1 << 6)
#define MRB_UNDEF_ALLOCATOR(c) (mrb_assert((c)->tt == MRB_TT_CLASS), (c)->flags |= MRB_FL_UNDEF_ALLOCATE)
#define MRB_UNDEF_ALLOCATOR_P(c) ((c)->flags & MRB_FL_UNDEF_ALLOCATE)
#define MRB_DEFINE_ALLOCATOR(c) ((c)->flags &= ~MRB_FL_UNDEF_ALLOCATE)

MRB_API void mrb_define_method_raw(mrb_state*, struct RClass*, mrb_sym, mrb_method_t);
MRB_API void mrb_alias_method(mrb_state*, struct RClass *c, mrb_sym a, mrb_sym b);
MRB_API void mrb_remove_method(mrb_state *mrb, struct RClass *c, mrb_sym sym);

MRB_API mrb_method_t mrb_method_search_vm(mrb_state*, struct RClass**, mrb_sym);
MRB_API mrb_method_t mrb_method_search(mrb_state*, struct RClass*, mrb_sym);

MRB_API struct RClass* mrb_class_real(struct RClass* cl);
MRB_API struct RClass* mrb_class_outer(mrb_state *mrb, struct RClass *c);

#ifndef MRB_NO_METHOD_CACHE
void mrb_mc_clear_by_class(mrb_state *mrb, struct RClass* c);
#else
#define mrb_mc_clear_by_class(mrb,c)
#endif

/* return non zero to break the loop */
typedef int (mrb_mt_foreach_func)(mrb_state*,mrb_sym,mrb_method_t,void*);
MRB_API void mrb_mt_foreach(mrb_state*, struct RClass*, mrb_mt_foreach_func*, void*);

/* ROM method table types for static method registration */
union mrb_mt_ptr {
  const struct RProc *proc;
  mrb_func_t func;
};

/* entry combining function pointer, symbol key, and flags */
typedef struct mrb_mt_entry {
  union mrb_mt_ptr val;
  mrb_sym key;              /* pure symbol ID (no flags packed) */
  uint32_t flags;           /* method flags + aspec */
} mrb_mt_entry;

typedef struct mrb_mt_tbl {
  int               size;
  int               alloc;  /* bit 30: MRB_MT_READONLY_BIT, bit 29: MRB_MT_FROZEN_BIT */
  mrb_mt_entry     *ptr;
  struct mrb_mt_tbl *next;
} mrb_mt_tbl;

#define MRB_MT_READONLY_BIT  (1 << 30)
#define MRB_MT_FROZEN_BIT    (1 << 29)
#define MRB_MT_FUNC    (1 << 24)  /* MRB_METHOD_FUNC_FL */
#define MRB_MT_PUBLIC  0
#define MRB_MT_PRIVATE (1 << 25)  /* MRB_METHOD_PRIVATE_FL */

/* ROM table entry: 3rd param is MRB_ARGS_*() optionally OR'd with MRB_MT_PRIVATE.
   The macro supplies its own trailing comma, so entries are written
   without one; this lets an excluded entry vanish from the table. */
#define MRB_MT_ENTRY_ROM_(fn, sym, flags) \
  { { .func = (fn) }, (sym), (flags) | MRB_MT_FUNC },

#ifndef MRB_MT_EXCLUDE_HEADER
#define MRB_MT_ENTRY(fn, sym, flags) MRB_MT_ENTRY_ROM_(fn, sym, flags)
#else
/* Build-time exclusion of ROM entries (see doc/guides/rom-method-table.md).
   MRB_MT_EXCLUDE_HEADER names a header (quoted or <>) that defines
   MRB_MT_EXCLUDE_<Class>_<presym C name> to 1 for each entry to drop, e.g.
     #define MRB_MT_EXCLUDE_String_SYM__squeeze 1     String#squeeze
     #define MRB_MT_EXCLUDE_Array_SYM_B__rotate 1     Array#rotate!
     #define MRB_MT_EXCLUDE_Integer_OPSYM__cmp 1      Integer#<=>
   <Class> is the MRB_MT_CLASS a table defines before its entries. */
#include MRB_MT_EXCLUDE_HEADER

/* MRB_SYM_x(name) -> MRB_MT_EXCLUDE_<MRB_MT_CLASS>_SYM_x__name */
#define MRB_MT_XNAME_MRB_SYM(n)   MRB_MT_XNAME_(MRB_MT_CLASS, SYM__##n)
#define MRB_MT_XNAME_MRB_SYM_B(n) MRB_MT_XNAME_(MRB_MT_CLASS, SYM_B__##n)
#define MRB_MT_XNAME_MRB_SYM_Q(n) MRB_MT_XNAME_(MRB_MT_CLASS, SYM_Q__##n)
#define MRB_MT_XNAME_MRB_SYM_E(n) MRB_MT_XNAME_(MRB_MT_CLASS, SYM_E__##n)
#define MRB_MT_XNAME_MRB_OPSYM(n) MRB_MT_XNAME_(MRB_MT_CLASS, OPSYM__##n)
#define MRB_MT_XNAME_(c, s) MRB_MT_XNAME2_(c, s)
#define MRB_MT_XNAME2_(c, s) MRB_MT_EXCLUDE_##c##_##s

/* select KEEP or DROP depending on whether the name is defined to 1 */
#define MRB_MT_ENTRY(fn, sym, flags) MRB_MT_ENTRY1_(MRB_MT_XNAME_##sym, fn, sym, flags)
#define MRB_MT_ENTRY1_(x, fn, sym, flags) MRB_MT_ENTRY2_(x, fn, sym, flags)
#define MRB_MT_ENTRY2_(x, fn, sym, flags) MRB_MT_ENTRY3_(MRB_MT_EXCL_P_##x, fn, sym, flags)
#define MRB_MT_ENTRY3_(p, fn, sym, flags) MRB_MT_EXPAND_(MRB_MT_SEL_(p MRB_MT_DROP_, MRB_MT_ENTRY_ROM_, ~))(fn, sym, flags)
#define MRB_MT_EXCL_P_1 ~,
#define MRB_MT_SEL_(a, b, ...) b
#define MRB_MT_EXPAND_(x) x
#define MRB_MT_DROP_(fn, sym, flags)

/* an excluded C function is left unreferenced; let the compiler drop it quietly */
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic ignored "-Wunused-function"
#endif
#endif /* MRB_MT_EXCLUDE_HEADER */
#define MRB_MT_ASPEC(flags) ((mrb_aspec)((flags) & 0xffffff))

/* "removed" tombstone: MRB_MT_FUNC flag set with NULL function pointer.
   This combination never occurs naturally (C functions are never NULL).
   Unlike undef (proc=NULL without MRB_MT_FUNC), a removed marker makes
   mt_get() return 0 ("not found"), blocking ROM chain walk while
   allowing superclass lookup. */
#define MRB_MT_REMOVED_P(e) (((e).flags&MRB_MT_FUNC) && (e).val.func==NULL)

/* Singly-linked list node for tracking heap-allocated ROM wrappers. */
struct mrb_mt_rom_list {
  mrb_mt_tbl *tbl;
  struct mrb_mt_rom_list *next;
};

/* Allocate a per-state ROM layer wrapping the const entries array,
   and push it onto the class's method table chain. */
void mrb_mt_init_rom(mrb_state *mrb, struct RClass *c,
                     const mrb_mt_entry *entries, int size);
#define MRB_MT_INIT_ROM(mrb, cls, entries) \
  mrb_mt_init_rom(mrb, cls, entries, \
                  (int)(sizeof(entries)/sizeof(entries[0])))

MRB_END_DECL

#endif  /* MRUBY_CLASS_H */
