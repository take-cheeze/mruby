##
# String#encode Test

assert('String#encode returns a new string in the named encoding') do
  s = "abc"
  e = s.encode("UTF-8")
  assert_equal "abc", e
  assert_equal "UTF-8", e.encoding
  assert_not_same s, e
  assert_equal "US-ASCII", "abc".encode("US-ASCII").encoding
  assert_equal "US-ASCII", "abc".encode("ASCII").encoding
  assert_equal "ASCII-8BIT", "abc".encode("BINARY").encoding
  assert_equal "ASCII-8BIT", "abc".encode(Encoding::ASCII_8BIT).encoding
  assert_equal "UTF-8", "abc".encode(Encoding::UTF_8).encoding
end

assert('String#encode with no arguments keeps the encoding') do
  s = "hé".force_encoding("BINARY")
  e = s.encode
  assert_equal "ASCII-8BIT", e.encoding
  assert_equal s, e
  assert_not_same s, e
  assert_equal "UTF-8", "hé".encode.encoding
end

assert('String#encode same encoding is a copy, not validated') do
  s = "\xff".force_encoding("UTF-8")
  e = s.encode("UTF-8")
  assert_equal "UTF-8", e.encoding
  assert_equal "\xff".force_encoding("UTF-8"), e
  assert_false e.valid_encoding?
  assert_equal "\xff".force_encoding("BINARY"), "\xff".force_encoding("BINARY").encode("BINARY")
end

assert('String#encode ASCII-only strings change encoding') do
  assert_equal "UTF-8", "abc".force_encoding("US-ASCII").encode("UTF-8").encoding
  assert_equal "US-ASCII", "abc".force_encoding("BINARY").encode("US-ASCII").encoding
  assert_equal "abc", "abc".force_encoding("BINARY").encode("UTF-8")
end

assert('String#encode UTF-8 to US-ASCII and BINARY') do
  assert_raise(Encoding::UndefinedConversionError) { "hé".encode("US-ASCII") }
  assert_raise(Encoding::UndefinedConversionError) { "hé".encode("BINARY") }
end

assert('String#encode undef: :replace') do
  assert_equal "h?", "hé".encode("US-ASCII", undef: :replace)
  assert_equal "h*", "hé".encode("US-ASCII", undef: :replace, replace: "*")
  assert_equal "h?", "hé".encode("BINARY", undef: :replace)
end

assert('String#encode binary to UTF-8 and US-ASCII') do
  assert_raise(Encoding::UndefinedConversionError) { "\xff".force_encoding("BINARY").encode("UTF-8") }
  assert_raise(Encoding::UndefinedConversionError) { "\xff".force_encoding("BINARY").encode("US-ASCII") }
  assert_equal "�", "\xff".force_encoding("BINARY").encode("UTF-8", undef: :replace)
  assert_equal "?", "\xff".force_encoding("BINARY").encode("US-ASCII", undef: :replace)
  assert_equal "a�b", "a\xffb".force_encoding("BINARY").encode("UTF-8", undef: :replace)
end

assert('String#encode invalid byte sequences') do
  s = "\xff".force_encoding("UTF-8")
  assert_raise(Encoding::InvalidByteSequenceError) { s.encode("US-ASCII") }
  assert_raise(Encoding::InvalidByteSequenceError) { "\xff".force_encoding("US-ASCII").encode("UTF-8") }
  assert_equal "�", s.encode("UTF-8", invalid: :replace)
  assert_equal "?", s.encode("US-ASCII", invalid: :replace)
  assert_equal "��", "\xff\xfe".force_encoding("UTF-8").encode("UTF-8", invalid: :replace)
  assert_equal "�", "\xe3\x81".force_encoding("UTF-8").encode("UTF-8", invalid: :replace)
end

assert('String#encode with a source encoding') do
  assert_equal "abc", "abc".force_encoding("BINARY").encode("UTF-8", "BINARY")
  assert_equal "UTF-8", "abc".force_encoding("BINARY").encode("UTF-8", "BINARY").encoding
  assert_raise(Encoding::UndefinedConversionError) { "\xe9".force_encoding("BINARY").encode("UTF-8", "BINARY") }
  assert_equal "�", "\xe9".force_encoding("BINARY").encode("UTF-8", "US-ASCII", invalid: :replace)
end

assert('String#encode unsupported encodings and options') do
  assert_raise(NotImplementedError) { "abc".encode("ISO-8859-1") }
  assert_raise(NotImplementedError) { "abc".encode("FOO") }
  assert_raise(NotImplementedError) { "abc".encode("UTF-8", "ISO-8859-1") }
  assert_raise(NotImplementedError) { "abc".encode("UTF-16LE") }
  assert_raise(NotImplementedError) { "a\nb".encode("UTF-8", universal_newline: true) }
  assert_raise(NotImplementedError) { "a<".encode("US-ASCII", xml: :text) }
  assert_raise(ArgumentError) { "abc".encode("UTF-8", invalid: :foo) }
  assert_raise(ArgumentError) { "abc".encode("UTF-8", "US-ASCII", "BINARY") }
end

assert('Encoding::UndefinedConversionError and InvalidByteSequenceError') do
  assert_kind_of Class, Encoding::UndefinedConversionError
  assert_kind_of Class, Encoding::InvalidByteSequenceError
  assert_true Encoding::UndefinedConversionError.ancestors.include?(EncodingError)
  assert_true Encoding::InvalidByteSequenceError.ancestors.include?(EncodingError)
end
