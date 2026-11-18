/*
** mruby/encoding.h - the C side of the Poorman's Encoding (mrbgems/mruby-encoding).
**
** Only two encodings exist: UTF-8 and ASCII-8BIT (BINARY). An rb_encoding is a static
** descriptor; there is no transcoding, so a conversion to another encoding is refused.
** The names follow CRuby's C API (ruby/encoding.h) so that converted extensions can
** call them unchanged. struct nfcx_rb_encoding is the name the cext2mrbgem converter
** uses for CRuby's rb_encoding.
*/
#ifndef MRUBY_ENCODING_H
#define MRUBY_ENCODING_H

#include <mruby.h>

#ifdef __cplusplus
extern "C" {
#endif

struct nfcx_rb_encoding {
  const char *name;  /* "UTF-8" or "ASCII-8BIT" */
  int binary;        /* 1 for ASCII-8BIT */
};
typedef struct nfcx_rb_encoding rb_encoding;

MRB_API rb_encoding *rb_utf8_encoding(void);
MRB_API rb_encoding *rb_ascii8bit_encoding(void);
/* the default external encoding is UTF-8 */
MRB_API rb_encoding *rb_default_external_encoding(void);
/* the default internal encoding is nil (NULL), as in CRuby's default */
MRB_API rb_encoding *rb_default_internal_encoding(void);
/* the encoding of a string: ASCII-8BIT when it is binary, UTF-8 otherwise */
MRB_API rb_encoding *rb_enc_get(mrb_value str);
/* tags a string with enc (a String#force_encoding); returns the string */
MRB_API mrb_value rb_enc_associate(mrb_value str, rb_encoding *enc);
/* the string in enc; enc NULL is no conversion. Only UTF-8 and BINARY exist, so a
   string is returned unchanged; there is no transcoding */
MRB_API mrb_value rb_str_export_to_enc(mrb_value str, rb_encoding *enc);

#ifdef __cplusplus
}
#endif

#endif
