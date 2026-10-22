/**
** @file mruby/data.h - Data class
**
** See Copyright Notice in mruby.h
*/

#ifndef MRUBY_DATA_H
#define MRUBY_DATA_H

#include "common.h"

/**
 * Custom C wrapped data.
 *
 * Defining Ruby wrappers around native objects.
 */
MRB_BEGIN_DECL

/**
 * Custom data type description.
 */
typedef struct mrb_data_type {
  /** data type name */
  const char *struct_name;

  /** data type release function pointer */
  void (*dfree)(mrb_state *mrb, void*);

  /** Optional compiler-managed instance-variable slot descriptor. */
  const struct mrb_data_ivar *ivars;
  size_t ivar_count;
  size_t ivar_data_size;

  /**
   * Optional open-addressing table over `ivars` (FNV-1a 32 of the name, linear
   * probing, 0xFFFF = empty, size ivar_hash_mask + 1); NULL falls back to a scan.
   */
  const uint16_t *ivar_hash;
  size_t ivar_hash_mask;
} mrb_data_type;

/**
 * How an RData payload stores one ivar. VALUE is an mrb_value the GC marks;
 * the others are raw C values that bc2cpp's generated code reads unboxed
 * (mrb_int, mrb_sym, mrb_bool, and INT_OR_NIL's struct mrb_data_int_or_nil).
 * The generated code proves every store has that type, so a typed slot is
 * always assigned and never holds a heap reference.
 */
enum mrb_data_ivar_kind {
  MRB_DATA_IVAR_VALUE = 0,
  MRB_DATA_IVAR_INT,
  MRB_DATA_IVAR_SYMBOL,
  MRB_DATA_IVAR_BOOL,
  MRB_DATA_IVAR_INT_OR_NIL
};

/** Layout of an MRB_DATA_IVAR_INT_OR_NIL slot. */
struct mrb_data_int_or_nil {
  mrb_bool present;
  mrb_int value;
};

/** A Ruby instance variable stored in an RData payload. */
struct mrb_data_ivar {
  const char *name;
  size_t offset;
  /** enum mrb_data_ivar_kind; zero (a two-field initializer) is VALUE. */
  int kind;
};

struct RData {
  MRB_OBJECT_HEADER;
  struct iv_tbl *iv;
  const mrb_data_type *type;
  void *data;
};

MRB_API struct RData *mrb_data_object_alloc(mrb_state *mrb, struct RClass* klass, void *datap, const mrb_data_type *type);

#define Data_Wrap_Struct(mrb,klass,type,ptr)\
  mrb_data_object_alloc(mrb,klass,ptr,type)

#define Data_Make_Struct(mrb,klass,strct,type,sval,data_obj) do { \
  (data_obj) = Data_Wrap_Struct(mrb,klass,type,NULL);\
  (sval) = (strct*)mrb_malloc(mrb, sizeof(strct));                     \
  { static const strct zero = { 0 }; *(sval) = zero; };\
  (data_obj)->data = (sval);\
} while (0)

#define RDATA(obj)         ((struct RData*)(mrb_ptr(obj)))
#define DATA_PTR(d)        (RDATA(d)->data)
#define DATA_TYPE(d)       (RDATA(d)->type)
MRB_API void mrb_data_check_type(mrb_state *mrb, mrb_value, const mrb_data_type*);
MRB_API void *mrb_data_get_ptr(mrb_state *mrb, mrb_value, const mrb_data_type*);
#define DATA_GET_PTR(mrb,obj,dtype,type) (type*)mrb_data_get_ptr(mrb,obj,dtype)
MRB_API void *mrb_data_check_get_ptr(mrb_state *mrb, mrb_value, const mrb_data_type*);
#define DATA_CHECK_GET_PTR(mrb,obj,dtype,type) (type*)mrb_data_check_get_ptr(mrb,obj,dtype)

/* obsolete functions and macros */
#define mrb_data_check_and_get(mrb,obj,dtype) mrb_data_get_ptr(mrb,obj,dtype)
#define mrb_get_datatype(mrb,val,type) mrb_data_get_ptr(mrb, val, type)
#define mrb_check_datatype(mrb,val,type) mrb_data_get_ptr(mrb, val, type)
#define Data_Get_Struct(mrb,obj,type,sval) do {\
  *(void**)&sval = mrb_data_get_ptr(mrb, obj, type); \
} while (0)

MRB_INLINE void
mrb_data_init(mrb_value v, void *ptr, const mrb_data_type *type)
{
  mrb_assert(mrb_data_p(v));
  DATA_PTR(v) = ptr;
  DATA_TYPE(v) = type;
}

MRB_END_DECL

#endif /* MRUBY_DATA_H */
