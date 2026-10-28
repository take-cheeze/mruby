/*
** enum.c - Enumerable module
**
** See Copyright Notice in mruby.h
*/

#include <mruby.h>
#include <mruby/array.h>
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

/*
 * Block used by Enumerable#entries: appends one yielded value to the
 * array held in the block's environment. Matches `|*val|` + `val.__svalue`:
 * no values -> nil, one value -> that value, several values -> an array.
 */
static mrb_value
enum_entries_i(mrb_state *mrb, mrb_value self)
{
  mrb_value *argv;
  mrb_int argc;
  mrb_value ary;

  mrb_get_args(mrb, "*", &argv, &argc);
  ary = mrb_proc_cfunc_env_get(mrb, 0);
  switch (argc) {
  case 0:
    mrb_ary_push(mrb, ary, mrb_nil_value());
    break;
  case 1:
    mrb_ary_push(mrb, ary, argv[0]);
    break;
  default:
    mrb_ary_push(mrb, ary, mrb_ary_new_from_values(mrb, argc, argv));
    break;
  }
  return mrb_nil_value();
}

/* Enumerable#entries / Enumerable#to_a: call self.each and collect the yielded values */
static mrb_value
enum_entries(mrb_state *mrb, mrb_value self)
{
  mrb_value ary = mrb_ary_new(mrb);
  struct RProc *blk = mrb_proc_new_cfunc_with_env(mrb, enum_entries_i, 1, &ary);

  mrb_funcall_with_block(mrb, self, MRB_SYM(each), 0, NULL, mrb_obj_value(blk));
  return ary;
}

#define MRB_MT_CLASS Enumerable
static const mrb_mt_entry enum_rom_entries[] = {
  MRB_MT_ENTRY(enum_entries,              MRB_SYM(entries),         MRB_ARGS_NONE())   /* ISO 15.3.2.2.6 */
  MRB_MT_ENTRY(enum_entries,              MRB_SYM(to_a),            MRB_ARGS_NONE())   /* ISO 15.3.2.2.20 */
};
#undef MRB_MT_CLASS

void
mrb_init_enumerable(mrb_state *mrb)
{
  struct RClass *enumerable = mrb_define_module_id(mrb, MRB_SYM(Enumerable));  /* 15.3.2 */
  mrb_define_module_function_id(mrb, enumerable, MRB_SYM(__update_hash), enum_update_hash, MRB_ARGS_REQ(3));

  MRB_MT_INIT_ROM(mrb, enumerable, enum_rom_entries);
}
