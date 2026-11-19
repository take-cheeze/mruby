/*
** mruby/internal.h - internal helpers of the core that gems may use.
**
** This build has none that the bigint gem needs (mrbgems/mruby-bigint includes it for the upstream helpers
** it does not use here). The heap-slot size check that upstream writes with mrb_static_assert_object_size is
** the gem's own (see bigint.h): the slot is private to gc.c.
*/
#ifndef MRUBY_INTERNAL_H
#define MRUBY_INTERNAL_H

#include <mruby.h>

#endif  /* MRUBY_INTERNAL_H */
