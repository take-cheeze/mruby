MRuby::Gem::Specification.new('mruby-thread') do |spec|
  spec.license = 'MIT'
  spec.author  = 'mruby developers'
  spec.summary = 'Thread on libuv threads, with a global lock and per-thread VM state (see src/thread.c)'
  spec.add_dependency 'mruby-fiber', core: 'mruby-fiber'
  spec.linker.libraries << 'uv' << 'pthread'
end
