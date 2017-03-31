class File
  class << self
    alias absolute_path expand_path
  end
end
