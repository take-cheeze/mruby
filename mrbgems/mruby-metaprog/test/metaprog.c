#include <mruby.h>
#include <mruby/class.h>
#include <mruby/proc.h>
#include <mruby/array.h>
#include <mruby/variable.h>

/* the visibility a fresh mrb_state sees for a ROM method: the tables are shared const data */
static mrb_value
vis_sym(mrb_state *mrb, mrb_method_t m)
{
  if (MRB_METHOD_UNDEF_P(m)) return mrb_symbol_value(mrb_intern_lit(mrb, "undefined"));
  switch (MRB_METHOD_VISIBILITY(m)) {
  case MRB_METHOD_PRIVATE_FL: return mrb_symbol_value(mrb_intern_lit(mrb, "private"));
  case MRB_METHOD_PROTECTED_FL: return mrb_symbol_value(mrb_intern_lit(mrb, "protected"));
  default: return mrb_symbol_value(mrb_intern_lit(mrb, "public"));
  }
}

static mrb_value
fresh_state_visibility(mrb_state *mrb, mrb_value self)
{
  mrb_state *other = mrb_open_core();
  if (!other) return mrb_nil_value();
  struct RClass *c = other->string_class;
  mrb_method_t m1 = mrb_method_search_vm(other, &c, mrb_intern_lit(other, "upcase"));
  struct RClass *gc = mrb_singleton_class_ptr(other, mrb_obj_value(mrb_module_get(other, "GC")));
  mrb_method_t m2 = mrb_method_search_vm(other, &gc, mrb_intern_lit(other, "step_ratio"));
  mrb_close(other);
  mrb_value pair[2] = { vis_sym(mrb, m1), vis_sym(mrb, m2) };
  return mrb_ary_new_from_values(mrb, 2, pair);
}

void
mrb_mruby_metaprog_gem_test(mrb_state *mrb)
{
  struct RClass *t = mrb_define_module(mrb, "MetaprogTest");
  mrb_define_module_function(mrb, t, "fresh_state_visibility", fresh_state_visibility, MRB_ARGS_NONE());
}
