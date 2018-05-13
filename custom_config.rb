MRuby::Build.new do |conf|
  toolchain :clang

  enable_debug
  enable_test

  cc.flags << '-Wno-c11-extensions' << '-Wno-missing-field-initializers'

  luajit_dir = "#{build_dir}/LuaJIT"
  luajit_lib = libfile("#{luajit_dir}/src/libluajit")

  conf.libmruby << luajit_lib
  conf.libmruby_core << luajit_lib

  file luajit_lib => luajit_dir do |_|
    sh "make -C #{luajit_dir} src/libluajit.a"
  end

  file luajit_dir do |t|
    sh "git clone https://github.com/LuaJIT/LuaJIT.git '#{t.name}'"
  end

  [cc, cxx, linker].each do |c|
    c.include_paths << "#{luajit_dir}/src" if c.respond_to? :include_paths
    c.flags << "-ferror-limit=0"
    c.flags << '-fsanitize=leak,address,undefined'
  end
end
