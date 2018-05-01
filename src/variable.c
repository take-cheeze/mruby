/*
** variable.c - mruby variables
**
** See Copyright Notice in mruby.h
*/

#include <mruby.h>
#include <mruby/array.h>
#include <mruby/class.h>
#include <mruby/proc.h>
#include <mruby/string.h>

#include "lj_tab.h"

typedef int (iv_foreach_func)(mrb_state*,mrb_sym,mrb_value,void*);

/*
#include <mruby/khash.h>

#ifndef MRB_IVHASH_INIT_SIZE
#define MRB_IVHASH_INIT_SIZE KHASH_MIN_SIZE
#endif

KHASH_DECLARE(iv, mrb_sym, mrb_value, TRUE)
KHASH_DEFINE(iv, mrb_sym, mrb_value, TRUE, kh_int_hash_func, kh_int_hash_equal)

// Instance variable table structure
typedef struct iv_tbl {
  khash_t(iv) h;
} iv_tbl;
*/
typedef GCtab iv_tbl;

/*
 * Creates the instance variable table.
 *
 * Parameters
 *   mrb
 * Returns
 *   the instance variable table.
 */
/*
static iv_tbl*
iv_new(mrb_state *mrb)
{
  return (iv_tbl*)kh_init_size(iv, mrb, MRB_IVHASH_INIT_SIZE);
}
*/

/*
 * Set the value for the symbol in the instance variable table.
 *
 * Parameters
 *   mrb
 *   t     the instance variable table to be set in.
 *   sym   the symbol to be used as the key.
 *   val   the value to be set.
 */
static void
iv_put(mrb_state *mrb, iv_tbl *t, mrb_sym sym, mrb_value val)
{
  setgcV(mrb->L, lj_tab_setstr(mrb->L, t, sym), gcV(&val), itype(&val));
}

/*
 * Get a value for a symbol from the instance variable table.
 *
 * Parameters
 *   mrb
 *   t     the variable table to be searched.
 *   sym   the symbol to be used as the key.
 *   vp    the value pointer. Receives the value if the specified symbol is
 *         contained in the instance variable table.
 * Returns
 *   true if the specified symbol is contained in the instance variable table.
 */
static mrb_bool
iv_get(mrb_state *mrb, iv_tbl *t, mrb_sym sym, mrb_value *vp)
{
  cTValue *v = lj_tab_getstr(t, sym);
  *vp = *v;
  return v;
}

/*
 * Deletes the value for the symbol from the instance variable table.
 *
 * Parameters
 *   t    the variable table to be searched.
 *   sym  the symbol to be used as the key.
 *   vp   the value pointer. Receive the deleted value if the symbol is
 *        contained in the instance variable table.
 * Returns
 *   true if the specified symbol is contained in the instance variable table.
 */
static mrb_bool
iv_del(mrb_state *mrb, iv_tbl *t, mrb_sym sym, mrb_value *vp)
{
  cTValue *v = lj_tab_getstr(t, sym);
  if (!v) { return FALSE; }

  if (vp) { *vp = *v; }
  setnilV(lj_tab_setstr(mrb->L, t, sym));
  return TRUE;
}

static mrb_bool
iv_foreach(mrb_state *mrb, iv_tbl *t, iv_foreach_func *func, void *p)
{
  mrb_value i[2];

  if (t == NULL) { return TRUE; }

  setnilV(i);
  while (lj_tab_next(mrb->L, t, i)) {
    int n = (*func)(mrb, strV(i + 0), i[1], p);
    if (n > 0) return FALSE;
    if (n < 0) {
      iv_del(mrb, t, strV(i + 1), NULL);
    }
  }
  return TRUE;
}

static size_t
iv_size(mrb_state *mrb, iv_tbl *t)
{
  if (t) {
    return lj_tab_len(t);
  }
  return 0;
}

static iv_tbl*
iv_copy(mrb_state *mrb, iv_tbl *t)
{
  return lj_tab_dup(mrb->L, t);
}

