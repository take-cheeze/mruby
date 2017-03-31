#include <mruby.h>
#include <mruby/variable.h>

mrb_value mrbgem_require_gems(mrb_state *mrb);

void
mrb_mruby_mrbgem_require_gem_init(mrb_state *mrb)
{
  mrb_define_global_const(mrb, "MRBGEMS", mrbgem_require_gems(mrb));
  mrb_gv_set(mrb, mrb_intern_lit(mrb, "$:"), mrb_gv_get(mrb, mrb_intern_lit(mrb, "$LOAD_PATH")));
}

void
mrb_mruby_mrbgem_require_gem_final(mrb_state *mrb)
{
}
