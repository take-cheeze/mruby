module MRuby
  class Presym
    OPERATORS = {
      "!" => "not",
      "%" => "mod",
      "&" => "and",
      "*" => "mul",
      "+" => "add",
      "-" => "sub",
      "/" => "div",
      "<" => "lt",
      ">" => "gt",
      "^" => "xor",
      "`" => "tick",
      "|" => "or",
      "~" => "neg",
      "!=" => "neq",
      "!~" => "nmatch",
      "&&" => "andand",
      "**" => "pow",
      "+@" => "plus",
      "-@" => "minus",
      "<<" => "lshift",
      "<=" => "le",
      "==" => "eq",
      "=~" => "match",
      ">=" => "ge",
      ">>" => "rshift",
      "[]" => "aref",
      "||" => "oror",
      "<=>" => "cmp",
      "===" => "eqq",
      "[]=" => "aset",
    }.freeze

    SYMBOL_TO_MACRO = {
    #      Symbol      =>      Macro
    # [prefix, suffix] => [prefix, suffix]
      ["$"   , ""    ] => ["GV"  , ""    ],
      ["@@"  , ""    ] => ["CV"  , ""    ],
      ["@"   , ""    ] => ["IV"  , ""    ],
      [""    , "!"   ] => [""    , "_B"  ],
      [""    , "?"   ] => [""    , "_Q"  ],
      [""    , "="   ] => [""    , "_E"  ],
      [""    , ""    ] => [""    , ""    ],
    }.freeze

    C_STR_LITERAL_RE = /"(?:[^\\\"]|\\.)*"/

    ESCAPE_SEQUENCE_MAP = {
      "a" => "\a",
      "b" => "\b",
      "e" => "\e",
      "f" => "\f",
      "n" => "\n",
      "r" => "\r",
      "t" => "\t",
      "v" => "\v",
    }
    ESCAPE_SEQUENCE_MAP.keys.each { |k| ESCAPE_SEQUENCE_MAP[ESCAPE_SEQUENCE_MAP[k]] = k }

    def initialize(build)
      @build = build
    end

    def scan(paths)
      presym_hash = {}
      paths.each {|path| read_preprocessed(presym_hash, path)}
      presym_hash.keys.sort_by!{|sym| [sym.bytesize, sym]}
    end

    def read_list
      File.readlines(list_path, mode: "r:binary").each(&:chomp!)
    end

    def write_list(presyms)
      _pp "GEN", list_path.relative_path
      File.binwrite(list_path, presyms.join("\n") << "\n")
    end

    def write_id_header(presyms)
      prefix_re = Regexp.union(*SYMBOL_TO_MACRO.keys.map(&:first).uniq)
      suffix_re = Regexp.union(*SYMBOL_TO_MACRO.keys.map(&:last).uniq)
      sym_re = /\A(#{prefix_re})?([\w&&\D]\w*)(#{suffix_re})?\z/o
      _pp "GEN", id_header_path.relative_path
      File.open(id_header_path, "w:binary") do |f|
        f.puts "enum mruby_presym {"
        presyms.each.with_index(1) do |sym, num|
          if sym_re =~ sym && (affixes = SYMBOL_TO_MACRO[[$1, $3]])
            f.puts "  MRB_#{affixes * 'SYM'}__#{$2} = #{num},"
          elsif name = OPERATORS[sym]
            f.puts "  MRB_OPSYM__#{name} = #{num},"
          end
        end
        f.puts "};"
        f.puts
        f.puts "#define MRB_PRESYM_MAX #{presyms.size}"
      end
    end

    # Presyms are sorted by [bytesize, name] (see #scan), so every name of one
    # length sits in one run.  The table stores them as a single blob of
    # NUL-terminated names plus, per length, the index and blob offset of that
    # run's first name: no per-symbol pointer or length entry.
    def write_table_header(presyms)
      _pp "GEN", table_header_path.relative_path
      len_max = presyms.empty? ? 0 : presyms.last.bytesize
      counts = Array.new(len_max + 1, 0)
      presyms.each {|sym| counts[sym.bytesize] += 1}
      starts = [0]
      offsets = [0]
      counts.each_with_index do |n, len|
        starts << starts.last + n
        offsets << offsets.last + n * (len + 1)
      end
      offsets.pop
      File.open(table_header_path, "w:binary") do |f|
        f.puts "#define MRB_PRESYM_LEN_MAX #{len_max}"
        f.puts
        f.puts "static const uint16_t presym_len_start[] = {"
        starts.each_with_index {|s, len| f.puts "  #{s},\t/* #{len} */"}
        f.puts "};"
        f.puts
        f.puts "static const uint32_t presym_len_offset[] = {"
        offsets.each_with_index {|o, len| f.puts "  #{o},\t/* #{len} */"}
        f.puts "};"
        f.puts
        f.puts "static const char presym_name_blob[] ="
        presyms.each do |sym|
          sym = sym.gsub(/([\x01-\x1f\x7f-\xff])|("|\\)/n) {
            case
            when $1
              e = ESCAPE_SEQUENCE_MAP[$1]
              e ? "\\#{e}" : '\\x%02x""' % $1.ord
            when $2
              "\\#$2"
            end
          }
          f.puts %|  "#{sym}\\0"|
        end
        f.puts "  \"\";"
      end
    end

    def list_path
      @list_path ||= "#{@build.build_dir}/presym".freeze
    end

    def header_dir
      @header_dir ||= "#{@build.build_dir}/include/mruby/presym".freeze
    end

    def id_header_path
      @id_header_path ||= "#{header_dir}/id.h".freeze
    end

    def table_header_path
      @table_header_path ||= "#{header_dir}/table.h".freeze
    end

    private

    def read_preprocessed(presym_hash, path)
      File.binread(path).scan(/<@! (.*?) !@>/) do |part,|
        literals = part.scan(C_STR_LITERAL_RE)
        unless literals.empty?
          literals = literals.map{|l| l[1..-2]}
          literals.each do |e|
            e.gsub!(/\\x([0-9A-Fa-f]{1,2})|\\(0[0-7]{,3})|\\([abefnrtv])|\\(.)/) do
              case
              when $1; $1.hex.chr(Encoding::BINARY)
              when $2; $2.oct.chr(Encoding::BINARY)
              when $3; ESCAPE_SEQUENCE_MAP[$3]
              when $4; $4
              end
            end
          end
          presym_hash[literals.join] = true
        end
      end
    end

    def c_literal_size(literal_without_quote)
      literal_without_quote.size  # TODO: consider escape sequence
    end
  end
end
