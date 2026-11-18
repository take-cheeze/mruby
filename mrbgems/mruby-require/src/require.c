/*
 * require for the features the build provides (see include/mruby/require.h). The loaded features are the array
 * $LOADED_FEATURES; a feature is found there by its name.
 */
#include <mruby.h>
#include <mruby/array.h>
#include <mruby/class.h>
#include <mruby/string.h>
#include <mruby/variable.h>
#include <mruby/require.h>
#include <string.h>

static mrb_value
loaded_features(mrb_state *mrb)
{
  mrb_sym sym = mrb_intern_lit(mrb, "$LOADED_FEATURES");
  mrb_value list = mrb_gv_get(mrb, sym);
  if (mrb_type(list) != MRB_TT_ARRAY) {
    list = mrb_ary_new(mrb);
    mrb_gv_set(mrb, sym, list);
  }
  return list;
}

static int
is_loaded(mrb_state *mrb, mrb_value list, const char *name)
{
  mrb_int i, n = RARRAY_LEN(list);
  size_t len = strlen(name);
  for (i = 0; i < n; i++) {
    mrb_value e = mrb_ary_ref(mrb, list, i);
    if (mrb_string_p(e) && (size_t)RSTRING_LEN(e) == len && memcmp(RSTRING_PTR(e), name, len) == 0) {
      return 1;
    }
  }
  return 0;
}

MRB_API void
mrb_require_provide(mrb_state *mrb, const char *name)
{
  mrb_value list = loaded_features(mrb);
  if (!is_loaded(mrb, list, name)) {
    mrb_ary_push(mrb, list, mrb_str_new_cstr(mrb, name));
  }
}

MRB_API mrb_value
rb_require(mrb_state *mrb, const char *name)
{
  if (is_loaded(mrb, loaded_features(mrb), name)) {
    return mrb_false_value();
  }
  mrb_raisef(mrb, mrb_class_get(mrb, "LoadError"), "cannot load such file -- %s", name);
  return mrb_nil_value();
}

static mrb_value
k_require(mrb_state *mrb, mrb_value self)
{
  mrb_value name;
  mrb_get_args(mrb, "S", &name);
  return rb_require(mrb, mrb_string_cstr(mrb, name));
}

void
mrb_mruby_require_gem_init(mrb_state *mrb)
{
  struct RClass *load_error = mrb_define_class(mrb, "LoadError", mrb_class_get(mrb, "ScriptError"));
  (void)load_error;
  mrb_define_method(mrb, mrb->kernel_module, "require", k_require, MRB_ARGS_REQ(1));
}

void
mrb_mruby_require_gem_final(mrb_state *mrb)
{
}
