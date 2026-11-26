/**
** @file mruby/rom_graph.h - classes as const data
**
** See Copyright Notice in mruby.h
*/

#ifndef MRUBY_ROM_GRAPH_H
#define MRUBY_ROM_GRAPH_H

#include "common.h"

/*
 * A ROM class graph (MRB_ROM_CLASS_GRAPH) is the class, module, singleton class and iclass objects a boot leaves,
 * written out by mrb_rom_graph_dump as C: const RClass structs (painted MRB_GC_RED), their constant and instance
 * variable tables, and the links between them. A program that links that C file gets those objects in place of the
 * ones the boot would allocate: mrb_init_class takes the four root classes from it, and every later definition finds
 * its class already defined, as a reopening does. Each class with methods has a RAM slot for its method table chain
 * (the boot installs the tables there). A class's constant table is const; the first store that changes it copies it
 * to the heap. See doc/guides/rom-class-graph.md.
 */

MRB_BEGIN_DECL

/* iv_tbl's layout (variable.c checks it) */
struct mrb_rom_iv {
  int size, alloc;
  mrb_value *ptr;
};

#define MRB_ROM_GRAPH_VERSION 1
#define MRB_ROM_GRAPH_ABI \
  ((uint32_t)sizeof(struct RClass) | ((uint32_t)sizeof(mrb_value) << 8) | ((uint32_t)MRB_ROM_GRAPH_VERSION << 16))

struct mrb_rom_graph {
  uint32_t abi;                    /* MRB_ROM_GRAPH_ABI of the program that compiled the graph */
  uint32_t nclasses, nmt, niv;
  struct RClass *classes;          /* const: nclasses structs */
  struct mrb_mt_tbl **mt;          /* RAM: nmt method table slots */
  const struct mrb_rom_iv *iv;     /* const: niv tables, which a store copies to the heap (COW) */
  struct RClass *basic_object, *object, *module, *class_class;
};

/* The graph a program links (the dump's C file); absent, every class is on the heap. */
extern const struct mrb_rom_graph mrb_rom_class_graph __attribute__((weak));

/* Set before mrb_open: the state that opens leaves the graph alone (heap classes). */
MRB_API int mrb_rom_graph_disabled;

/* Whether `mrb` runs on the graph. */
MRB_API mrb_bool mrb_rom_graph_active(mrb_state *mrb);

/* Calls `func` with each ROM class, as mrb_objspace_each_objects does with the heap's objects. */
MRB_API void mrb_rom_graph_each(mrb_state *mrb, mrb_each_object_callback *func, void *data);

/* Writes the class graph `mrb` holds now as C, a line at a time, to `out`. `ram_classes` (NULL-terminated paths, or
   NULL) are the classes whose constants or instance variables change after the boot: their const tables hold only
   their classes and name, the boot stores the rest (a heap copy). */
typedef void mrb_rom_graph_out(void *ud, const char *line);
MRB_API void mrb_rom_graph_dump(mrb_state *mrb, mrb_rom_graph_out *out, void *ud, const char *const *ram_classes);

/* Constant mrb_value initializers for the dump's tables. */
#if defined(MRB_WORD_BOXING)
#define MRB_ROM_VAL(w) { (uintptr_t)(w) }
#define MRB_ROM_FIXNUM(n) MRB_ROM_VAL(((uintptr_t)(intptr_t)(n) << WORDBOX_FIXNUM_SHIFT) | WORDBOX_FIXNUM_FLAG)
#define MRB_ROM_SYMV(s) MRB_ROM_VAL(((uintptr_t)(s) << WORDBOX_SYMBOL_SHIFT) | WORDBOX_SYMBOL_FLAG)
#define MRB_ROM_OBJV(p) MRB_ROM_VAL(p)
#define MRB_ROM_NIL MRB_ROM_VAL(MRB_Qnil)
#define MRB_ROM_FALSE MRB_ROM_VAL(MRB_Qfalse)
#define MRB_ROM_TRUE MRB_ROM_VAL(MRB_Qtrue)
#endif

MRB_END_DECL

#endif  /* MRUBY_ROM_GRAPH_H */
