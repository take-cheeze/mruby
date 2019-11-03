/**
** @file mruby/float.h - Float utilities
**
** See Copyright Notice in mruby.h
*/

#ifndef MRUBY_FLOAT_H
#define MRUBY_FLOAT_H

#include "common.h"

MRB_BEGIN_DECL

#ifdef MRB_BF_FLOAT

mrb_float mrb_int_to_float(mrb_state *mrb, mrb_int i);
mrb_int mrb_float_to_int(mrb_state *mrb, mrb_float f);
mrb_bool mrb_float_equal(mrb_state *mrb, mrb_float a, mrb_float b);

mrb_bool mrb_isinf(mrb_state *mrb, mrb_float a);
mrb_bool mrb_isnan(mrb_state *mrb, mrb_float a);
mrb_bool mrb_isneg(mrb_state *mrb, mrb_float a);

mrb_float mrb_infinity(mrb_state* mrb);
mrb_float mrb_float_neg(mrb_state* mrb, mrb_float v);
#else

#endif

MRB_END_DECL

#endif /* MRUBY_FLOAT_H */
