MRuby::Gem::Specification.new('mruby-bigint') do |spec|
  spec.license = 'MIT'
  spec.author  = 'mruby developers'
  spec.summary = 'Integer class extension to multiple-precision'
  # MRB_USE_BIGINT is for the whole build: the core's Integer operators and the gem read the same configuration
  build.cc.defines << 'MRB_USE_BIGINT'
  spec.cc.defines << 'MRB_USE_BIGINT'

  # the bigint sources are in core/ and are linked into libmruby with the core (as mruby-compiler's are)
  current_dir = spec.dir
  current_build_dir = spec.build_dir
  core_objs = Dir.glob("#{current_dir}/core/*.c").map { |f|
    objfile(f.pathmap("#{current_build_dir}/core/%n"))
  }
  file build.libmruby_core_static => core_objs
  build.libmruby << core_objs
end