mrb_value
mrb_vm_special_get(mrb_state *mrb, mrb_sym i)
{
  return mrb_fixnum_value(0);
}

void
mrb_vm_special_set(mrb_state *mrb, mrb_sym i, mrb_value v)
{
}

static mrb_bool
obj_iv_p(mrb_value obj)
{
  switch (mrb_type(obj)) {
    case MRB_TT_OBJECT:
    case MRB_TT_CLASS:
    case MRB_TT_MODULE:
    case MRB_TT_SCLASS:
    case MRB_TT_HASH:
    case MRB_TT_DATA:
    case MRB_TT_EXCEPTION:
      return TRUE;
    default:
      return FALSE;
  }
}

MRB_API mrb_value
mrb_obj_iv_get(mrb_state *mrb, RObject *obj, mrb_sym sym)
{
  mrb_value v;

  if (iv_get(mrb, obj, sym, &v))
    return v;
  return mrb_nil_value();
}

MRB_API mrb_value
mrb_iv_get(mrb_state *mrb, mrb_value obj, mrb_sym sym)
{
  if (obj_iv_p(obj)) {
    return mrb_obj_iv_get(mrb, mrb_obj_ptr(obj), sym);
  }
  return mrb_nil_value();
}

MRB_API void
mrb_obj_iv_set(mrb_state *mrb, RObject *obj, mrb_sym sym, mrb_value v)
{
  mrb_write_barrier(mrb, (RBasic*)obj);
  iv_put(mrb, obj, sym, v);
}

MRB_API void
mrb_iv_set(mrb_state *mrb, mrb_value obj, mrb_sym sym, mrb_value v)
{
  if (obj_iv_p(obj)) {
    mrb_obj_iv_set(mrb, mrb_obj_ptr(obj), sym, v);
  }
  else {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "cannot set instance variable");
  }
}

MRB_API mrb_bool
mrb_obj_iv_defined(mrb_state *mrb, RObject *obj, mrb_sym sym)
{
  return iv_get(mrb, obj, sym, NULL);
}

MRB_API mrb_bool
mrb_iv_defined(mrb_state *mrb, mrb_value obj, mrb_sym sym)
{
  if (!obj_iv_p(obj)) return FALSE;
  return mrb_obj_iv_defined(mrb, mrb_obj_ptr(obj), sym);
}

#define identchar(c) (ISALNUM(c) || (c) == '_' || !ISASCII(c))

MRB_API mrb_bool
mrb_iv_p(mrb_state *mrb, mrb_sym iv_name)
{
  const char *s;
  mrb_int i, len;

  s = mrb_sym2name_len(mrb, iv_name, &len);
  if (len < 2) return FALSE;
  if (s[0] != '@') return FALSE;
  if (s[1] == '@') return FALSE;
  for (i=1; i<len; i++) {
    if (!identchar(s[i])) return FALSE;
  }
  return TRUE;
}

MRB_API void
mrb_iv_check(mrb_state *mrb, mrb_sym iv_name)
{
  if (!mrb_iv_p(mrb, iv_name)) {
    mrb_name_error(mrb, iv_name, "'%S' is not allowed as an instance variable name", mrb_sym2str(mrb, iv_name));
  }
}

MRB_API void
mrb_iv_copy(mrb_state *mrb, mrb_value dest, mrb_value src)
{
  RObject *d = mrb_obj_ptr(dest);
  RObject *s = mrb_obj_ptr(src);

  mrb_write_barrier(mrb, (RBasic*)d);
  // d->iv = iv_copy(mrb, s);
}

