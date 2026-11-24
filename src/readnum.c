/* this file defines obsolete functions: mrb_int_read() and mrb_float_read() */
/* use mrb_read_int() and mrb_read_float() instead */

#include <mruby.h>
#include <mruby/numeric.h>
#include <errno.h>
#include <stdlib.h>

/* mrb_int_read(): read mrb_int from a string (base 10 only) */
/* const char *p - string to read                            */
/* const char *e - end of string                             */
/* char **endp   - end of parsed integer                     */

/* if integer overflows, errno will be set to ERANGE         */
/* also endp will be set to NULL on overflow                 */
MRB_API mrb_int
mrb_int_read(const char *p, const char *e, char **endp)
{
  mrb_int n;

  if (!mrb_read_int(p, e, endp, &n)) {
    if (endp) *endp = NULL;
    errno = ERANGE;
    return MRB_INT_MAX;
  }
  if (endp) *endp = (char*)p;
  return n;
}

#ifndef MRB_NO_FLOAT
//#include <string.h>
//#include <math.h>

MRB_API double
mrb_float_read(const char *str, char **endp)
{
  /* a decimal number is read by strtod, which rounds correctly (5e-324 is a subnormal, not 0.0); anything else is not a number */
  {
    const char *q = str;
    while (*q == ' ' || (*q >= '\t' && *q <= '\r')) q++;
    if (*q == '+' || *q == '-') q++;
    if ((*q >= '0' && *q <= '9') || (*q == '.' && q[1] >= '0' && q[1] <= '9')) {
      return strtod(str, endp);
    }
  }
  {
    double d0;
    if (!mrb_read_float(str, endp, &d0)) errno = ERANGE;
    return d0;
  }
}

#if 0  /* the previous reader, replaced by the strtod path above */
static double
mrb_float_read_unused(const char *str, char **endp)
{
  double d;

  if (!mrb_read_float(str, endp, &d)) {
    errno = ERANGE;
  }
  return d;
}
#endif
#endif
