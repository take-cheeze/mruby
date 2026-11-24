assert('ENV[] and []= round-trip') do
  ENV["MRUBY_ENV_TEST"] = "abc"
  assert_equal "abc", ENV["MRUBY_ENV_TEST"]
  ENV["MRUBY_ENV_TEST"] = nil
  assert_nil ENV["MRUBY_ENV_TEST"]
end

assert('ENV.fetch raises KeyError without default') do
  ENV.delete("MRUBY_ENV_MISSING")
  assert_raise(KeyError) { ENV.fetch("MRUBY_ENV_MISSING") }
  assert_equal "d", ENV.fetch("MRUBY_ENV_MISSING", "d")
end

assert('ENV.key? and delete') do
  ENV["MRUBY_ENV_TEST2"] = "x"
  assert_true ENV.key?("MRUBY_ENV_TEST2")
  assert_equal "x", ENV.delete("MRUBY_ENV_TEST2")
  assert_false ENV.has_key?("MRUBY_ENV_TEST2")
end

assert('ENV rejects a non-String key') do
  assert_raise(TypeError) { ENV[1] }
end

assert('ENV.to_h includes set variables') do
  ENV["MRUBY_ENV_TEST3"] = "v"
  assert_equal "v", ENV.to_h["MRUBY_ENV_TEST3"]
  ENV.delete("MRUBY_ENV_TEST3")
end
