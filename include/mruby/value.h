#pragma once

#include <lj_obj.h>
#include <lj_str.h>
#include <lj_gc.h>
#include <lj_tab.h>

typedef _Bool mrb_bool;
typedef lua_Integer mrb_int;
typedef lua_Number mrb_float;

typedef TValue mrb_value;

typedef GCstr* mrb_sym;

typedef GCobj RObject;
typedef GCudata RClass;
typedef GCtab RArray;
typedef GCtab RHash;
typedef GCudata RString;
typedef GCudata RData;
typedef GCproto mrb_irep;
typedef GCfunc RProc;
typedef GCobj RBasic;

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

static inline mrb_bool mrb_float_p(mrb_value v) { return tvisnumber(&v); }

static inline mrb_vtype mrb_type(mrb_value v) {
  if (tvisint(&v)) { return MRB_TT_FIXNUM; }

  switch (itype(&v)) {
  case LJ_TNIL: return MRB_TT_FALSE;
  case LJ_TTRUE: return MRB_TT_TRUE;
  case LJ_TLIGHTUD: return MRB_TT_CPTR;
  case LJ_TSTR: return MRB_TT_SYMBOL;
  case LJ_TTHREAD: return MRB_TT_FIBER;
  case LJ_TFUNC: return MRB_TT_PROC;
  case LJ_TUDATA: return MRB_TT_DATA;
  }

  return MRB_TT_UNDEF;
}

static inline mrb_int mrb_fixnum(mrb_value v) { return intV(&v); }
static inline mrb_bool mrb_nil_p(mrb_value v) { return tvisnil(&v); }
static inline mrb_sym mrb_symbol(mrb_value v) { return strV(&v); }
static inline void *mrb_ptr(mrb_value v) { return gcval(&v); }
#define mrb_float(v) numberVnum(&v)

static inline mrb_value mrb_obj_value_impl(RObject *obj) {
  mrb_value v;
  setgcV(NULL, &v, obj, ~obj->gch.gct);
  return v;
}
#define mrb_obj_value(v) mrb_obj_value_impl(obj2gco(v))

static inline mrb_value mrb_nil_value() {
  mrb_value v;
  setnilV(&v);
  return v;
}

static inline mrb_value mrb_symbol_value(mrb_sym s) {
  mrb_value v;
  setstrV(NULL, &v, s);
  return v;
}

static inline RBasic* mrb_basic_ptr(mrb_value v) {
  return gcV(&v);
}

static inline RClass* mrb_class_ptr(mrb_value v) {
  return udataV(&v);
}

static inline mrb_value mrb_fixnum_value(mrb_int i) {
  mrb_value ret;
  setintV(&ret, i);
  return ret;
}

static inline RObject* mrb_obj_ptr(mrb_value v) {
  return gcV(&v);
}

static inline mrb_value mrb_bool_value(mrb_bool b) {
  mrb_value ret;
  setboolV(&ret, b);
  return ret;
}

static inline mrb_value mrb_false_value() { return mrb_bool_value(FALSE); }
static inline mrb_value mrb_true_value() { return mrb_bool_value(TRUE); }

static inline mrb_bool mrb_array_p(mrb_value v) { return tvistab(&v); }

static inline mrb_bool mrb_fixnum_p(mrb_value v) { return tvisint(&v); }
static inline mrb_bool mrb_string_p(mrb_value v) { return tvisudata(&v); }

mrb_vtype mrb_obj_type(RObject *v);

#define MRB_INT_MAX INT32_MAX
#define MRB_INT_MIN INT32_MIN
