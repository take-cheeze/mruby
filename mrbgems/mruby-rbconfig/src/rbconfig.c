#include <mruby.h>

mrb_value mrb_rbconfig_hash(mrb_state *mrb);
void mrb_rbconfig_define_const(mrb_state *mrb);

void
mrb_mruby_rbconfig_gem_init(mrb_state *mrb)
{
  struct RClass* rbconfig = mrb_define_module(mrb, "RbConfig");
  mrb_define_const(mrb, rbconfig, "CONFIG", mrb_rbconfig_hash(mrb));

  mrb_rbconfig_define_const(mrb);
}

void
mrb_mruby_rbconfig_gem_final(mrb_state *mrb)
{
}
