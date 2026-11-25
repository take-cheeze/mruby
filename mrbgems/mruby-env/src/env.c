/*
** mruby-env - ENV: the process environment
**
** A CRuby-compatible subset: [], []=, fetch, key?/has_key?/include?, delete,
** to_h. Values are Strings; assigning nil deletes. Keys and values may not
** contain NUL (ArgumentError), as in CRuby.
**
** [ MIT license: https://www.opensource.org/licenses/mit-license.php ]
*/

#include <stdlib.h>
#include <string.h>
#include <mruby.h>
#include <mruby/array.h>
#include <mruby/hash.h>
#include <mruby/string.h>
#include <mruby/class.h>
#include <mruby/error.h>

#ifdef _WIN32
#include <stdlib.h>
#else
extern char **environ;
#endif

static const char *
env_key(mrb_state *mrb, mrb_value key)
{
  const char *p;
  mrb_int len;
  if (!mrb_string_p(key)) {
    mrb_raisef(mrb, E_TYPE_ERROR, "no implicit conversion of %S into String", mrb_obj_value(mrb_obj_class(mrb, key)));
  }
  len = RSTRING_LEN(key);
  p = RSTRING_PTR(key);
  if (len == 0 || memchr(p, '\0', (size_t)len) || memchr(p, '=', (size_t)len)) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "bad environment variable name");
  }
  return mrb_str_to_cstr(mrb, key);
}

static mrb_value
env_aref(mrb_state *mrb, mrb_value self)
{
  mrb_value key;
  const char *v;
  mrb_get_args(mrb, "S", &key);
  v = getenv(env_key(mrb, key));
  return v ? mrb_str_new_cstr(mrb, v) : mrb_nil_value();
}

static mrb_value
env_aset(mrb_state *mrb, mrb_value self)
{
  mrb_value key, val;
  const char *k;
  mrb_get_args(mrb, "So", &key, &val);
  k = env_key(mrb, key);
  if (mrb_nil_p(val)) {
#ifdef _WIN32
    _putenv_s(k, "");
#else
    unsetenv(k);
#endif
    return val;
  }
  if (!mrb_string_p(val)) {
    mrb_raisef(mrb, E_TYPE_ERROR, "no implicit conversion of %S into String", mrb_obj_value(mrb_obj_class(mrb, val)));
  }
  if (memchr(RSTRING_PTR(val), '\0', (size_t)RSTRING_LEN(val))) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "bad environment variable value");
  }
#ifdef _WIN32
  _putenv_s(k, mrb_str_to_cstr(mrb, val));
#else
  if (setenv(k, mrb_str_to_cstr(mrb, val), 1) != 0) {
    mrb_sys_fail(mrb, "setenv");
  }
#endif
  return val;
}

static mrb_value
env_fetch(mrb_state *mrb, mrb_value self)
{
  mrb_value key, def = mrb_undef_value();
  const char *v;
  mrb_get_args(mrb, "S|o", &key, &def);
  v = getenv(env_key(mrb, key));
  if (v) return mrb_str_new_cstr(mrb, v);
  if (!mrb_undef_p(def)) return def;
  mrb_raisef(mrb, E_KEY_ERROR, "key not found: %S", key);
  return mrb_nil_value();
}

static mrb_value
env_has_key(mrb_state *mrb, mrb_value self)
{
  mrb_value key;
  mrb_get_args(mrb, "S", &key);
  return mrb_bool_value(getenv(env_key(mrb, key)) != NULL);
}

static mrb_value
env_delete(mrb_state *mrb, mrb_value self)
{
  mrb_value key;
  const char *k, *v;
  mrb_get_args(mrb, "S", &key);
  k = env_key(mrb, key);
  v = getenv(k);
  if (!v) return mrb_nil_value();
  {
    mrb_value old = mrb_str_new_cstr(mrb, v);
#ifdef _WIN32
    _putenv_s(k, "");
#else
    unsetenv(k);
#endif
    return old;
  }
}

static mrb_value
env_to_h(mrb_state *mrb, mrb_value self)
{
  mrb_value h = mrb_hash_new(mrb);
#ifndef _WIN32
  char **e;
  for (e = environ; e && *e; e++) {
    const char *eq = strchr(*e, '=');
    if (!eq) continue;
    mrb_hash_set(mrb,
      h,
      mrb_str_new(mrb, *e, (mrb_int)(eq - *e)),
      mrb_str_new_cstr(mrb, eq + 1));
  }
#endif
  return h;
}

void
mrb_mruby_env_gem_init(mrb_state *mrb)
{
  struct RClass *env_sc;
  mrb_value env = mrb_obj_new(mrb, mrb->object_class, 0, NULL);
  env_sc = mrb_singleton_class_ptr(mrb, env);

  mrb_define_method(mrb, env_sc, "[]", env_aref, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, env_sc, "[]=", env_aset, MRB_ARGS_REQ(2));
  mrb_define_method(mrb, env_sc, "store", env_aset, MRB_ARGS_REQ(2));
  mrb_define_method(mrb, env_sc, "fetch", env_fetch, MRB_ARGS_ARG(1, 1));
  mrb_define_method(mrb, env_sc, "key?", env_has_key, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, env_sc, "has_key?", env_has_key, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, env_sc, "include?", env_has_key, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, env_sc, "delete", env_delete, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, env_sc, "to_h", env_to_h, MRB_ARGS_NONE());

  mrb_define_const(mrb, mrb->object_class, "ENV", env);
}

void
mrb_mruby_env_gem_final(mrb_state *mrb)
{
}
