/*
 * The upstream core helpers that mruby-bigint uses, for this build. Each is the local equivalent of an upstream name.
 *   mrb_as_int(mrb, v)       the Integer of a value (to_int), as upstream's mrb_as_int
 *   mrb_integer_p / mrb_integer  the fixnum tag (MRB_TT_INTEGER is MRB_TT_FIXNUM here)
 *   mrb_int_value           a fixnum; a value that does not fit one is a bigint (phase 2: mrb_bint_new_int)
 *   mrb_int_zerodiv         raises ZeroDivisionError
 *   mrb_byte_hash(_step)    FNV-1a over bytes (upstream's hash; the value differs from upstream's)
 */
#ifndef NFCX_BIGINT_COMPAT_H
#define NFCX_BIGINT_COMPAT_H

#include <math.h>
#include <stdint.h>
#include <string.h>

#define mrb_as_int(mrb, v)       mrb_fixnum(mrb_to_int((mrb), (v)))
#define mrb_integer_p(o)         mrb_fixnum_p(o)
#define mrb_integer(o)           mrb_fixnum(o)
#define mrb_int_value(mrb, i)    mrb_fixnum_value((mrb_int)(i))

static inline void
mrb_int_zerodiv(mrb_state *mrb)
{
  mrb_raise(mrb, E_ZERODIV_ERROR, "divided by 0");
}

static inline uint32_t
mrb_byte_hash_step(const uint8_t *p, size_t n, uint32_t h)
{
  while (n--) {
    h ^= *p++;
    h *= 16777619u;
  }
  return h;
}

static inline uint32_t
mrb_byte_hash(const uint8_t *p, size_t n)
{
  return mrb_byte_hash_step(p, n, 2166136261u);
}

#endif
