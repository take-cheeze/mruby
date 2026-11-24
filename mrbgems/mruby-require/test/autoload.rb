assert('Module#autoload? returns the path until the constant is loaded') do
  m = Module.new
  m.autoload(:Missing, "no_such_file_zz")
  assert_equal "no_such_file_zz", m.autoload?(:Missing)
  assert_nil m.autoload?(:Other)
end

assert('a missing autoload raises LoadError and keeps the path') do
  m = Module.new
  m.autoload(:Missing, "no_such_file_zz")
  assert_raise(LoadError) { m::Missing }
  assert_equal "no_such_file_zz", m.autoload?(:Missing)
end

assert('an autoload whose require does not define the constant raises NameError') do
  # the feature is provided (see require.rb): its require succeeds but defines no Stringy
  $LOADED_FEATURES = ["stringio"]
  m = Module.new
  m.autoload(:Stringy, "stringio")
  assert_raise(NameError) { m::Stringy }
  assert_nil m.autoload?(:Stringy)
end
