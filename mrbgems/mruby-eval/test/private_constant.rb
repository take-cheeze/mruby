# a lexical reference to a private constant, from a method defined by a string class_eval (the lexical scope is c, as in CRuby)
assert('Module#private_constant: lexical reference from class_eval string') do
  c = Class.new
  c.const_set(:PRIV, 1)
  c.class_eval "private_constant :PRIV; def self.lex; PRIV; end"
  assert_equal 1, c.lex
  assert_raise(NameError) { c::PRIV }
end
