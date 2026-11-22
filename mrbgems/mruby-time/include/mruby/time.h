/*
** mruby/time.h - Time class
**
** See Copyright Notice in mruby.h
*/

#ifndef MRUBY_TIME_H
#define MRUBY_TIME_H

#include "mruby/common.h"
#include <time.h>

MRB_BEGIN_DECL

typedef enum mrb_timezone {
  MRB_TIMEZONE_NONE   = 0,
  MRB_TIMEZONE_UTC    = 1,
  MRB_TIMEZONE_LOCAL  = 2,
  MRB_TIMEZONE_OFFSET = 3,  /* a fixed UTC offset (mrb_time_at_offset), in seconds */
  MRB_TIMEZONE_LAST   = 4
} mrb_timezone;

MRB_API mrb_value mrb_time_at(mrb_state *mrb, time_t sec, time_t usec, mrb_timezone timezone);
/* A Time at sec and usec with a fixed UTC offset of offset seconds (east positive), e.g. 32400 for +09:00. */
MRB_API mrb_value mrb_time_at_offset(mrb_state *mrb, time_t sec, time_t usec, int offset);

MRB_END_DECL

#endif /* MRUBY_TIME_H */
