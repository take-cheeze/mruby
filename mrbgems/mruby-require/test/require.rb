# require: a provided feature returns false, any other feature raises LoadError.
# A gem's test state loads only its dependencies, so the features the build provides (the generated table) are
# registered here by hand; the table itself is checked by running a full build (see tasks/mrbgems.rake).
assert('require of a provided feature returns false') do
  $LOADED_FEATURES = ["stringio"]
  assert_false require('stringio')
end

assert('require of an unknown feature raises LoadError') do
  assert_raise(LoadError) { require 'no_such_feature_xyz' }
  assert_kind_of ScriptError, LoadError.new
end
