MRuby::Gem::Specification.new 'mruby-pathname' do |spec|
  spec.license = 'MIT'
  spec.author  = 'ruby/mruby developers'
  spec.summary = 'pathname port from CRuby'

  add_dependency 'mruby-mrbgem-require', core: 'mruby-mrbgem-require'
  add_dependency 'mruby-numeric-ext', core: 'mruby-numeric-ext'
  add_dependency 'mruby-proc-ext', core: 'mruby-proc-ext'

  add_dependency 'mruby-io', mgem: 'mruby-io'
  add_dependency 'mruby-onig-regexp', mgem: 'mruby-onig-regexp'

  add_dependency 'mruby-fileutils', github: 'mrbgems/mruby-fileutils', branch: 'minirake'

  add_test_dependency 'mruby-tempfile', mgem: 'mruby-tempfile'
  add_test_dependency 'mruby-kernel-ext', core: 'mruby-kernel-ext'
  add_test_dependency 'mruby-mtest', mgem: 'mruby-mtest'
end
