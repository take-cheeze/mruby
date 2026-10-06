assert('sprintf invalid') do
  assert_raise(ArgumentError) { sprintf('%1$*d', 3) }
  assert_raise(ArgumentError) { sprintf('%1$.*d', 3) }
end

assert('String#%') do
  assert_equal "one=1", "one=%d" % 1
  assert_equal "1 one", "%d %s" % [ 1, "one" ]
  assert_equal "123 < 456", "%{num} < %<str>s" % { num: 123, str: "456" }
  assert_equal 15, ("%b" % (1<<14)).size
  skip unless Object.const_defined?(:Float)
  assert_equal "1.0", "%3.1f" % 1.01
  assert_equal " 12345.12", "% 4.2f" % 12345.1234
  assert_equal "12345.12", "%-4.2f" % 12345.12345
  assert_equal "+12345.12", "%+4.2f" % 12345.1234
  assert_equal "12345.12", "%04.2f" % 12345.12345
  assert_equal "0012345.12", "%010.2f" % 12345.1234
end

assert('String#% with the 0 flag and a sign') do
  skip unless Object.const_defined?(:Float)
  assert_equal "-001.500", "%08.3f" % -1.5
  assert_equal "+001.500", "%+08.3f" % 1.5
  assert_equal " 001.500", "% 08.3f" % 1.5
  assert_equal "-1.500", "%05.3f" % -1.5
  assert_equal "-1.5", "%0.1f" % -1.5
  assert_equal "-01.50e+00", "%010.2e" % -1.5
  assert_equal "-00001.5", "%08.3g" % -1.5
  assert_equal "-1.500  ", "%-08.3f" % -1.5
end

assert('String#% with inf') do
  skip unless Object.const_defined?(:Float)
  inf = Float::INFINITY

  assert_equal "Inf", "%f" % inf
  assert_equal "Inf", "%2f" % inf
  assert_equal "Inf", "%3f" % inf
  assert_equal " Inf", "%4f" % inf
  assert_equal "  Inf", "%5f" % inf

  assert_equal "+Inf", "%+f" % inf
  assert_equal "+Inf", "%+2f" % inf
  assert_equal "+Inf", "%+3f" % inf
  assert_equal "+Inf", "%+4f" % inf
  assert_equal " +Inf", "%+5f" % inf

  assert_equal "Inf", "%-f" % inf
  assert_equal "Inf", "%-2f" % inf
  assert_equal "Inf", "%-3f" % inf
  assert_equal "Inf ", "%-4f" % inf
  assert_equal "Inf  ", "%-5f" % inf

  assert_equal " Inf", "% f" % inf
  assert_equal " Inf", "% 2f" % inf
  assert_equal " Inf", "% 3f" % inf
  assert_equal " Inf", "% 4f" % inf
  assert_equal "  Inf", "% 5f" % inf
end

assert('String#% with nan') do
  skip unless Object.const_defined?(:Float)
  nan = Float::NAN

  assert_equal "NaN", "%f" % nan
  assert_equal "NaN", "%2f" % nan
  assert_equal "NaN", "%3f" % nan
  assert_equal " NaN", "%4f" % nan
  assert_equal "  NaN", "%5f" % nan

  assert_equal "+NaN", "%+f" % nan
  assert_equal "+NaN", "%+2f" % nan
  assert_equal "+NaN", "%+3f" % nan
  assert_equal "+NaN", "%+4f" % nan
  assert_equal " +NaN", "%+5f" % nan

  assert_equal "NaN", "%-f" % nan
  assert_equal "NaN", "%-2f" % nan
  assert_equal "NaN", "%-3f" % nan
  assert_equal "NaN ", "%-4f" % nan
  assert_equal "NaN  ", "%-5f" % nan

  assert_equal " NaN", "% f" % nan
  assert_equal " NaN", "% 2f" % nan
  assert_equal " NaN", "% 3f" % nan
  assert_equal " NaN", "% 4f" % nan
  assert_equal "  NaN", "% 5f" % nan
end

assert("String#% %b") do
  assert_equal("..10115", "%0b5" % -5)
end

assert("String#% %x, %o and %b of a negative number and with a sign flag") do
  assert_equal("..F01", "%X" % -255)
  assert_equal("0X..F01", "%#X" % -255)
  assert_equal("..1011", "%B" % -5)
  assert_equal("+FF", "%+X" % 255)
  assert_equal("-FF", "%+X" % -255)
  assert_equal("+ff", "%+x" % 255)
  assert_equal(" 101", "% b" % 5)
  assert_equal("-10", "% o" % -8)
end

assert("String#% pads the two's complement of a negative number") do
  assert_equal("     ..f", "%8x" % -1)
  assert_equal("..ffffff", "%08x" % -1)
  assert_equal("..ffffffffff", "%.12x" % -1)
  assert_equal("   0x..f", "%#8x" % -1)
  assert_equal("0x..f   ", "%-#8.3x" % -1)
  assert_equal("0X..FF01", "%#08X" % -255)
  assert_equal("..770", "%#.5o" % -8)
  assert_equal("..110   ", "%-8.5B" % -2)
  assert_equal("..f0", "%.3x" % -16)
  assert_equal("0x..f0", "%#x" % -16)
