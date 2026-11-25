assert('at_exit requires a block') do
  assert_raise(ArgumentError) { at_exit }
end

assert('at_exit returns its block') do
  b = Proc.new { }
  assert_equal b, at_exit(&b)
end