static int
inspect_i(mrb_state *mrb, mrb_sym sym, mrb_value v, void *p)
{
  mrb_value str = *(mrb_value*)p;
  const char *s;
  mrb_int len;
  mrb_value ins;
  char *sp = RSTRING_PTR(str);

  /* need not to show internal data */
  if (sp[0] == '-') { /* first element */
    sp[0] = '#';
    mrb_str_cat_lit(mrb, str, " ");
  }
  else {
    mrb_str_cat_lit(mrb, str, ", ");
  }
  s = mrb_sym2name_len(mrb, sym, &len);
  mrb_str_cat(mrb, str, s, len);
  mrb_str_cat_lit(mrb, str, "=");
  if (mrb_type(v) == MRB_TT_OBJECT) {
    ins = mrb_any_to_s(mrb, v);
  }
  else {
    ins = mrb_inspect(mrb, v);
  }
  mrb_str_cat_str(mrb, str, ins);
  return 0;
}

mrb_value
mrb_obj_iv_inspect(mrb_state *mrb, RObject *obj)
{
  iv_tbl *t = obj;
  size_t len = iv_size(mrb, t);

  if (len > 0) {
    const char *cn = mrb_obj_classname(mrb, mrb_obj_value(obj));
    mrb_value str = mrb_str_new_capa(mrb, 30);

    mrb_str_cat_lit(mrb, str, "-<");
    mrb_str_cat_cstr(mrb, str, cn);
    mrb_str_cat_lit(mrb, str, ":");
    mrb_str_concat(mrb, str, mrb_ptr_to_str(mrb, obj));

    iv_foreach(mrb, t, inspect_i, &str);
    mrb_str_cat_lit(mrb, str, ">");
    return str;
  }
  return mrb_any_to_s(mrb, mrb_obj_value(obj));
}

MRB_API mrb_value
mrb_iv_remove(mrb_state *mrb, mrb_value obj, mrb_sym sym)
{
  if (obj_iv_p(obj)) {
    iv_tbl *t = mrb_obj_ptr(obj);
    mrb_value val;

    if (t && iv_del(mrb, t, sym, &val)) {
      return val;
    }
  }
  return mrb_nil_value();
}

/*
mrb_value
mrb_vm_iv_get(mrb_state *mrb, mrb_sym sym)
{
  // get self
  return mrb_iv_get(mrb, mrb->c->stack[0], sym);
}

void
mrb_vm_iv_set(mrb_state *mrb, mrb_sym sym, mrb_value v)
{
  // get self
  mrb_iv_set(mrb, mrb->c->stack[0], sym, v);
}
*/

static int
iv_i(mrb_state *mrb, mrb_sym sym, mrb_value v, void *p)
{
  mrb_value ary;
  const char* s;
  mrb_int len;

  ary = *(mrb_value*)p;
  s = mrb_sym2name_len(mrb, sym, &len);
  if (len > 1 && s[0] == '@' && s[1] != '@') {
    mrb_ary_push(mrb, ary, mrb_symbol_value(sym));
  }
  return 0;
}

/* 15.3.1.3.23 */
/*
 *  call-seq:
 *     obj.instance_variables    -> array
 *
 *  Returns an array of instance variable names for the receiver. Note
 *  that simply defining an accessor does not create the corresponding
 *  instance variable.
 *
 *     class Fred
 *       attr_accessor :a1
 *       def initialize
 *         @iv = 3
 *       end
 *     end
 *     Fred.new.instance_variables   #=> [:@iv]
 */
mrb_value
mrb_obj_instance_variables(mrb_state *mrb, mrb_value self)
{
  mrb_value ary;

  ary = mrb_ary_new(mrb);
  if (obj_iv_p(self) && mrb_obj_ptr(self)) {
    iv_foreach(mrb, mrb_obj_ptr(self), iv_i, &ary);
  }
  return ary;
}

static int
cv_i(mrb_state *mrb, mrb_sym sym, mrb_value v, void *p)
{
  mrb_value ary;
  const char* s;
  mrb_int len;

  ary = *(mrb_value*)p;
  s = mrb_sym2name_len(mrb, sym, &len);
  if (len > 2 && s[0] == '@' && s[1] == '@') {
    mrb_ary_push(mrb, ary, mrb_symbol_value(sym));
  }
  return 0;
}

