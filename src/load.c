/*
** load.c - mruby binary loader
**
** See Copyright Notice in mruby.h
*/

#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <mruby/dump.h>
#include <mruby/irep.h>
#include <mruby/proc.h>
#include <mruby/string.h>
#include <mruby/debug.h>
#include <mruby/error.h>

#include "lj_bcdump.h"
#include "lj_lex.h"

#if SIZE_MAX < UINT32_MAX
# error size_t must be at least 32 bits wide
#endif

mrb_irep*
mrb_read_irep(mrb_state *mrb, const uint8_t *bin)
{
  LexState ls;
  lj_lex_setup(mrb->L, &ls);
  lj_bcread(&ls);
}

MRB_API mrb_value
mrb_load_irep_cxt(mrb_state *mrb, const uint8_t *bin, mrbc_context *c)
{
}

MRB_API mrb_value
mrb_load_irep(mrb_state *mrb, const uint8_t *bin)
{
}

#ifndef MRB_DISABLE_STDIO

mrb_irep*
mrb_read_irep_file(mrb_state *mrb, FILE* fp)
{
}

MRB_API mrb_value
mrb_load_irep_file_cxt(mrb_state *mrb, FILE* fp, mrbc_context *c)
{
}

MRB_API mrb_value
mrb_load_irep_file(mrb_state *mrb, FILE* fp)
{
}
#endif /* MRB_DISABLE_STDIO */
