module Kernel
  alias iij_require require

  def mrbgem_require mod
    prefix = 'mruby-'
    mod = mod[prefix.length, -1] if mod.start_with? prefix
    raise LoadError, "cannot find mrbgem: #{mod}" unless MRBGEMS.any? {|v| v == mod }
  end

  def require mod
    exp = nil

    begin
      mrbgem_require mod
    rescue LoadError => e
      exp = e
    end

    if e
      iij_require mod
    end
  end

  module_function :mrbgem_require, :require
end