/* 15.2.2.4.19 */
/*
 *  call-seq:
 *     mod.class_variables   -> array
 *
 *  Returns an array of the names of class variables in <i>mod</i>.
 *
 *     class One
 *       @@var1 = 1
 *     end
 *     class Two < One
 *       @@var2 = 2
 *     end
 *     One.class_variables   #=> [:@@var1]
 *     Two.class_variables   #=> [:@@var2]
 */
mrb_value
mrb_mod_class_variables(mrb_state *mrb, mrb_value mod)
{
  mrb_value ary;
  RClass *c;

  ary = mrb_ary_new(mrb);
  c = mrb_class_ptr(mod);
  while (c) {
    if (c) {
      iv_foreach(mrb, c, cv_i, &ary);
    }
    c = mrb_cls_super(mrb, c);
  }
  return ary;
}

MRB_API mrb_value
mrb_mod_cv_get(mrb_state *mrb, RClass *c, mrb_sym sym)
{
  RClass * cls = c;
  mrb_value v;
  int given = FALSE;

  while (c) {
    if (iv_get(mrb, c, sym, &v)) {
      given = TRUE;
    }
    c = mrb_cls_super(mrb, c);
  }
  if (given) return v;
  if (cls /* && cls->tt == MRB_TT_SCLASS */) {
    mrb_value klass;

    klass = mrb_obj_iv_get(mrb, (RObject *)cls,
                           mrb_intern_lit(mrb, "__attached__"));
    c = mrb_class_ptr(klass);

    given = FALSE;
    while (c) {
      if (iv_get(mrb, c, sym, &v)) {
        given = TRUE;
      }
      c = mrb_cls_super(mrb, c);
    }
    if (given) return v;
  }
  mrb_name_error(mrb, sym, "uninitialized class variable %S in %S",
                 mrb_sym2str(mrb, sym), mrb_obj_value(cls));
  /* not reached */
  return mrb_nil_value();
}

MRB_API mrb_value
mrb_cv_get(mrb_state *mrb, mrb_value mod, mrb_sym sym)
{
  return mrb_mod_cv_get(mrb, mrb_class_ptr(mod), sym);
}

MRB_API void
mrb_mod_cv_set(mrb_state *mrb, RClass *c, mrb_sym sym, mrb_value v)
{
  RClass * cls = c;

  while (c) {
    iv_tbl *t = c;

    if (iv_get(mrb, t, sym, NULL)) {
      mrb_write_barrier(mrb, (RBasic*)c);
      iv_put(mrb, t, sym, v);
      return;
    }
    c = mrb_cls_super(mrb, c);
  }

  if (cls /* && cls->tt == MRB_TT_SCLASS */) {
    mrb_value klass;

    klass = mrb_obj_iv_get(mrb, (RObject*)cls,
                           mrb_intern_lit(mrb, "__attached__"));
    switch (mrb_type(klass)) {
    case MRB_TT_CLASS:
    case MRB_TT_MODULE:
    case MRB_TT_SCLASS:
      c = mrb_class_ptr(klass);
      break;
    default:
      c = cls;
      break;
    }
  }
  else{
    c = cls;
  }

  mrb_write_barrier(mrb, (RBasic*)c);
  iv_put(mrb, c, sym, v);
}

MRB_API void
mrb_cv_set(mrb_state *mrb, mrb_value mod, mrb_sym sym, mrb_value v)
{
  mrb_mod_cv_set(mrb, mrb_class_ptr(mod), sym, v);
}

MRB_API mrb_bool
mrb_mod_cv_defined(mrb_state *mrb, RClass * c, mrb_sym sym)
{
  while (c) {
    iv_tbl *t = c;
    if (iv_get(mrb, t, sym, NULL)) return TRUE;
    c = mrb_cls_super(mrb, c);
  }

  return FALSE;
}

