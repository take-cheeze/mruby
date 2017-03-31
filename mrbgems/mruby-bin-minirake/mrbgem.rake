MRuby::Gem::Specification.new 'mruby-bin-minirake' do |spec|
  spec.license = 'MIT'
  spec.author  = 'mruby developers'
  spec.summary = 'minirake executable'
  spec.bins = %w(minirake)

  add_dependency 'mruby-mrbgem-require', core: 'mruby-mrbgem-require'
  add_dependency 'mruby-exit', core: 'mruby-exit'
  add_dependency 'mruby-tsort', core: 'mruby-tsort'
  add_dependency 'mruby-pathname', core: 'mruby-pathname'
  add_dependency 'mruby-rbconfig', core: 'mruby-rbconfig'
  add_dependency 'mruby-bin-mruby', core: 'mruby-bin-mruby'
  add_dependency 'mruby-object-ext', core: 'mruby-object-ext'
  add_dependency 'mruby-hash-ext', core: 'mruby-hash-ext'

  add_dependency 'mruby-getoptlong', mgem: 'mruby-getoptlong'
  add_dependency 'mruby-forwardable', mgem: 'mruby-forwardable'
  add_dependency 'mruby-shellwords', mgem: 'mruby-shellwords'
  add_dependency 'mruby-yaml', mgem: 'mruby-yaml'
  add_dependency 'mruby-marshal', mgem: 'mruby-marshal'
  add_dependency 'mruby-process', mgem: 'mruby-process'
  add_dependency 'mruby-io', mgem: 'mruby-io'
  add_dependency 'mruby-env', mgem: 'mruby-env'
  add_dependency 'mruby-onig-regexp', mgem: 'mruby-onig-regexp'
  add_dependency 'mruby-dir-glob', mgem: 'mruby-dir-glob'
  add_dependency 'mruby-open3', mgem: 'mruby-open3'

  add_dependency 'mruby-fileutils', github: 'hfm/mruby-fileutils'

  minirake_gem_list = "#{dir}/.dep_gems.txt"

  task minirake_gem_list do |t|
    _pp "GEN", t.name
    open t.name, 'w' do |f|
      gem_table = build.gems.generate_gem_table build
      dep_list = build.gems.tsort_dependencies([spec.name], gem_table).select(&:generate_functions)
      f.write dep_list.map(&:name).join "\n"
    end
  end

  file "#{dir}/tools/minirake/minirake.rb".relative_path_from(MRUBY_ROOT) => minirake_gem_list
end
