#include <string.h>
#include <mruby.h>
#include <mruby/array.h>
#include <mruby/presym.h>

/* Presyms sharing more than the 63 prefix bytes a compact table records, and
   names next to them that are none. */
#define PRESYM_TEST_LONG "presym_test_aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
static mrb_value
presym_long_names(mrb_state *mrb, mrb_value self)
{
  static const char *const absent[] = {
    PRESYM_TEST_LONG "_0", PRESYM_TEST_LONG "_3", PRESYM_TEST_LONG "_1x", PRESYM_TEST_LONG, PRESYM_TEST_LONG "_",
  };
  static const char *const names[] = {PRESYM_TEST_LONG "_1", PRESYM_TEST_LONG "_10", PRESYM_TEST_LONG "_2"};
  const mrb_sym present[] = {MRB_SYM(presym_test_aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa_1), MRB_SYM(presym_test_aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa_10), MRB_SYM(presym_test_aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa_2)};
  mrb_value res = mrb_ary_new(mrb);

  for (size_t i = 0; i < sizeof(present)/sizeof(present[0]); i++) {
    mrb_ary_push(mrb, res, mrb_bool_value(mrb_intern_check_cstr(mrb, names[i]) == present[i] &&
                                          strcmp(mrb_sym_name(mrb, present[i]), names[i]) == 0));
  }
  for (size_t i = 0; i < sizeof(absent)/sizeof(absent[0]); i++) {
    mrb_ary_push(mrb, res, mrb_bool_value(mrb_intern_check_cstr(mrb, absent[i]) == 0));
  }
  return res;
}

void
mrb_mruby_presym_compact_test_gem_init(mrb_state *mrb)
{
  mrb_define_method(mrb, mrb->kernel_module, "presym_long_names", presym_long_names, MRB_ARGS_NONE());
}

void
mrb_mruby_presym_compact_test_gem_final(mrb_state *mrb)
{
}