MRB_API mrb_bool
mrb_cv_defined(mrb_state *mrb, mrb_value mod, mrb_sym sym)
{
  return mrb_mod_cv_defined(mrb, mrb_class_ptr(mod), sym);
}

mrb_value
mrb_vm_cv_get(mrb_state *mrb, mrb_sym sym)
{
  RClass *c;

  c = mrb_class(mrb, mrb->L->base[0]);
  return mrb_mod_cv_get(mrb, c, sym);
}

void
mrb_vm_cv_set(mrb_state *mrb, mrb_sym sym, mrb_value v)
{
  RClass *c;

  c = mrb_class(mrb, mrb->L->base[0]);
  mrb_mod_cv_set(mrb, c, sym, v);
}

static void
mod_const_check(mrb_state *mrb, mrb_value mod)
{
  switch (mrb_type(mod)) {
  case MRB_TT_CLASS:
  case MRB_TT_MODULE:
  case MRB_TT_SCLASS:
    break;
  default:
    mrb_raise(mrb, E_TYPE_ERROR, "constant look-up for non class/module");
    break;
  }
}

static mrb_value
const_get(mrb_state *mrb, RClass *base, mrb_sym sym, mrb_bool top)
{
  RClass *c = base;
  mrb_value v;
  mrb_bool retry = FALSE;
  mrb_value name;
  RClass *oclass = mrb->object_class;

L_RETRY:
  while (c) {
    if ((top || c != oclass || base == oclass)) {
      if (iv_get(mrb, c, sym, &v))
        return v;
    }
    c = mrb_cls_super(mrb, c);
  }
  if (!retry /* && base->tt == MRB_TT_MODULE */) {
    c = oclass;
    retry = TRUE;
    goto L_RETRY;
  }
  name = mrb_symbol_value(sym);
  return mrb_funcall_argv(mrb, mrb_obj_value(base), mrb_intern_lit(mrb, "const_missing"), 1, &name);
}

MRB_API mrb_value
mrb_const_get(mrb_state *mrb, mrb_value mod, mrb_sym sym)
{
  mod_const_check(mrb, mod);
  return const_get(mrb, mrb_class_ptr(mod), sym, FALSE);
}

mrb_value
mrb_vm_const_get(mrb_state *mrb, mrb_sym sym)
{
  RClass *c;
  RClass *c2;
  mrb_value v;
  RProc *proc;

  c = mrb_class(mrb, mrb->L->base[0]);
  if (iv_get(mrb, c, sym, &v)) {
    return v;
  }
  c2 = c;
  while (c2 /* && c2->tt == MRB_TT_SCLASS */) {
    mrb_value klass;
    klass = mrb_obj_iv_get(mrb, (RObject *)c2,
                           mrb_intern_lit(mrb, "__attached__"));
    c2 = mrb_class_ptr(klass);
  }
  // if (c2->tt == MRB_TT_CLASS || c2->tt == MRB_TT_MODULE) c = c2;
  c = c2;
  // mrb_assert(!MRB_PROC_CFUNC_P(mrb->c->ci->proc));
  /*
  proc = mrb->c->ci->proc;
  while (proc) {
    c2 = MRB_PROC_TARGET_CLASS(proc);
    if (c2 && iv_get(mrb, c2, sym, &v)) {
      return v;
    }
    proc = proc->upper;
  }
  */
  return const_get(mrb, c, sym, TRUE);
}

MRB_API void
mrb_const_set(mrb_state *mrb, mrb_value mod, mrb_sym sym, mrb_value v)
{
  mod_const_check(mrb, mod);
  if (mrb_type(v) == MRB_TT_CLASS || mrb_type(v) == MRB_TT_MODULE) {
    mrb_class_name_class(mrb, mrb_class_ptr(mod), mrb_class_ptr(v), sym);
  }
  mrb_iv_set(mrb, mod, sym, v);
}

void
mrb_vm_const_set(mrb_state *mrb, mrb_sym sym, mrb_value v)
{
  RClass *c;

  c = mrb_class(mrb, mrb->L->base[0]);
  mrb_obj_iv_set(mrb, (RObject*)c, sym, v);
}

