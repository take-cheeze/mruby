#include <string.h>
#include <mruby.h>
#include <mruby/array.h>
#include <mruby/presym.h>

/* The presyms whose name does not intern back to them, or whose name pointer
   changes between calls (callers keep it; see presym_sym2name). */
static mrb_value
presym_round_trip_failures(mrb_state *mrb, mrb_value self)
{
  mrb_value bad = mrb_ary_new(mrb);
  const char **first = (const char**)mrb_malloc(mrb, sizeof(*first) * (MRB_PRESYM_MAX+1));

  for (mrb_sym sym = 1; sym <= MRB_PRESYM_MAX; sym++) {
    mrb_int len;
    const char *name = mrb_sym_name_len(mrb, sym, &len);
    first[sym] = name;
    if (!name || strlen(name) != (size_t)len || mrb_intern_check(mrb, name, len) != sym) {
      mrb_ary_push(mrb, bad, mrb_fixnum_value(sym));
    }
  }
  for (mrb_sym sym = MRB_PRESYM_MAX; sym >= 1; sym--) {
    mrb_int len;
    if (mrb_sym_name_len(mrb, sym, &len) != first[sym]) mrb_ary_push(mrb, bad, mrb_fixnum_value(sym));
  }
  mrb_free(mrb, first);
  return bad;
}

void
mrb_mruby_symbol_ext_gem_test(mrb_state *mrb)
{
  mrb_define_method(mrb, mrb->kernel_module, "presym_round_trip_failures", presym_round_trip_failures, MRB_ARGS_NONE());
}
