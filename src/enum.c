/*
** enum.c - Enumerable module
**
** See Copyright Notice in mruby.h
*/

#include <mruby.h>
#include <mruby/array.h>
#include <mruby/class.h>
#include <mruby/proc.h>

/*
 * Enumerable#inject / reduce, native: no lambda and no VM entry. The block
 * given to `each` is a cfunc proc whose env is one state array, indexed below.
 * The first element becomes the result when there is no initial value; each
 * later element is sent to the result as `result.__send__(sym, val)`, or passed
 * to the block as `block.call(result, val)`. A multi-value yield is one value,
 * as `|*val| val.__svalue` gave it.
 */
#define ENUM_INJECT_RESULT 0
#define ENUM_INJECT_FIRST  1
#define ENUM_INJECT_SYM    2
#define ENUM_INJECT_BLOCK  3

static mrb_value
enum_inject_each(mrb_state *mrb, mrb_value self)
{
  mrb_value state = mrb_proc_cfunc_env_get(mrb, 0);
  mrb_value *argv;
  mrb_int argc;
  mrb_value val, result, sym, blk, args[2];

  mrb_get_args(mrb, "*", &argv, &argc);
  switch (argc) {  /* __svalue */
  case 0:  val = mrb_nil_value(); break;
  case 1:  val = argv[0]; break;
  default: val = mrb_ary_new_from_values(mrb, argc, argv); break;
  }

  if (mrb_test(mrb_ary_ref(mrb, state, ENUM_INJECT_FIRST))) {
    mrb_ary_set(mrb, state, ENUM_INJECT_FIRST, mrb_false_value());
    mrb_ary_set(mrb, state, ENUM_INJECT_RESULT, val);
    return mrb_nil_value();
  }

  result = mrb_ary_ref(mrb, state, ENUM_INJECT_RESULT);
  sym = mrb_ary_ref(mrb, state, ENUM_INJECT_SYM);
  if (!mrb_nil_p(sym)) {
    result = mrb_funcall_argv(mrb, result, mrb_symbol(sym), 1, &val);
  }
  else {
    blk = mrb_ary_ref(mrb, state, ENUM_INJECT_BLOCK);
    args[0] = result;
    args[1] = val;
    if (mrb_nil_p(blk)) {  /* no block: the same NoMethodError as nil.call */
      result = mrb_funcall_argv(mrb, blk, MRB_SYM(call), 2, args);
    }
    else {
      result = mrb_yield_argv(mrb, blk, 2, args);
    }
  }
  mrb_ary_set(mrb, state, ENUM_INJECT_RESULT, result);
  return mrb_nil_value();
}

static mrb_value
enum_inject(mrb_state *mrb, mrb_value self)
{
  mrb_value *argv;
  mrb_int argc;
  mrb_value blk, sym = mrb_nil_value(), init[4], state;
  struct RProc *p;

  mrb_get_args(mrb, "*&", &argv, &argc, &blk);
  if (argc > 2) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "too many arguments");
  }
  if (argc > 0 && mrb_symbol_p(argv[argc - 1])) {
    sym = argv[--argc];
  }

  init[ENUM_INJECT_FIRST] = mrb_bool_value(argc == 0);  /* no initial argument */
  init[ENUM_INJECT_RESULT] = argc == 0 ? mrb_nil_value() : argv[0];
  init[ENUM_INJECT_SYM] = sym;
  init[ENUM_INJECT_BLOCK] = blk;
  state = mrb_ary_new_from_values(mrb, 4, init);

  p = mrb_proc_new_cfunc_with_env(mrb, enum_inject_each, 1, &state);
  mrb_funcall_with_block(mrb, self, MRB_SYM(each), 0, NULL, mrb_obj_value(p));
  return mrb_ary_ref(mrb, state, ENUM_INJECT_RESULT);
}

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
  MRB_MT_ENTRY(enum_inject,              MRB_SYM(inject),          MRB_ARGS_ANY())
  MRB_MT_ENTRY(enum_inject,              MRB_SYM(reduce),          MRB_ARGS_ANY())
};
#undef MRB_MT_CLASS

void
mrb_init_enumerable(mrb_state *mrb)
{
  struct RClass *enumerable = mrb_define_module_id(mrb, MRB_SYM(Enumerable));  /* 15.3.2 */
  mrb_define_module_function_id(mrb, enumerable, MRB_SYM(__update_hash), enum_update_hash, MRB_ARGS_REQ(3));
  MRB_MT_INIT_ROM(mrb, enumerable, enum_rom_entries);
}
