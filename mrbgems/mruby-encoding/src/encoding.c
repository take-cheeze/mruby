/*
 * Poorman's Encoding: UTF-8 and ASCII-8BIT (BINARY) only.
 * An encoding is the frozen name string of the Encoding constant, not an object.
 * A binary string has the MRB_STR_BINARY flag; every other string is UTF-8.
 */
#include <mruby.h>
#include <mruby/string.h>
#include <mruby/variable.h>
#include <mruby/encoding.h>
#include <string.h>
#include <strings.h>

/* a strict UTF-8 check: no overlong forms, no surrogates, nothing above U+10FFFF */
static int
utf8_valid(const unsigned char *p, const unsigned char *e)
{
  while (p < e) {
    unsigned char c = *p;
    int n, lo = 0x80, hi = 0xBF;
    if (c < 0x80) { p++; continue; }
    if (c >= 0xC2 && c <= 0xDF) n = 1;
    else if (c >= 0xE0 && c <= 0xEF) {
      n = 2;
      if (c == 0xE0) lo = 0xA0;
      if (c == 0xED) hi = 0x9F;
    }
    else if (c >= 0xF0 && c <= 0xF4) {
      n = 3;
      if (c == 0xF0) lo = 0x90;
      if (c == 0xF4) hi = 0x8F;
    }
    else return 0;
    if (e - p < n + 1) return 0;
    if (p[1] < lo || p[1] > hi) return 0;
    for (int i = 2; i <= n; i++) {
      if ((p[i] & 0xC0) != 0x80) return 0;
    }
    p += n + 1;
  }
  return 1;
}

static mrb_value
str_valid_enc_p(mrb_state *mrb, mrb_value str)
{
  struct RString *s = mrb_str_ptr(str);
  if (RSTR_BINARY_P(s)) return mrb_true_value();
  return mrb_bool_value(utf8_valid((const unsigned char *)RSTR_PTR(s),
                                   (const unsigned char *)RSTR_PTR(s) + RSTR_LEN(s)));
}

static mrb_value
get_encoding(mrb_state *mrb, const char *name)
{
  struct RClass *e = mrb_module_get(mrb, "Encoding");
  return mrb_const_get(mrb, mrb_obj_value(e), mrb_intern_cstr(mrb, name));
}

static mrb_value
str_encoding(mrb_state *mrb, mrb_value self)
{
  if (RSTR_BINARY_P(mrb_str_ptr(self))) {
    return get_encoding(mrb, "BINARY");
  }
  return get_encoding(mrb, "UTF_8");
}

/* a name equal to lit, ignoring ASCII case */
static int
name_is(mrb_value name, const char *lit)
{
  mrb_int len = (mrb_int)strlen(lit);
  return RSTRING_LEN(name) == len && strncasecmp(RSTRING_PTR(name), lit, (size_t)len) == 0;
}

static mrb_value
str_force_encoding(mrb_state *mrb, mrb_value self)
{
  mrb_value enc;
  struct RString *s;
  mrb_get_args(mrb, "S", &enc);

  s = mrb_str_ptr(self);
  if (name_is(enc, "ASCII-8BIT") || name_is(enc, "BINARY")) {
    RSTR_SET_BINARY_FLAG(s);
  }
  else if (name_is(enc, "UTF-8")) {
    RSTR_UNSET_BINARY_FLAG(s);
  }
  else {
    mrb_raisef(mrb, E_ARGUMENT_ERROR, "unknown encoding name - %v", enc);
  }
  return self;
}

void
mrb_mruby_encoding_gem_init(mrb_state *mrb)
{
  struct RClass *s = mrb->string_class;
  struct RClass *e;
  mrb_value b;

  mrb_define_method(mrb, s, "valid_encoding?", str_valid_enc_p, MRB_ARGS_NONE());
  mrb_define_method(mrb, s, "encoding",        str_encoding,    MRB_ARGS_NONE());
  mrb_define_method(mrb, s, "force_encoding",  str_force_encoding, MRB_ARGS_REQ(1));

  e = mrb_define_module(mrb, "Encoding");
  b = mrb_str_new_lit_frozen(mrb, "ASCII-8BIT");
  mrb_define_const(mrb, e, "ASCII_8BIT", b);
  mrb_define_const(mrb, e, "BINARY", b);
  mrb_define_const(mrb, e, "UTF_8", mrb_str_new_lit_frozen(mrb, "UTF-8"));
}

void
mrb_mruby_encoding_gem_final(mrb_state *mrb)
{
}

/* ---- the C side: rb_encoding descriptors (see include/mruby/encoding.h) ---- */

static rb_encoding enc_utf8 = { "UTF-8", 0 };
static rb_encoding enc_binary = { "ASCII-8BIT", 1 };

rb_encoding *
rb_utf8_encoding(void)
{
  return &enc_utf8;
}

rb_encoding *
rb_ascii8bit_encoding(void)
{
  return &enc_binary;
}

rb_encoding *
rb_default_external_encoding(void)
{
  return &enc_utf8;
}

rb_encoding *
rb_default_internal_encoding(void)
{
  return NULL;
}

rb_encoding *
rb_enc_get(mrb_value str)
{
  return RSTR_BINARY_P(mrb_str_ptr(str)) ? &enc_binary : &enc_utf8;
}

mrb_value
rb_enc_associate(mrb_value str, rb_encoding *enc)
{
  struct RString *s = mrb_str_ptr(str);
  if (enc && enc->binary) {
    RSTR_SET_BINARY_FLAG(s);
  }
  else {
    RSTR_UNSET_BINARY_FLAG(s);
  }
  return str;
}

mrb_value
rb_str_export_to_enc(mrb_value str, rb_encoding *enc)
{
  (void)enc;
  return str;
}
