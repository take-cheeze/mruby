/*
** data_type.c - test helpers for data type parent checks
**
** See Copyright Notice in mruby.h
*/

#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>

static int test_dt_value;

static const mrb_data_type test_dt_parent = { "TestDTParent", NULL, NULL, NULL };
static const mrb_data_type test_dt_child = { "TestDTChild", NULL, NULL, &test_dt_parent };
static const mrb_data_type test_dt_other = { "TestDTOther", NULL, NULL, NULL };

static mrb_value
dt_new(mrb_state *mrb, const mrb_data_type *type)
{
  return mrb_obj_value(mrb_data_object_alloc(mrb, mrb->object_class, &test_dt_value, type));
}

static mrb_value
dt_s_new_parent(mrb_state *mrb, mrb_value self)
{
  return dt_new(mrb, &test_dt_parent);
}

static mrb_value
dt_s_new_child(mrb_state *mrb, mrb_value self)
{
  return dt_new(mrb, &test_dt_child);
}

static mrb_value
dt_s_new_other(mrb_state *mrb, mrb_value self)
{
  return dt_new(mrb, &test_dt_other);
}

/* raises TypeError unless obj is a TestDTParent or a child of it */
static mrb_value
dt_s_check_parent(mrb_state *mrb, mrb_value self)
{
  mrb_value obj;

  mrb_get_args(mrb, "o", &obj);

  mrb_data_check_type(mrb, obj, &test_dt_parent);
  return mrb_true_value();
}

/* raises TypeError unless obj is a TestDTChild (a parent object does not match) */
static mrb_value
dt_s_check_child(mrb_state *mrb, mrb_value self)
{
  mrb_value obj;

  mrb_get_args(mrb, "o", &obj);

  mrb_data_check_type(mrb, obj, &test_dt_child);
  return mrb_true_value();
}

/* true when mrb_data_check_get_ptr accepts obj for TestDTParent */
static mrb_value
dt_s_get_parent_p(mrb_state *mrb, mrb_value self)
{
  mrb_value obj;

  mrb_get_args(mrb, "o", &obj);

  return mrb_bool_value(mrb_data_check_get_ptr(mrb, obj, &test_dt_parent) != NULL);
}

void
mrb_init_test_data_type(mrb_state *mrb)
{
  struct RClass *dt;

  dt = mrb_define_module(mrb, "TestDataType");
  mrb_define_class_method(mrb, dt, "new_parent", dt_s_new_parent, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, dt, "new_child", dt_s_new_child, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, dt, "new_other", dt_s_new_other, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, dt, "check_parent", dt_s_check_parent, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, dt, "check_child", dt_s_check_child, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, dt, "get_parent?", dt_s_get_parent_p, MRB_ARGS_REQ(1));
}
