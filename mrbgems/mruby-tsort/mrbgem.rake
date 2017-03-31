MRuby::Gem::Specification.new 'mruby-tsort' do |spec|
  spec.license = 'MIT'
  spec.author  = 'ruby/mruby developers'
  spec.summary = 'tsort port from CRuby'

  add_dependency 'mruby-kernel-ext', core: 'mruby-kernel-ext'
  add_dependency 'mruby-array-ext', core: 'mruby-array-ext'
  add_dependency 'mruby-symbol-ext', core: 'mruby-symbol-ext'
  add_dependency 'mruby-enum-lazy', core: 'mruby-enum-lazy'
  add_dependency 'mruby-method', mgem: 'mruby-method'
end
