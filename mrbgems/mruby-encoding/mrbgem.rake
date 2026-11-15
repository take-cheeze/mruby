MRuby::Gem::Specification.new('mruby-encoding') do |spec|
  spec.license = 'MIT'
  spec.author  = 'mruby developers'
  spec.summary = "Poorman's Encoding for mruby"
  spec.cc.defines << 'MRB_UTF8_STRING'
  spec.add_test_dependency 'mruby-string-ext'
end
