# Module#{public,private,protected}_method_defined?: the method is found (inherited ones too) and has that visibility
assert('Module#public_method_defined?') do
  c = Class.new do
    def pub; end
    private def priv; end
    protected def prot; end
  end
  assert_true c.public_method_defined?(:pub)
  assert_false c.public_method_defined?(:priv)
  assert_false c.public_method_defined?(:prot)
  assert_false c.public_method_defined?(:nothing)
end

assert('Module#private_method_defined?') do
  c = Class.new do
    private def priv; end
    def pub; end
  end
  assert_true c.private_method_defined?(:priv)
  assert_false c.private_method_defined?(:pub)
  sub = Class.new(c)
  assert_true sub.private_method_defined?(:priv)
  assert_false sub.private_method_defined?(:nothing)
end

assert('Module#protected_method_defined?') do
  c = Class.new do
    protected def prot; end
  end
  assert_true c.protected_method_defined?(:prot)
  assert_false c.protected_method_defined?(:nothing)
end
