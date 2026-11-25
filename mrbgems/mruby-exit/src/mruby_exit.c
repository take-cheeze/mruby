#include <stdlib.h>
#include <mruby.h>
#include <mruby/class.h>
#include <mruby/error.h>
#include <mruby/variable.h>
#include <mruby/array.h>
#include <mruby/proc.h>

#ifndef EXIT_SUCCESS
# define EXIT_SUCCESS 0
#endif

#ifndef EXIT_FAILURE
# define EXIT_FAILURE 1
#endif

static int
get_status(mrb_state *mrb)
{
  mrb_value status = mrb_true_value();

  mrb_get_args(mrb, "|o", &status);
  if (mrb_true_p(status)) return EXIT_SUCCESS;
  if (mrb_false_p(status)) return EXIT_FAILURE;
  mrb_int n = mrb_as_int(mrb, status);
#if MRB_INT_MAX > INT_MAX
  if (n < INT_MIN || n > INT_MAX) {
    mrb_raisef(mrb, E_RANGE_ERROR, "exit status out of range: %i", n);
  }
#endif
  return (int)n;
}

/*
 *  call-seq:
 *     exit(status=true)
 *
 *  Initiates the termination of the Ruby script by raising the
 *  SystemExit exception. This exception may be caught. The
 *  optional parameter is used to return a status code to the invoking
 *  environment.
 *
 *  `true` and `false` of _status_ means success and failure
 *  respectively.  The interpretation of other integer values are
 *  system dependent.
 *
 *     exit(0)
 */
static mrb_value
f_exit(mrb_state *mrb, mrb_value self)
{
  int status = get_status(mrb);
  mrb_value exc = mrb_obj_new(mrb, mrb_exc_get_id(mrb, MRB_SYM(SystemExit)), 0, NULL);
  struct RException *e = mrb_exc_ptr(exc);
  e->flags |= MRB_EXC_EXIT;
  mrb_iv_set(mrb, exc, MRB_IVSYM(status), mrb_int_value(mrb, (mrb_int)status));
  mrb_exc_raise(mrb, exc);
  /* not reached */
  return mrb_nil_value();
}

/*
 *  call-seq:
 *     exit!(status=true)
 *
 *  Exits the process immediately. No exit handlers are run.
 *  <em>status</em> is returned to the underlying system as the
 *  exit status.
 *
 *     exit!(0)
 */
static mrb_value
f_exit_bang(mrb_state *mrb, mrb_value self)
{
  exit(get_status(mrb));
  /* not reached */
  return mrb_nil_value();
}

/* at_exit: the blocks are kept in a hidden Kernel ivar (no '@', so it is not
   listed by instance_variables) and run in reverse order by mrb_close, through
   mrb_state_atexit. A block that raises does not stop the others. */
static mrb_value
f_at_exit(mrb_state *mrb, mrb_value self)
{
  mrb_value blk, list;
  mrb_sym key = mrb_intern_lit(mrb, "__at_exit__");
  mrb_value kernel = mrb_obj_value(mrb->kernel_module);
  mrb_get_args(mrb, "&", &blk);
  if (mrb_nil_p(blk)) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "called without a block");
  }
  list = mrb_iv_get(mrb, kernel, key);
  if (!mrb_array_p(list)) {
    list = mrb_ary_new(mrb);
    mrb_iv_set(mrb, kernel, key, list);
  }
  mrb_ary_push(mrb, list, blk);
  return blk;
}

static mrb_value
run_one_at_exit(mrb_state *mrb, void *userdata)
{
  mrb_value *p = (mrb_value *)userdata;
  return mrb_funcall_with_block(mrb, *p, MRB_SYM(call), 0, NULL, mrb_nil_value());
}

static void
run_at_exit(mrb_state *mrb)
{
  mrb_sym key = mrb_intern_lit(mrb, "__at_exit__");
  mrb_value list = mrb_iv_get(mrb, mrb_obj_value(mrb->kernel_module), key);
  while (mrb_array_p(list) && RARRAY_LEN(list) > 0) {
    mrb_bool err = FALSE;
    mrb_value p = mrb_ary_pop(mrb, list);
    mrb_value r = mrb_protect_error(mrb, run_one_at_exit, &p, &err);
    if (err && mrb_obj_ptr(r) && mrb_type(r) == MRB_TT_EXCEPTION) {
      /* CRuby reports the error and carries on with the other hooks */
      mrb->exc = mrb_obj_ptr(r);
      mrb_print_error(mrb);
      mrb->exc = NULL;
    }
    mrb_gc_arena_restore(mrb, 0);
  }
}

#define MRB_MT_CLASS Kernel
static const mrb_mt_entry exit_kernel_rom_entries[] = {
  MRB_MT_ENTRY(f_exit,      MRB_SYM(exit),   MRB_ARGS_OPT(1) | MRB_MT_PRIVATE)
  MRB_MT_ENTRY(f_exit_bang, MRB_SYM_B(exit), MRB_ARGS_OPT(1) | MRB_MT_PRIVATE)
  MRB_MT_ENTRY(f_at_exit,   MRB_SYM(at_exit), MRB_ARGS_BLOCK() | MRB_MT_PRIVATE)
};
#undef MRB_MT_CLASS

void
mrb_mruby_exit_gem_init(mrb_state* mrb)
{
  mrb_define_class_id(mrb, MRB_SYM(SystemExit), E_EXCEPTION);
  MRB_MT_INIT_ROM(mrb, mrb->kernel_module, exit_kernel_rom_entries);
  mrb_state_atexit(mrb, run_at_exit);
}

void
mrb_mruby_exit_gem_final(mrb_state* mrb)
{
}
