#ifndef MRB_VALUE_ARRAY_H__
#define MRB_VALUE_ARRAY_H__

#include <mruby.h>

static inline void
values_move(mrb_value *s1, const mrb_value *s2, size_t n)
{
  if (s1 > s2 && s1 < s2 + n)
  {
    s1 += n;
    s2 += n;
    while (n-- > 0) {
      *--s1 = *--s2;
    }
  }
  else if (s1 != s2) {
    while (n-- > 0) {
      *s1++ = *s2++;
    }
  }
  else {
    /* nothing to do. */
  }
}

static inline void
values_clear(mrb_state *mrb, mrb_value *from, size_t count)
{
  while (count-- > 0) {
    mrb_dec_ref(mrb, *from);
    SET_NIL_VALUE(*from);
    from++;
  }
}

static inline void
values_nil_init(mrb_value *ptr, mrb_int size)
{
  mrb_value nil = mrb_nil_value();

  while (size--) {
    *ptr++ = nil;
  }
}

/*
 * to copy array, use this instead of memcpy because of portability
 * * gcc on ARM may fail optimization of memcpy
 *   http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.faqs/ka3934.html
 * * gcc on MIPS also fail
 *   http://gcc.gnu.org/bugzilla/show_bug.cgi?id=39755
 * * memcpy doesn't exist on freestanding environment
 *
 * If you optimize for binary size, use memcpy instead of this at your own risk
 * of above portability issue.
 *
 * see also http://togetter.com/li/462898
 *
 */

static inline void
values_copy(mrb_state *mrb, mrb_value *dst, const mrb_value *src, size_t n)
{
  if (dst > src && dst < src + n)
  {
    dst += n;
    src += n;
    while (n-- > 0) {
      --dst; --src;
      mrb_ref_set(mrb, *dst, *src);
    }
  }
  else if (dst != src) {
    while (n-- > 0) {
      mrb_ref_set(mrb, *dst, *src);
      dst++; src++;
    }
  }
  else {
    /* nothing to do. */
  }
}

static inline void
values_init(mrb_state *mrb, mrb_value *dst, const mrb_value *src, size_t size)
{
  while (size-- > 0) {
    mrb_inc_ref(mrb, *src);
    *dst++ = *src++;
  }
}

#endif /* MRB_VALUE_ARRAY_H__ */
