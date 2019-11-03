#include <mruby.h>
#include <mruby/float.h>
#include <mruby/value.h>


mrb_float
mrb_int_to_float(mrb_state *mrb, mrb_int i)
{
  bf_t f;
  bf_double_t ret;

  bf_init(&mrb->bf_ctx, &f);

  bf_set_si(&f, i);
  bf_get_float64(&f, &ret, BF_RNDN);

  bf_delete(&f);
  mrb_float v = {ret.v};
  return v;
}

mrb_bool
mrb_float_equal(mrb_state *mrb, mrb_float a_, mrb_float b_)
{
  bf_t a, b;
  int ret;
  bf_double_t a_raw = { a_.v };
  bf_double_t b_raw = { b_.v };
  bf_init(&mrb->bf_ctx, &a);
  bf_init(&mrb->bf_ctx, &b);

  bf_set_float64(&a, a_raw);
  bf_set_float64(&b, b_raw);
  ret = bf_cmp_eq(&a, &b);

  bf_delete(&b);
  bf_delete(&a);

  return ret;
}

mrb_bool
mrb_isinf(mrb_state *mrb, mrb_float a_)
{
  bf_t a;
  int ret;
  bf_double_t a_raw = { a_.v };
  bf_init(&mrb->bf_ctx, &a);
  bf_set_float64(&a, a_raw);
  ret = !bf_is_finite(&a);
  bf_delete(&a);
  return ret;
}

mrb_bool
mrb_isnan(mrb_state *mrb, mrb_float a_)
{
  bf_t a;
  int ret;
  bf_double_t a_raw = { a_.v };
  bf_init(&mrb->bf_ctx, &a);
  bf_set_float64(&a, a_raw);
  ret = bf_is_nan(&a);
  bf_delete(&a);
  return ret;
}

mrb_bool
mrb_isneg(mrb_state *mrb, mrb_float a_)
{
  bf_t a;
  int ret;
  bf_double_t a_raw = { a_.v };
  bf_init(&mrb->bf_ctx, &a);
  bf_set_float64(&a, a_raw);
  ret = a.sign;
  bf_delete(&a);
  return ret;
}
