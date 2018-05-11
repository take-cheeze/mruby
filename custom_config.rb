MRuby::Build.new do |conf|
  toolchain :clang

  enable_debug
  enable_test

  [cc, cxx, linker].each do |c|
    c.include_paths << "#{MRUBY_ROOT}/LuaJIT/src" if c.respond_to? :include_paths
    c.flags << "-ferror-limit=0"
    c.flags << '-fsanitize=leak,address,undefined'
  end
end
