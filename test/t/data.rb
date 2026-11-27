##
# Data type check with parent types (mrb_data_type.parent)

assert('mrb_data_check_type: exact type matches') do
  assert_true TestDataType.check_parent(TestDataType.new_parent)
end

assert('mrb_data_check_type: child type matches its parent') do
  assert_true TestDataType.check_parent(TestDataType.new_child)
end

assert('mrb_data_check_type: parent object does not match child type') do
  assert_raise_with_message(TypeError, "wrong argument type TestDTParent (expected TestDTChild)") do
    TestDataType.check_child(TestDataType.new_parent)
  end
end

assert('mrb_data_check_type: unrelated type raises TypeError') do
  assert_raise_with_message(TypeError, "wrong argument type TestDTOther (expected TestDTParent)") do
    TestDataType.check_parent(TestDataType.new_other)
  end
end

assert('mrb_data_check_get_ptr: child type matches its parent') do
  assert_true TestDataType.get_parent?(TestDataType.new_child)
end

assert('mrb_data_check_get_ptr: unrelated type returns NULL') do
  assert_false TestDataType.get_parent?(TestDataType.new_other)
end

assert('mrb_data_check_get_ptr: non-data object returns NULL') do
  assert_false TestDataType.get_parent?(Object.new)
end
