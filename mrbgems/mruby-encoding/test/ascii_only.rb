assert('String#ascii_only?') do
  assert_true "abc".ascii_only?
  assert_true "".ascii_only?
  assert_false "h\u00e9".ascii_only?
  assert_false "\x80".ascii_only?
end