MRB_API void
mrb_const_remove(mrb_state *mrb, mrb_value mod, mrb_sym sym)
{
  mod_const_check(mrb, mod);
  mrb_iv_remove(mrb, mod, sym);
}

MRB_API void
mrb_define_const(mrb_state *mrb, RClass *mod, const char *name, mrb_value v)
{
  mrb_obj_iv_set(mrb, (RObject*)mod, mrb_intern_cstr(mrb, name), v);
}

MRB_API void
mrb_define_global_const(mrb_state *mrb, const char *name, mrb_value val)
{
  mrb_define_const(mrb, mrb->object_class, name, val);
}

static int
const_i(mrb_state *mrb, mrb_sym sym, mrb_value v, void *p)
{
  mrb_value ary;
  const char* s;
  mrb_int len;

  ary = *(mrb_value*)p;
  s = mrb_sym2name_len(mrb, sym, &len);
  if (len >= 1 && ISUPPER(s[0])) {
    mrb_ary_push(mrb, ary, mrb_symbol_value(sym));
  }
  return 0;
}

/* 15.2.2.4.24 */
/*
 *  call-seq:
 *     mod.constants    -> array
 *
 *  Returns an array of all names of contants defined in the receiver.
 */
mrb_value
mrb_mod_constants(mrb_state *mrb, mrb_value mod)
{
  mrb_value ary;
  mrb_bool inherit = TRUE;
  RClass *c = mrb_class_ptr(mod);

  mrb_get_args(mrb, "|b", &inherit);
  ary = mrb_ary_new(mrb);
  while (c) {
    iv_foreach(mrb, c, const_i, &ary);
    if (!inherit) break;
    c = mrb_cls_super(mrb, c);
    if (c == mrb->object_class) break;
  }
  return ary;
}

MRB_API mrb_value
mrb_gv_get(mrb_state *mrb, mrb_sym sym)
{
  mrb_value v;

  if (iv_get(mrb, mrb_globals(mrb), sym, &v))
    return v;
  return mrb_nil_value();
}

MRB_API void
mrb_gv_set(mrb_state *mrb, mrb_sym sym, mrb_value v)
{
  iv_put(mrb, mrb_globals(mrb), sym, v);
}

MRB_API void
mrb_gv_remove(mrb_state *mrb, mrb_sym sym)
{
  iv_del(mrb, mrb_globals(mrb), sym, NULL);
}

static int
gv_i(mrb_state *mrb, mrb_sym sym, mrb_value v, void *p)
{
  mrb_value ary;

  ary = *(mrb_value*)p;
  mrb_ary_push(mrb, ary, mrb_symbol_value(sym));
  return 0;
}

/* 15.3.1.2.4  */
/* 15.3.1.3.14 */
/*
 *  call-seq:
 *     global_variables    -> array
 *
 *  Returns an array of the names of global variables.
 *
 *     global_variables.grep /std/   #=> [:$stdin, :$stdout, :$stderr]
 */
mrb_value
mrb_f_global_variables(mrb_state *mrb, mrb_value self)
{
  iv_tbl *t = mrb_globals(mrb);
  mrb_value ary = mrb_ary_new(mrb);
  size_t i;
  char buf[3];

  if (t) {
    iv_foreach(mrb, t, gv_i, &ary);
  }
  buf[0] = '$';
  buf[2] = 0;
  for (i = 1; i <= 9; ++i) {
    buf[1] = (char)(i + '0');
    mrb_ary_push(mrb, ary, mrb_symbol_value(mrb_intern(mrb, buf, 2)));
  }
  return ary;
}

