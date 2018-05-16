MRuby::Build.new do |conf|
  toolchain :clang

  enable_debug
  enable_test

  cc.flags << '-Wno-c11-extensions' << '-Wno-missing-field-initializers'

  luajit_dir = "#{build_dir}/LuaJIT"
  luajit_lib = libfile("#{luajit_dir}/src/libluajit")

  conf.libmruby << luajit_lib
  conf.libmruby_core << luajit_lib

  file luajit_lib => [luajit_dir, __FILE__] do |_|
    # sh "make -C #{luajit_dir}/src CC='#{cc.command}' CFLAGS='#{cc.flags.flatten.join(' ')}' LDFLAGS='#{linker.flags.flatten.join(' ')}' -j2 libluajit.a"
    sh "make -C #{luajit_dir}/src -j2 CFLAGS='-g3' libluajit.a"
  end

  file luajit_dir => [__FILE__] do |t|
    if Dir.exist? t.name
      sh "cd '#{t.name}' && git pull origin master"
    else
      sh "git clone https://github.com/LuaJIT/LuaJIT.git '#{t.name}'"
    end
  end

  [cc, cxx, linker].each do |c|
    c.include_paths << "#{luajit_dir}/src" if c.respond_to? :include_paths
    c.flags << "-ferror-limit=0"
    c.flags << '-fsanitize=address,undefined' # leak
  end
end
