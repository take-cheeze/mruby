/*
 * Compatibility for this mruby fork, which has no presym table (mruby/presym.h), no MRB_IVSYM/MRB_OPSYM,
 * no mrb_funcall_id and no mrb_int_value. Symbols are interned where they are used.
 */
#ifndef MRUBY_ONIG_REGEXP_COMPAT_H
#define MRUBY_ONIG_REGEXP_COMPAT_H
#include <mruby.h>
#define MRB_SYM(x) mrb_intern_lit(mrb, #x)
#define MRB_IVSYM(x) mrb_intern_lit(mrb, "@" #x)
#define MRB_OPSYM(x) mrb_intern_lit(mrb, mruby_onig_regexp_op_##x)
#define mruby_onig_regexp_op_aref "[]"
#define mrb_funcall_id(mrb, obj, id, argc, ...) \
  mrb_funcall_argv((mrb), (obj), (id), (argc), ((mrb_value[]){mrb_nil_value(), ##__VA_ARGS__}) + 1)
#define mrb_int_value(m, i) mrb_fixnum_value(i)
#endif
