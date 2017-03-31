assert 'check MRBGEMS' do
  assert_true Object.const_defined? :MRBGEMS
end

assert 'check mrbgem-require in list' do
  assert_true MRBGEMS.any? {|v| v == 'mrbgem-require' }
end

assert 'check LoadError class' do
  Object.const_defined? :LoadError
  LoadError.superclass == ScriptError
end

assert 'check require' do
  require 'mrbgem-require'
  assert_raise(LoadError) { require 'mrbgem-unrequirable' }
end
