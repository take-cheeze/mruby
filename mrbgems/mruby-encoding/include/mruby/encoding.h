/*
** mruby/encoding.h - the C side of the Poorman's Encoding (mrbgems/mruby-encoding).
**
** Three encodings exist: ASCII-8BIT (BINARY), UTF-8 and US-ASCII. An rb_encoding is a static
** descriptor; there is no transcoding. The indices and names follow CRuby (ENCINDEX_ASCII_8BIT = 0,
** ENCINDEX_UTF_8 = 1, ENCINDEX_US_ASCII = 2). A string carries its encoding in two flags: MRB_STR_BINARY
** (ASCII-8BIT) and MRB_STR_USASCII (US-ASCII); a string with neither is UTF-8.
**
** The names follow CRuby's ruby/encoding.h so that converted extensions can call them unchanged.
** struct nfcx_rb_encoding is the name the cext2mrbgem converter uses for CRuby's rb_encoding.
*/
#ifndef MRUBY_ENCODING_H
#define MRUBY_ENCODING_H

#include <mruby.h>
#include <mruby/string.h>

#ifdef __cplusplus
extern "C" {
#endif

/* the US-ASCII flag of a string (bit 12; bit 11 is MRB_STR_BINARY) */
#define MRB_STR_USASCII 4096

#define RSTR_USASCII_P(s)         ((s)->flags & MRB_STR_USASCII)
#define RSTR_SET_USASCII_FLAG(s)  ((s)->flags |= MRB_STR_USASCII)
#define RSTR_UNSET_USASCII_FLAG(s) ((s)->flags &= ~MRB_STR_USASCII)

#define ENCINDEX_ASCII_8BIT 0
#define ENCINDEX_UTF_8      1
#define ENCINDEX_US_ASCII   2

struct nfcx_rb_encoding {
  const char *name;  /* "ASCII-8BIT", "UTF-8" or "US-ASCII" */
  int binary;        /* 1 for ASCII-8BIT */
  int index;         /* ENCINDEX_* */
};
typedef struct nfcx_rb_encoding rb_encoding;

MRB_API rb_encoding *rb_utf8_encoding(void);
MRB_API rb_encoding *rb_ascii8bit_encoding(void);
MRB_API rb_encoding *rb_usascii_encoding(void);
/* the default external encoding is UTF-8 */
MRB_API rb_encoding *rb_default_external_encoding(void);
/* the default internal encoding is nil (NULL), as in CRuby's default */
MRB_API rb_encoding *rb_default_internal_encoding(void);
/* an encoding by index, or NULL for an unknown index */
MRB_API rb_encoding *rb_enc_from_index(int index);
/* the index of an encoding by name (ASCII-8BIT, BINARY, UTF-8, US-ASCII, ASCII; case-insensitive), or -1 */
MRB_API int rb_enc_find_index(const char *name);
MRB_API int rb_enc_to_index(rb_encoding *enc);
MRB_API int rb_utf8_encindex(void);
MRB_API int rb_ascii8bit_encindex(void);
MRB_API int rb_usascii_encindex(void);
/* the encoding of a string: ASCII-8BIT when it is binary, US-ASCII when so tagged, UTF-8 otherwise */
MRB_API rb_encoding *rb_enc_get(mrb_value str);
MRB_API int rb_enc_get_index(mrb_value str);
/* tags a string with enc (a String#force_encoding); returns the string */
MRB_API mrb_value rb_enc_associate(mrb_value str, rb_encoding *enc);
MRB_API mrb_value rb_enc_associate_index(mrb_value str, int index);
/* sets the encoding index of a string (CRuby's ENCODING_SET); an unknown index is ignored */
MRB_API void rb_enc_set_index(mrb_value str, int index);
/* the string in enc; enc NULL is no conversion. There is no transcoding, so a string is returned unchanged */
MRB_API mrb_value rb_str_export_to_enc(mrb_value str, rb_encoding *enc);
/* the number of characters from p up to e in enc (a UTF-8 lead byte counts one character; a malformed byte counts one) */
MRB_API long rb_enc_strlen(const char *p, const char *e, rb_encoding *enc);
/* the byte length of the character at p (1 for ASCII-8BIT and US-ASCII; the UTF-8 sequence length, at least 1) */
MRB_API int rb_enc_mbclen(const char *p, const char *e, rb_encoding *enc);
/* CRuby's rb_enc_check: the encoding two strings combine in (mrb is needed to raise Encoding::CompatibilityError when they
   cannot: two non-ASCII strings in different encodings) */
MRB_API rb_encoding *rb_enc_check(mrb_state *mrb, mrb_value a, mrb_value b);
/* the codepoint of the character at p (UTF-8 decoded; a byte is its own codepoint in ASCII-8BIT and US-ASCII); *len_p
   gets its length. An invalid UTF-8 sequence raises ArgumentError (CRuby's rb_enc_codepoint_len) */
MRB_API unsigned int rb_enc_codepoint_len(mrb_state *mrb, const char *p, const char *e, int *len_p, rb_encoding *enc);
/* the Encoding object of enc (the Encoding name string here); nil for NULL */
MRB_API mrb_value rb_enc_from_encoding(mrb_state *mrb, rb_encoding *enc);
/* the string in enc (CRuby's rb_str_conv_enc): an ASCII-only string, or a conversion into ASCII-8BIT, or a valid UTF-8 string
   into UTF-8, is retagged; any other conversion is not possible here, and the string is returned unchanged (as CRuby does on
   a failed conversion) */
MRB_API mrb_value rb_str_conv_enc(mrb_state *mrb, mrb_value str, rb_encoding *from, rb_encoding *to);
/* CRuby's rb_str_encode(str, to, ecflags, ecopts): a new string of str in the encoding named by to (an Encoding name, as
   rb_enc_from_encoding gives), converted among the three encodings as String#encode does: a character the target lacks
   raises Encoding::UndefinedConversionError and an invalid byte raises Encoding::InvalidByteSequenceError. ecflags and
   ecopts are not used (String#encode's options are in encode.c). */
MRB_API mrb_value rb_str_encode(mrb_state *mrb, mrb_value str, mrb_value to, int ecflags, mrb_value ecopts);
/* CRuby's rb_enc_sprintf: the formatted string (mrb_vformat), tagged with enc (the converter maps PRIsVALUE in the
   format as rb_sprintf does) */
MRB_API mrb_value rb_enc_sprintf(mrb_state *mrb, rb_encoding *enc, const char *format, ...);
/* 1 when every byte of the string is ASCII (7-bit), 0 otherwise */
MRB_API int rb_enc_str_asciionly_p(mrb_value str);

#ifdef __cplusplus
}
#endif

#endif