end

assert("String#% %o keeps a leading 1 or 3 digit of a negative number") do
  assert_equal("..71", "%o" % -7)
  assert_equal("..710", "%o" % -56)
  assert_equal("..7177777777", "%o" % -100663297)
  assert_equal("..70", "%o" % -8)
  assert_equal("..7", "%o" % -1)
end

assert("String#% %o keeps other leading digits of a negative big integer") do
  begin
    big = 1 << 93
  rescue RangeError
    skip 'requires mruby-bigint'
  end
  assert_equal("..767777777777777777777777777777770", "%o" % (-big - 8))
end

assert("String#% %d of a big integer with a sign") do
  k = 70
  begin
    big = 1 << k
  rescue RangeError
    skip 'requires mruby-bigint'
  end
  assert_equal("-00000001180591620717411303424", "%030d" % -big)
  assert_equal("+1180591620717411303424", "%+d" % big)
  assert_equal(" 1180591620717411303424", "% d" % big)
  assert_equal("-0001180591620717411303424", "%.25d" % -big)
end

assert("String#% %d") do
  assert_equal("  10",   "%4d" % 10)
  assert_equal("1000",   "%4d" % 1000)
  assert_equal("10000",  "%4d" % 10000)
end

assert("String#% invalid format") do
  assert_raise ArgumentError do
    "%?" % ""
  end
end

assert("sprintf rejects an oversized float precision/width") do
  # A precision/width near INT_MAX used to allocate ~2GB and overflow the
  # formatter's int length into a negative memmove size, segfaulting on input
  # like "%.2147483647e" (clusterfuzz). It must raise instead.
  skip unless Object.const_defined?(:Float)
  assert_raise(ArgumentError) { sprintf("%.2147483647e", 131072) }
  assert_raise(ArgumentError) { sprintf("%2147483647e", 1.0) }
  assert_raise(ArgumentError) { sprintf("%.2147483647f", 1.0) }
  # ordinary precision/width still work
  assert_equal "1.31072e+05", sprintf("%.5e", 131072.0)
  assert_equal "3.1400000000", sprintf("%.10f", 3.14)
end

assert("sprintf with to_s mutating format string") do
  # The to_s callback must not be able to invalidate sprintf's internal
  # iteration pointers by mutating the format string.
  fmt = "%s" + "B" * 200
  mutator = Object.new
  $sprintf_test_fmt = fmt
  def mutator.to_s
    $sprintf_test_fmt.replace("Z")
    "ok"
  end
  result = sprintf(fmt, mutator)
  assert_equal 202, result.length
  assert_equal "ok", result[0, 2]
  assert_equal "B" * 200, result[2..]
end

assert('sprintf - a width near mrb_int is refused, not wrapped') do
  # CHECK() grows the output buffer, and it used to ask whether blen+(l)
  # reached bsiz.  That sum is evaluated in mrb_int, so a width near the
  # maximum wrapped negative, read as below bsiz, and the resize was skipped:
  # the FILL that followed then wrote about 2GB into a buffer of some 120
  # bytes (CVE-2018-14337, fixed once in 180f39bf4 and reintroduced by
  # b58094c88).  A literal ahead of the directive is what makes blen non-zero
  # and the sum wrap.
  #
  # 2147483647 is the width that wrapped.  Where mrb_int is 32-bit it is past
  # what a String can hold and has to be refused; where mrb_int is wider it is
  # an ordinary request and the answer is a 2GB string, which is not something
  # to allocate in a test.  Asking sprintf which build this is would build that
  # string to find out, so the width is read off an integer instead: 2**31 is
  # an object of its own where mrb_int stops below it, and where the build has
  # no wider integer at all it cannot be reached.  Spelling it as a literal
  # would take the whole file down on the builds this guards, so it is raised.
  narrow =
    begin
      n = 2 ** 31
      !n.equal?(2 ** 31)
    rescue RangeError
      true
    end
  skip "mrb_int holds a width of 2147483647" unless narrow

  assert_raise(ArgumentError) { sprintf("ab%2147483647c", 65) }
  assert_raise(ArgumentError) { sprintf("ab%2147483647d", 0) }
  assert_raise(ArgumentError) { sprintf("ab%2147483647s", "x") }
  assert_raise(ArgumentError) { sprintf("ab%.2147483647d", 0) }
  assert_raise(ArgumentError) { sprintf("ab%2147483626f", 1.0) } if Object.const_defined?(:Float)
end

assert('sprintf - the widths a buffer can still be asked for keep working') do
  assert_equal "  42", sprintf("%4d", 42)
  assert_equal "00042", sprintf("%.*d", 5, 42)
  assert_equal 1000, sprintf("%1000d", 7).size
  assert_equal 500, sprintf("%500s", "x").size
  assert_equal "     1.000000", sprintf("%13f", 1.0) if Object.const_defined?(:Float)
end
