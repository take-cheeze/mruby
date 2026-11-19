/*
 * Poorman's Encoding: ASCII-8BIT (BINARY), UTF-8 and US-ASCII; no transcoding.
 * An encoding is the frozen name string of the Encoding constant, not an object.
 * A string carries its encoding in two flags: MRB_STR_BINARY (ASCII-8BIT) and MRB_STR_USASCII
 * (US-ASCII); a string with neither flag is UTF-8.
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

static int
ascii_only(const unsigned char *p, const unsigned char *e)
{
  for (; p < e; p++) {
    if (*p >= 0x80) return 0;
  }
  return 1;
}

static mrb_value
str_valid_enc_p(mrb_state *mrb, mrb_value str)
{
  struct RString *s = mrb_str_ptr(str);
  const unsigned char *p = (const unsigned char *)RSTR_PTR(s);
  const unsigned char *e = p + RSTR_LEN(s);
  if (RSTR_BINARY_P(s)) return mrb_true_value();
  if (RSTR_USASCII_P(s)) return mrb_bool_value(ascii_only(p, e));
  return mrb_bool_value(utf8_valid(p, e));
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
  struct RString *s = mrb_str_ptr(self);
  if (RSTR_BINARY_P(s)) return get_encoding(mrb, "BINARY");
  if (RSTR_USASCII_P(s)) return get_encoding(mrb, "US_ASCII");
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
    RSTR_UNSET_USASCII_FLAG(s);
    RSTR_SET_BINARY_FLAG(s);
  }
  else if (name_is(enc, "UTF-8")) {
    RSTR_UNSET_USASCII_FLAG(s);
    RSTR_UNSET_BINARY_FLAG(s);
  }
  else if (name_is(enc, "US-ASCII") || name_is(enc, "ASCII")) {
    RSTR_UNSET_BINARY_FLAG(s);
    RSTR_SET_USASCII_FLAG(s);
  }
  else {
    mrb_raisef(mrb, E_ARGUMENT_ERROR, "unknown encoding name - %v", enc);
  }
  return self;
}

/* ---- the C side: rb_encoding descriptors (see include/mruby/encoding.h) ---- */

