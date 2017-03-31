MRuby::Gem::Specification.new 'mruby-rbconfig' do |spec|
  spec.license = 'MIT'
  spec.author  = 'mruby developers'
  spec.summary = 'RbConfig for mruby'

  rbconfig = "#{build_dir}/rbconfig"
  rbconfig_src = "#{rbconfig}.c"

  spec.build.libmruby << objfile(rbconfig)
  file objfile(rbconfig) => rbconfig_src
  file rbconfig_src => [MRUBY_CONFIG, __FILE__] do |t|
    _pp 'GEN', t.name

    configs = {
      'host_os' => `uname -sr`.downcase.gsub(/\s+/, ''),
      'ruby_install_name' => 'mruby',
      'bindir' => "#{build.build_dir}/bin",
    }
    ruby_plat = "#{`uname -m`}-#{`uname -s`}".downcase.gsub(/\s+/, '')

    out = ''
    out << <<EOS
#include <mruby.h>
#include <mruby/hash.h>

mrb_value
 mrb_rbconfig_hash(mrb_state *mrb)
{
  mrb_value ret = mrb_hash_new(mrb);

EOS

    configs.each do |k,v|
      out << %Q[mrb_hash_set(mrb, ret, mrb_str_new_lit(mrb, "#{k}"), mrb_str_new_lit(mrb, "#{v}"));\n]
    end

    out << <<EOS

  return ret;
}

void
mrb_rbconfig_define_const(mrb_state *mrb)
{
  mrb_define_global_const(mrb, "RUBY_PLATFORM", mrb_str_new_lit(mrb, "#{ruby_plat}"));
}
EOS

    FileUtils.mkdir_p File.dirname t.name
    File.open(t.name, 'w') {|f| f.write out }
  end
end
