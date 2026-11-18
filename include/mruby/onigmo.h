/*
** mruby/onigmo.h - CRuby's Onigmo API (ruby/re.h, onigmo.h) for the converted extensions, on the system oniguruma
** library. Oniguruma's names and types are Onigmo's (regex_t, OnigRegion, onig_search, onig_region_*), so they are
** used as they are. The fork's Regexp is the mruby-onig-regexp gem, whose objects hold the compiled regex as their
** data pointer: RREGEXP_PTR reads it. Needs the oniguruma library (the onig gem links it).
*/
#ifndef MRUBY_ONIGMO_H
#define MRUBY_ONIGMO_H

#include <mruby.h>
#include <mruby/data.h>
#include <mruby/re.h>
#include <oniguruma.h>

#ifdef __cplusplus
extern "C" {
#endif

/* CRuby's names for the match region and the character type (the library's are OnigRegion and OnigUChar) */
typedef struct re_registers re_registers;
typedef unsigned char UChar;

/* CRuby's OnigPosition is the offset type of a match region; the system library's region holds ints */
typedef int OnigPosition;

/* whether a value is a Regexp object of the mruby-onig-regexp gem (CRuby's RB_TYPE_P(v, T_REGEXP)) */
MRB_API int mruby_onig_regexp_is(mrb_value v);

/* the compiled regex of a Regexp object (the onig gem's data pointer) */
#define RREGEXP_PTR(r) (*(regex_t **)&(DATA_PTR(r)))

/* CRuby's struct RRegexp, for its usecnt (the count of matches using the regex). The fork's Regexp keeps no such count:
   rb_reg_prepare_re returns the object's own regex and never recompiles, so the count is only incremented and decremented
   in balance (the scratch record below), and no temporary regex is freed. */
struct nfcx_rregexp { unsigned long usecnt; };
static inline struct nfcx_rregexp *
nfcx_rregexp_of(mrb_value r)
{
  static struct nfcx_rregexp scratch;
  (void)r;
  return &scratch;
}
#define RREGEXP(r) nfcx_rregexp_of(r)

/* CRuby's rb_reg_region_copy: the region copy strscan takes */
static inline int
rb_reg_region_copy(struct re_registers *to, const struct re_registers *from)
{
  onig_region_copy(to, (struct re_registers *)from);
  return 0;
}

/* CRuby's rb_reg_prepare_re: the compiled regex of a Regexp (the string is not needed: no per-string recompile) */
static inline regex_t *
rb_reg_prepare_re(mrb_value re, mrb_value str)
{
  (void)str;
  return RREGEXP_PTR(re);
}

#ifdef __cplusplus
}
#endif

#endif