static rb_encoding enc_binary = { "ASCII-8BIT", 1, ENCINDEX_ASCII_8BIT };
static rb_encoding enc_utf8 = { "UTF-8", 0, ENCINDEX_UTF_8 };
static rb_encoding enc_usascii = { "US-ASCII", 0, ENCINDEX_US_ASCII };

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
rb_usascii_encoding(void)
{
  return &enc_usascii;
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
rb_enc_from_index(int index)
{
  switch (index) {
    case ENCINDEX_ASCII_8BIT: return &enc_binary;
    case ENCINDEX_UTF_8:      return &enc_utf8;
    case ENCINDEX_US_ASCII:   return &enc_usascii;
    default:                  return NULL;
  }
}

int
rb_enc_find_index(const char *name)
{
  if (strcasecmp(name, "ASCII-8BIT") == 0 || strcasecmp(name, "BINARY") == 0) return ENCINDEX_ASCII_8BIT;
  if (strcasecmp(name, "UTF-8") == 0) return ENCINDEX_UTF_8;
  if (strcasecmp(name, "US-ASCII") == 0 || strcasecmp(name, "ASCII") == 0) return ENCINDEX_US_ASCII;
  return -1;
}

int
rb_enc_to_index(rb_encoding *enc)
{
  return enc ? enc->index : ENCINDEX_ASCII_8BIT;
}

int rb_utf8_encindex(void) { return ENCINDEX_UTF_8; }
int rb_ascii8bit_encindex(void) { return ENCINDEX_ASCII_8BIT; }
int rb_usascii_encindex(void) { return ENCINDEX_US_ASCII; }

int
rb_enc_get_index(mrb_value str)
{
  struct RString *s = mrb_str_ptr(str);
  if (RSTR_BINARY_P(s)) return ENCINDEX_ASCII_8BIT;
  if (RSTR_USASCII_P(s)) return ENCINDEX_US_ASCII;
  return ENCINDEX_UTF_8;
}

rb_encoding *
rb_enc_get(mrb_value str)
{
  return rb_enc_from_index(rb_enc_get_index(str));
}

void
rb_enc_set_index(mrb_value str, int index)
{
  struct RString *s = mrb_str_ptr(str);
  switch (index) {
    case ENCINDEX_ASCII_8BIT:
      RSTR_UNSET_USASCII_FLAG(s);
      RSTR_SET_BINARY_FLAG(s);
      break;
    case ENCINDEX_UTF_8:
      RSTR_UNSET_USASCII_FLAG(s);
      RSTR_UNSET_BINARY_FLAG(s);
      break;
    case ENCINDEX_US_ASCII:
      RSTR_UNSET_BINARY_FLAG(s);
      RSTR_SET_USASCII_FLAG(s);
      break;
    default:
      break;
  }
}

mrb_value
rb_enc_associate_index(mrb_value str, int index)
{
  rb_enc_set_index(str, index);
  return str;
}

mrb_value
rb_enc_associate(mrb_value str, rb_encoding *enc)
{
  rb_enc_set_index(str, enc ? enc->index : ENCINDEX_UTF_8);
  return str;
}

mrb_value
rb_str_export_to_enc(mrb_value str, rb_encoding *enc)
{
  (void)enc;
  return str;
}

/* the length in bytes of the UTF-8 sequence that starts with c (a malformed lead byte counts one) */
static int
utf8_seq_len(unsigned char c)
{
  if (c < 0x80) return 1;
  if (c >= 0xC2 && c <= 0xDF) return 2;
  if (c >= 0xE0 && c <= 0xEF) return 3;
  if (c >= 0xF0 && c <= 0xF4) return 4;
  return 1;
}

int
rb_enc_mbclen(const char *p, const char *e, rb_encoding *enc)
{
  const unsigned char *u = (const unsigned char *)p;
  int n;
  if (p >= e) return 0;
  if (!enc || enc->index != ENCINDEX_UTF_8) return 1;
  n = utf8_seq_len(*u);
  if (e - p < n) return 1;
  return utf8_valid(u, u + n) ? n : 1;
}

long
rb_enc_strlen(const char *p, const char *e, rb_encoding *enc)
{
  long count = 0;
  if (!enc || enc->index != ENCINDEX_UTF_8) return (long)(e - p);
  while (p < e) {
    p += rb_enc_mbclen(p, e, enc);
    count++;
  }
  return count;
}

int
rb_enc_str_asciionly_p(mrb_value str)
{
  struct RString *s = mrb_str_ptr(str);
  return ascii_only((const unsigned char *)RSTR_PTR(s), (const unsigned char *)RSTR_PTR(s) + RSTR_LEN(s));
}

/* CRuby's rb_enc_check: the encoding two strings can be combined in. The same encoding is that one; otherwise an ASCII-only
   string takes the other's encoding; two non-ASCII strings in different encodings raise Encoding::CompatibilityError */
rb_encoding *
rb_enc_check(mrb_state *mrb, mrb_value a, mrb_value b)
{
  rb_encoding *ea = rb_enc_get(a);
  rb_encoding *eb = rb_enc_get(b);
  if (ea == eb) return ea;
  if (ascii_only((const unsigned char *)RSTRING_PTR(b), (const unsigned char *)RSTRING_PTR(b) + RSTRING_LEN(b))) return ea;
  if (ascii_only((const unsigned char *)RSTRING_PTR(a), (const unsigned char *)RSTRING_PTR(a) + RSTRING_LEN(a))) return eb;
  {
    struct RClass *enc_mod = mrb_module_get(mrb, "Encoding");
    struct RClass *cls = (struct RClass *)mrb_ptr(mrb_const_get(mrb, mrb_obj_value(enc_mod), mrb_intern_cstr(mrb, "CompatibilityError")));
    mrb_raisef(mrb, cls, "incompatible character encodings: %s and %s", ea->name, eb->name);
  }
  return NULL;
}

/* the codepoint of the character at p (see encoding.h) */
unsigned int
rb_enc_codepoint_len(mrb_state *mrb, const char *p, const char *e, int *len_p, rb_encoding *enc)
{
  const unsigned char *u = (const unsigned char *)p;
  int n;
  if (p >= e) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "empty string");
  }
  if (!enc || enc->index != ENCINDEX_UTF_8) {
    if (len_p) *len_p = 1;
    return *u;
  }
  n = utf8_seq_len(*u);
  if (n == 1) {
    if (*u >= 0x80) {
      mrb_raisef(mrb, mrb_class_get(mrb, "ArgumentError"), "invalid byte sequence in UTF-8");
    }
    if (len_p) *len_p = 1;
    return *u;
  }
  if (e - p < n || !utf8_valid(u, u + n)) {
    mrb_raisef(mrb, mrb_class_get(mrb, "ArgumentError"), "invalid byte sequence in UTF-8");
  }
  if (len_p) *len_p = n;
  if (n == 2) return ((u[0] & 0x1F) << 6) | (u[1] & 0x3F);
  if (n == 3) return ((u[0] & 0x0F) << 12) | ((u[1] & 0x3F) << 6) | (u[2] & 0x3F);
  return ((u[0] & 0x07) << 18) | ((u[1] & 0x3F) << 12) | ((u[2] & 0x3F) << 6) | (u[3] & 0x3F);
}

mrb_value
rb_enc_from_encoding(mrb_state *mrb, rb_encoding *enc)
{
  if (!enc) return mrb_nil_value();
  return get_encoding(mrb, enc->index == ENCINDEX_ASCII_8BIT ? "BINARY" : enc->index == ENCINDEX_US_ASCII ? "US_ASCII" : "UTF_8");
}

mrb_value
rb_str_conv_enc(mrb_state *mrb, mrb_value str, rb_encoding *from, rb_encoding *to)
{
  struct RString *s = mrb_str_ptr(str);
  (void)mrb;
  if (!to || (from && from->index == to->index)) return str;
  /* an ASCII-only string has the same characters in each of the three encodings: it takes the target's tag. Anything else is
     not converted (no transcoding), and is returned unchanged, as CRuby does for a failed conversion */
  if (ascii_only((const unsigned char *)RSTR_PTR(s), (const unsigned char *)RSTR_PTR(s) + RSTR_LEN(s))) {
    rb_enc_set_index(str, to->index);
  }
  return str;
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
  mrb_define_const(mrb, e, "US_ASCII", mrb_str_new_lit_frozen(mrb, "US-ASCII"));
  mrb_define_const(mrb, e, "ASCII", mrb_str_new_lit_frozen(mrb, "US-ASCII"));

  /* CRuby's EncodingError, and Encoding::CompatibilityError (raised when two strings cannot be combined) */
  {
    struct RClass *ee = mrb_define_class(mrb, "EncodingError", mrb_class_get(mrb, "StandardError"));
    mrb_define_class_under(mrb, e, "CompatibilityError", ee);
  }
}

void
mrb_mruby_encoding_gem_final(mrb_state *mrb)
{
}
