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
      presyms = presym_hash.keys
      compact?(presyms) ? presyms.sort! : presyms.sort_by!{|sym| [sym.bytesize, sym]}
    end

    # PRESYM_COMPACT (MRuby::Build#presym_compact): the names in byte order, front-coded in blocks and pair-coded
    # (#write_compact_table_header). A header byte is 0xc0|prefix (at most 63 shared bytes) and a code is below 0xc0, so a
    # name must be bytes of 0x01..0x7f; presym_sym2name keys its decoded-name cache with 16 bits.
    COMPACT_BLOCK_SHIFT = 4
    COMPACT_HEADER = 0xc0
    # in the header's first line (#table_header_current?): bump it when the layout changes
    COMPACT_FORMAT = 1

    def compact?(presyms)
      @build.respond_to?(:presym_compact) && @build.presym_compact &&
        !presyms.empty? && presyms.size < 0x10000 &&
        presyms.all? {|sym| sym.each_byte.all? {|b| b > 0 && b < 0x80}}
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
      return write_compact_table_header(presyms) if compact?(presyms)

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
        f.puts table_mode_line(presyms)
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

    # Whether the table header was written in the format #write_table_header now picks: a list the scan leaves
    # unchanged still needs new headers when presym_compact changed.
    def table_header_current?(presyms)
      File.exist?(table_header_path) && File.open(table_header_path, &:gets).to_s.chomp == table_mode_line(presyms)
    end

    def table_mode_line(presyms)
      "/* presym table: #{compact?(presyms) ? "compact #{COMPACT_FORMAT}" : 'plain'} */"
    end

    # Per block of 2**COMPACT_BLOCK_SHIFT names: each name is a header byte (COMPACT_HEADER | the length of the
    # prefix it shares with the previous name of its block, 0 for the first) and the codes of the rest. A code below
    # the number of distinct characters is one character; a higher one stands for a pair of codes, merged greedily by
    # frequency and stored expanded (presym_pair_text). A trailing header byte ends the last name. presym_first_block
    # narrows presym_find's search to the blocks starting with the name's first byte.
    def write_compact_table_header(presyms)
      _pp "GEN", table_header_path.relative_path
      block = 1 << COMPACT_BLOCK_SHIFT
      tails = []
      presyms.each_slice(block) do |names|
        prev = ""
        names.each do |name|
          pre = 0
          pre += 1 while pre < 63 && pre < prev.bytesize && pre < name.bytesize && prev.getbyte(pre) == name.getbyte(pre)
          tails << [pre, name.byteslice(pre..-1).bytes]
          prev = name
        end
      end
      chars = tails.flat_map(&:last).uniq.sort
      code_of = chars.each_with_index.to_h
      tails.each {|t| t[1].map! {|b| code_of[b]}}
      pairs = []
      text_size = Array.new(chars.size, 1)
      while chars.size + pairs.size < COMPACT_HEADER
        counts = Hash.new(0)
        tails.each {|_, codes| codes.each_cons(2) {|pair| counts[pair] += 1}}
        best, count = counts.min_by {|pair, n| [-n, pair]}
        # each use saves a byte; the pair costs its presym_pair_end entry and its text
        break if best.nil? || count <= 2 + text_size[best[0]] + text_size[best[1]]
        code = chars.size + pairs.size
        pairs << best
        text_size << text_size[best[0]] + text_size[best[1]]
        tails.each do |t|
          codes = t[1]
          out = []
          i = 0
          while i < codes.size
            if i + 1 < codes.size && codes[i] == best[0] && codes[i+1] == best[1]
              out << code
              i += 2
            else
              out << codes[i]
              i += 1
            end
          end
          t[1] = out
        end
      end
      bytes = []
      offsets = []
      tails.each_with_index do |(pre, codes), idx|
        offsets << bytes.size if idx % block == 0
        bytes << (COMPACT_HEADER | pre)
        bytes.concat(codes)
      end
      bytes << COMPACT_HEADER
      compact_verify(presyms, chars, pairs, offsets, bytes)
      offset_type = bytes.size < 0x10000 ? "uint16_t" : "uint32_t"
      expand = ->(code) { code < chars.size ? [chars[code]] : pairs[code - chars.size].flat_map(&expand) }
      pair_text = pairs.each_index.map {|i| expand.(chars.size + i)}
      pair_end = pair_text.each_with_object([]) {|t, ends| ends << (ends.last || 0) + t.size}
      # first_block[b]: the blocks whose first name starts below byte b
      first_block = (0..0x80).map {|b| offsets.each_index.count {|i| presyms[i * block].getbyte(0).to_i < b}}
      File.open(table_header_path, "w:binary") do |f|
        f.puts table_mode_line(presyms)
        f.puts "#define MRB_PRESYM_COMPACT 1"
        f.puts "#define MRB_PRESYM_LEN_MAX #{presyms.map(&:bytesize).max}"
        f.puts "#define MRB_PRESYM_BLOCK_SHIFT #{COMPACT_BLOCK_SHIFT}"
        f.puts "#define MRB_PRESYM_NCHARS #{chars.size}"
        f.puts
        f.puts "static const char presym_chars[] = {"
        chars.each_slice(16) {|row| f.puts "  #{row.map {|b| '0x%02x,' % b}.join(' ')}"}
        f.puts "};"
        f.puts
        f.puts "static const uint16_t presym_pair_end[] = {"
        pair_end.each_slice(16) {|row| f.puts "  #{row.join(', ')},"}
        f.puts "  0" if pairs.empty?
        f.puts "};"
        f.puts
        f.puts "static const char presym_pair_text[] = {"
        pair_text.flatten.each_slice(16) {|row| f.puts "  #{row.map {|b| '0x%02x,' % b}.join(' ')}"}
        f.puts "  0" if pairs.empty?
        f.puts "};"
        f.puts
        f.puts "static const uint16_t presym_first_block[] = {"
        first_block.each_slice(16) {|row| f.puts "  #{row.join(', ')},"}
        f.puts "};"
        f.puts
        f.puts "static const #{offset_type} presym_block_offset[] = {"
        offsets.each_slice(12) {|row| f.puts "  #{row.join(', ')},"}
        f.puts "};"
        f.puts
        f.puts "static const uint8_t presym_codes[] = {"
        bytes.each_slice(16) {|row| f.puts "  #{row.map {|b| '0x%02x,' % b}.join(' ')}"}
        f.puts "};"
      end
    end

    # The decoder of src/symbol.c (presym_decode), run over the whole table: a coding bug fails the build here.
    def compact_verify(presyms, chars, pairs, offsets, bytes)
      expand = ->(code) { code < chars.size ? [chars[code]] : pairs[code - chars.size].flat_map(&expand) }
      block = 1 << COMPACT_BLOCK_SHIFT
      name = []
      pos = nil
      presyms.each_with_index do |sym, idx|
        pos = offsets[idx / block] if idx % block == 0
        name = name.first(bytes[pos] & 0x3f)
        pos += 1
        while bytes[pos] < COMPACT_HEADER
          name.concat(expand.(bytes[pos]))
          pos += 1
        end
        raise "presym: compact table decodes #{name.pack('C*').inspect} for #{sym.inspect}" unless name.pack('C*') == sym
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
