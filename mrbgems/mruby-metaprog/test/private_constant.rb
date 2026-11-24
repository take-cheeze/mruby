# Module#private_constant: hidden from constants, refused by a scoped reference, still read by const_get (CRuby)
assert('Module#private_constant') do
  c = Class.new
  c.const_set(:PRIV, 1)
  c.const_set(:PUB, 2)
  c.__send__(:private_constant, :PRIV)
  assert_equal 2, c::PUB
  assert_true c.constants.include?(:PUB)
  assert_false c.constants.include?(:PRIV)
  assert_raise(NameError) { c::PRIV }
  assert_equal 1, c.const_get(:PRIV)
  assert_raise(NameError) { Class.new.__send__(:private_constant, :NOPE) }
end
