#pragma once

#include <lj_gc.h>
#include <lj_obj.h>
#include <lj_str.h>
#include <lj_state.h>
#include <lj_tab.h>
#include <lj_udata.h>
#include <lauxlib.h>

typedef enum mrb_vtype {
  MRB_TT_FALSE = 0,   /*   0 */
  MRB_TT_FREE,        /*   1 */
  MRB_TT_TRUE,        /*   2 */
  MRB_TT_FIXNUM,      /*   3 */
  MRB_TT_SYMBOL,      /*   4 */
  MRB_TT_UNDEF,       /*   5 */
  MRB_TT_FLOAT,       /*   6 */
  MRB_TT_CPTR,        /*   7 */
  MRB_TT_OBJECT,      /*   8 */
  MRB_TT_CLASS,       /*   9 */
  MRB_TT_MODULE,      /*  10 */
  MRB_TT_ICLASS,      /*  11 */
  MRB_TT_SCLASS,      /*  12 */
  MRB_TT_PROC,        /*  13 */
  MRB_TT_ARRAY,       /*  14 */
  MRB_TT_HASH,        /*  15 */
  MRB_TT_STRING,      /*  16 */
  MRB_TT_RANGE,       /*  17 */
  MRB_TT_EXCEPTION,   /*  18 */
  MRB_TT_FILE,        /*  19 */
  MRB_TT_ENV,         /*  20 */
  MRB_TT_DATA,        /*  21 */
  MRB_TT_FIBER,       /*  22 */
  MRB_TT_ISTRUCT,     /*  23 */
  MRB_TT_BREAK,       /*  24 */
  MRB_TT_MAXDEFINE    /*  25 */
} mrb_vtype;

#define MRB_TT_HAS_BASIC MRB_TT_OBJECT

typedef _Bool mrb_bool;
typedef int32_t mrb_int;
typedef lua_Number mrb_float;
typedef GCstr* mrb_sym;

#define MRB_INT_MAX INT32_MAX
#define MRB_INT_MIN INT32_MIN
#define MRB_INT_BIT 32

#define ARY_SIZE_MAX MRB_INT_MAX

typedef TValue* mrb_value;

#define mrb_fixnum(v) intV(v)
#define mrb_float(v) numV(v)
#define mrb_symbol(v) strV(v)
#define mrb_test(v) (mrb_type(v) == MRB_TT_FALSE)
#define mrb_bool(v) mrb_test(v)

#define mrb_type(v) ((struct RBasic*)udataV(v))->tt
#define mrb_ptr(v) ((void*)udataV(v))

#define mrb_obj_value(ptr) (setudataV(mrb->L, mrb->L->top, (GCudata*)ptr), incr_top(mrb->L), mrb->L->top - 1)
#define mrb_fixnum_value(v) (setintV(mrb->L->top, v), incr_top(mrb->L), mrb->L->top - 1)
#define mrb_bool_value(v) ((v)? mrb->true_value : mrb->false_value)
#define mrb_false_value() (mrb->false_value)
#define mrb_true_value() (mrb->true_value)
#define mrb_nil_value() (mrb->nil_value)
#define mrb_undef_value() niltv(mrb->L)
#define mrb_symbol_value(sym) (setstrV(mrb->L, mrb->L->top, sym), incr_top(mrb->L), mrb->L->top - 1)
#define mrb_float_value(mrb, f) (setnumV(mrb->L->top, f), incr_top(mrb->L), mrb->L->top - 1)
#define mrb_float_pool(mrb, f) mrb_float_value(mrb, f)

#define mrb_array_p(v) (mrb_type(v) == MRB_TT_ARRAY)
#define mrb_fixnum_p(v) (mrb_type(v) == MRB_TT_FIXNUM)
#define mrb_float_p(v) (mrb_type(v) == MRB_TT_FLOAT)
#define mrb_nil_p(v) (mrb_type(v) == MRB_TT_FALSE && mrb_ptr(v) == NULL)
#define mrb_string_p(v) (mrb_type(v) == MRB_TT_STRING)
#define mrb_symbol_p(v) (mrb_type(v) == MRB_TT_SYMBOL)
#define mrb_undef_p(v) tvisnil(v)

#define mrb_ro_data_p(p) FALSE

MRB_API double mrb_float_read(const char *string, char **endPtr);

#define MRB_PRId "d"

#include <mruby/object.h>
