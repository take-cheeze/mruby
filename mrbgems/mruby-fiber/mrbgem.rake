MRuby::Gem::Specification.new('mruby-fiber') do |spec|
  spec.license = 'MIT'
  spec.author  = 'mruby developers'
  spec.summary = 'Fiber class'

  cc.include_paths << "#{MRUBY_ROOT}/src"
end
