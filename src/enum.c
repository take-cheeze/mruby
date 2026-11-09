/*
** enum.c - Enumerable module
**
** See Copyright Notice in mruby.h
*/

#include <mruby.h>
#include <mruby/class.h>
#include <mruby/proc.h>

/* internal method `__update_hash(oldhash, index, itemhash)` */
static mrb_value
enum_update_hash(mrb_state *mrb, mrb_value self)
{
  mrb_int hash;
  mrb_int index;
  mrb_int hv;

  mrb_get_args(mrb, "iii", &hash, &index, &hv);
  hash ^= ((uint32_t)hv << (index % 16));

  return mrb_int_value(mrb, hash);
}

/* module_function: a public singleton method and a private instance method */
#define MRB_MT_CLASS Enumerable_singleton
static const mrb_mt_entry enum_s_rom_entries[] = {
  MRB_MT_ENTRY(enum_update_hash, MRB_SYM(__update_hash), MRB_ARGS_REQ(3))
};
#undef MRB_MT_CLASS
#define MRB_MT_CLASS Enumerable
static const mrb_mt_entry enum_rom_entries[] = {
  MRB_MT_ENTRY(enum_update_hash, MRB_SYM(__update_hash), MRB_ARGS_REQ(3) | MRB_MT_PRIVATE)
};
#undef MRB_MT_CLASS

void
mrb_init_enumerable(mrb_state *mrb)
{
  struct RClass *enumerable = mrb_define_module_id(mrb, MRB_SYM(Enumerable));  /* 15.3.2 */
  MRB_MT_INIT_ROM(mrb, mrb_singleton_class_ptr(mrb, mrb_obj_value(enumerable)), enum_s_rom_entries);
  MRB_MT_INIT_ROM(mrb, enumerable, enum_rom_entries);
}
