#include <mruby.h>
#include <mruby/proc.h>
#include <mruby/class.h>
#include <mruby/array.h>
#include <mruby/internal.h>

static mrb_value
return_func_name(mrb_state *mrb, mrb_value self)
{
  return mrb_cfunc_env_get(mrb, 0);
}

static mrb_value
proc_new_cfunc_with_env(mrb_state *mrb, mrb_value self)
{
  mrb_sym n;
  mrb_value n_val;
  mrb_method_t m;
  struct RProc *p;
  mrb_get_args(mrb, "n", &n);
  n_val = mrb_symbol_value(n);
  p = mrb_proc_new_cfunc_with_env(mrb, return_func_name, 1, &n_val);
  MRB_METHOD_FROM_PROC(m, p);
  mrb_define_method_raw(mrb, mrb_class_ptr(self), n, m);
  return self;
}

static mrb_value
return_env(mrb_state *mrb, mrb_value self)
{
  mrb_int idx;
  mrb_get_args(mrb, "i", &idx);
  return mrb_cfunc_env_get(mrb, idx);
}

static mrb_value
cfunc_env_get(mrb_state *mrb, mrb_value self)
{
  mrb_sym n;
  const mrb_value *argv;
  mrb_int argc;
  mrb_get_args(mrb, "na", &n, &argv, &argc);
  struct RProc *p = mrb_proc_new_cfunc_with_env(mrb, return_env, argc, argv);
  mrb_method_t m;
  MRB_METHOD_FROM_PROC(m, p);
  mrb_define_method_raw(mrb, mrb_class_ptr(self), n, m);
  return self;
}

static mrb_value
cfunc_without_env(mrb_state *mrb, mrb_value self)
{
  return mrb_cfunc_env_get(mrb, 0);
}

/* what a compiled block sees: self and whether the frame's target class is the proc's own */
static mrb_value
self_and_tc(mrb_state *mrb, mrb_value self)
{
  const struct RProc *p = mrb->c->ci->proc;
  mrb_value r[2];
  r[0] = self;
  r[1] = mrb_bool_value(mrb_vm_ci_target_class(mrb->c->ci) == MRB_PROC_TARGET_CLASS(p));
  return mrb_ary_new_from_values(mrb, 2, r);
}

/* a cfunc proc whose env slot 0 is a non-proc object, like a compiled block */
static mrb_value
self_proc(mrb_state *mrb, mrb_value self)
{
  mrb_value creator = mrb_get_arg1(mrb);
  return mrb_obj_value(mrb_proc_new_cfunc_with_env(mrb, self_and_tc, 1, &creator));
}

void mrb_mruby_proc_ext_gem_test(mrb_state *mrb)
{
  struct RClass *cls;

  cls = mrb_define_class(mrb, "ProcExtTest", mrb->object_class);
  mrb_define_class_method(mrb, cls, "mrb_proc_new_cfunc_with_env", proc_new_cfunc_with_env, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, cls, "mrb_cfunc_env_get", cfunc_env_get, MRB_ARGS_REQ(2));
  mrb_define_class_method(mrb, cls, "self_proc", self_proc, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, cls, "cfunc_without_env", cfunc_without_env, MRB_ARGS_NONE());
}