static mrb_bool
mrb_const_defined_0(mrb_state *mrb, mrb_value mod, mrb_sym id, mrb_bool exclude, mrb_bool recurse)
{
  RClass *klass = mrb_class_ptr(mod);
  RClass *tmp;
  mrb_bool mod_retry = FALSE;

  tmp = klass;
retry:
  while (tmp) {
    if (tmp && iv_get(mrb, tmp, id, NULL)) {
      return TRUE;
    }
    if (!recurse && (klass != mrb->object_class)) break;
    tmp = mrb_cls_super(mrb, tmp);
  }
  if (!exclude && !mod_retry /* && (klass->tt == MRB_TT_MODULE) */) {
    mod_retry = TRUE;
    tmp = mrb->object_class;
    goto retry;
  }
  return FALSE;
}

MRB_API mrb_bool
mrb_const_defined(mrb_state *mrb, mrb_value mod, mrb_sym id)
{
  return mrb_const_defined_0(mrb, mod, id, TRUE, TRUE);
}

MRB_API mrb_bool
mrb_const_defined_at(mrb_state *mrb, mrb_value mod, mrb_sym id)
{
  return mrb_const_defined_0(mrb, mod, id, TRUE, FALSE);
}

MRB_API mrb_value
mrb_attr_get(mrb_state *mrb, mrb_value obj, mrb_sym id)
{
  return mrb_iv_get(mrb, obj, id);
}

struct csym_arg {
  RClass *c;
  mrb_sym sym;
};
 
static int
csym_i(mrb_state *mrb, mrb_sym sym, mrb_value v, void *p)
{
  struct csym_arg *a = (struct csym_arg*)p;
  RClass *c = a->c;
 
  if (mrb_class_ptr(v) == c) {
    a->sym = sym;
    return 1;     /* stop iteration */
  }
  return 0;
}
 
static mrb_sym
find_class_sym(mrb_state *mrb, RClass *outer, RClass *c)
{
  struct csym_arg arg;
 
  if (!outer) return 0;
  if (outer == c) return 0;
  arg.c = c;
  arg.sym = 0;
  iv_foreach(mrb, outer, csym_i, &arg);
  return arg.sym;
}

static RClass*
outer_class(mrb_state *mrb, RClass *c)
{
  mrb_value ov;

  ov = mrb_obj_iv_get(mrb, (RObject*)c, mrb_intern_lit(mrb, "__outer__"));
  if (mrb_nil_p(ov)) return NULL;
  switch (mrb_type(ov)) {
  case MRB_TT_CLASS:
  case MRB_TT_MODULE:
    return mrb_class_ptr(ov);
  default:
    break;
  }
  return NULL;
}

static mrb_bool
detect_outer_loop(mrb_state *mrb, RClass *c)
{
  RClass *t = c;         /* tortoise */
  RClass *h = c;         /* hare */

  for (;;) {
    if (h == NULL) return FALSE;
    h = outer_class(mrb, h);
    if (h == NULL) return FALSE;
    h = outer_class(mrb, h);
    t = outer_class(mrb, t);
    if (t == h) return TRUE;
  }
}

mrb_value
mrb_class_find_path(mrb_state *mrb, RClass *c)
{
  RClass *outer;
  mrb_value path;
  mrb_sym name;
  const char *str;
  mrb_int len;

  if (detect_outer_loop(mrb, c)) return mrb_nil_value();
  outer = outer_class(mrb, c);
  if (outer == NULL) return mrb_nil_value();
  name = find_class_sym(mrb, outer, c);
  if (name == 0) return mrb_nil_value();
  str = mrb_class_name(mrb, outer);
  path = mrb_str_new_capa(mrb, 40);
  mrb_str_cat_cstr(mrb, path, str);
  mrb_str_cat_cstr(mrb, path, "::");

  str = mrb_sym2name_len(mrb, name, &len);
  mrb_str_cat(mrb, path, str, len);
  iv_del(mrb, c, mrb_intern_lit(mrb, "__outer__"), NULL);
  iv_put(mrb, c, mrb_intern_lit(mrb, "__classname__"), path);
  // mrb_field_write_barrier_value(mrb, (RBasic*)c, path);
  return path;
}
