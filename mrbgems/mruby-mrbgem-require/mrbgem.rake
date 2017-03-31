MRuby::Gem::Specification.new 'mruby-mrbgem-require' do |spec|
  spec.license = 'MIT'
  spec.author  = 'mruby developers'
  spec.summary = 'require method only for mrbgem'

  add_dependency 'mruby-string-ext', core: 'mruby-string-ext'
  add_dependency 'mruby-require', github: 'iij/mruby-require'

  gem_list = "#{build_dir}/gem_list"
  gem_list_src = "#{gem_list}.c"

  spec.build.libmruby << objfile(gem_list)
  file objfile(gem_list) => gem_list_src
  file gem_list_src => [MRUBY_CONFIG, __FILE__] do |t|
    _pp 'GEN', t.name
    out = ''
    out << <<EOS
#include <mruby.h>
#include <mruby/array.h>

mrb_value mrbgem_require_gems(mrb_state *mrb) {
  mrb_value ary = mrb_ary_new_capa(mrb, #{spec.build.gems.size});

EOS
    spec.build.gems.each do |g|
      # remove "mruby-" prefix
      name = g.name.sub(/^mruby-/, '')

      out << <<EOS
  mrb_ary_push(mrb, ary, mrb_str_new_lit(mrb, "#{name}"));
EOS
    end

    out << <<EOS

  return ary;
}
EOS

    FileUtils.mkdir_p File.dirname t.name
    File.open(t.name, 'w') {|f| f.write out }
  end
end
