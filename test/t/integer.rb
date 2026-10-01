##
# Integer ISO Test

assert('Integer', '15.2.8') do
  assert_equal Class, Integer.class
end

assert('Integer#+', '15.2.8.3.1') do
  a = 1+1
  b = 1+1.0 if Object.const_defined?(:Float)

  assert_equal 2, a
  assert_equal 2.0, b if Object.const_defined?(:Float)

  assert_raise(TypeError){ 0+nil }
  assert_raise(TypeError){ 1+nil }
end

assert('Integer#-', '15.2.8.3.2') do
  a = 2-1
  b = 2-1.0 if Object.const_defined?(:Float)

  assert_equal 1, a
  assert_equal 1.0, b if Object.const_defined?(:Float)
end

assert('Integer#*', '15.2.8.3.3') do
  a = 1*1
  assert_equal 1, a
  if Object.const_defined?(:Float)
    b = 1*1.0
    assert_equal 1.0, b
  end
  assert_raise(TypeError){ 0*nil }
  assert_raise(TypeError){ 1*nil }
end

assert('Integer#/', '15.2.8.3.4') do
  a = 2/1
  assert_equal 2, a
  a = 5/2
  assert_equal 2, a
  b = -1/2
  assert_equal(-1, b)
  b = 1/-2
  assert_equal(-1, b)
  skip unless Object.const_defined?(:Float)
  b = 2/1.0
  assert_equal 2.0, b
end

if Object.const_defined?(:Float)
  assert('Integer#quo') do
    a = 6.quo(5)
    assert_equal 1.2, a
  end
end

assert('Integer#%', '15.2.8.3.5') do
  a = 1%1
  b = 2%4
  c = 2%5
  d = 2%-5
  e = -2%5
  f = -2%-5
  g = 2%-2
  h = -2%2
  i = -2%-2

  assert_equal 0, a
  assert_equal 2, b
  assert_equal 2, c
  assert_equal(-3, d)
  assert_equal 3, e
  assert_equal(-2, f)
  assert_equal 0, g
  assert_equal 0, h
  assert_equal 0, i
  skip unless Object.const_defined?(:Float)
  j = 1%1.0
  assert_equal 0.0, j
end

assert('Integer#<=>', '15.2.9.3.6') do
  a = 1<=>0
  b = 1<=>1
  c = 1<=>2

  assert_equal  1, a
  assert_equal  0, b
  assert_equal(-1, c)
end

assert('Integer#==', '15.2.8.3.7') do
  a = 1==0
  b = 1==1

  assert_false a
  assert_true b
end

assert('Integer comparison past the inline fixnum range') do
  # word boxing keeps Integers past 2**30 (32-bit word) or 2**62 (64-bit
  # word) on the heap; they must still compare as exact integers
  bases = [0x3fff_ffff]
  begin
    # 2**62-1 built at run time: a >32-bit literal needs mruby-bigint
    bases << ((((bases[0] << 16) | 0xffff) << 16) | 0xffff)
  rescue RangeError
    # 32-bit mrb_int without bigint
  end
  bases.each do |base|
    a = base + 1
    b = base + 2
    c = base + 3
    next unless a.is_a?(Integer) && c.is_a?(Integer)
    a2 = base + 1                       # equal value, distinct object

    assert_equal(-1, a <=> b)
    assert_equal(1, b <=> a)
    assert_equal(0, a <=> a2)
    assert_equal(1, -a <=> -b)
    assert_equal(-1, base <=> a)
    assert_equal(1, a <=> base)
    assert_true(a.__send__(:<, b))
    assert_false(b.__send__(:<, a))
    assert_true(a.__send__(:<=, a2))
    assert_true(b.__send__(:>, a))
    assert_true(a.__send__(:>=, a2))
    assert_false(a.__send__(:>, a2))
    assert_true(a == a2)
    assert_true(a.eql?(a2))
    assert_false(a == b)

    assert_equal([-c, -a, 1, a, b, c], [c, 1, a, -c, b, -a].sort)
    assert_equal([c, b, a], [a, c, b].sort { |x, y| y <=> x })
    assert_equal(c, [a, c, b].max)
    assert_equal(a, [b, a, c].min)

    assert_equal(a.hash, a2.hash)
    h = { a => :a, b => :b }
    assert_equal(:a, h[a2])
    assert_equal(:b, h[base + 2])
    assert_nil(h[c])
  end
end

assert('Integer#~', '15.2.8.3.8') do
  # Complement
  assert_equal(-1, ~0)
  assert_equal(-3, ~2)
end

assert('Integer#&', '15.2.8.3.9') do
  # Bitwise AND
  #   0101 (5)
  # & 0011 (3)
  # = 0001 (1)
  assert_equal 1, 5 & 3
end

assert('Integer#|', '15.2.8.3.10') do
  # Bitwise OR
  #   0101 (5)
  # | 0011 (3)
  # = 0111 (7)
  assert_equal 7, 5 | 3
end

assert('Integer#^', '15.2.8.3.11') do
  # Bitwise XOR
  #   0101 (5)
  # ^ 0011 (3)
  # = 0110 (6)
  assert_equal 6, 5 ^ 3
end

assert('Integer bitwise ops reject non-Integer operands') do
  # A non-Integer operand has no bit pattern to combine, so &, |, ^ raise
  # TypeError instead of silently reading garbage (Float used to return a
  # bogus value via an unchecked union access).
  assert_raise(TypeError) { 5 & 5.0 }
  assert_raise(TypeError) { 5 | 5.0 }
  assert_raise(TypeError) { 5 ^ 5.0 }
  assert_raise(TypeError) { 5 | "3" }
  assert_raise(TypeError) { 5 & nil }
  assert_raise(TypeError) { 5 ^ :sym }
end

assert('Integer#<<', '15.2.8.3.12') do
  # Left Shift by one
  #   00010111 (23)
  # = 00101110 (46)
  assert_equal 46, 23 << 1

  # Left Shift by a negative is Right Shift
  assert_equal 23, 46 << -1

  skip unless Object.const_defined?(:Float)
end

assert('Integer#>>', '15.2.8.3.13') do
  # Right Shift by one
  #   00101110 (46)
  # = 00010111 (23)
  assert_equal 23, 46 >> 1

  # Right Shift by a negative is Left Shift
  assert_equal 46, 23 >> -1

  # Don't raise on large Right Shift
  assert_equal 0, 23 >> 128
end

assert('Integer#ceil', '15.2.8.3.14') do
  assert_equal 10, 10.ceil
  assert_equal 10, 10.ceil(2)
  assert_equal 1300, 1234.ceil(-2)
  assert_equal(-1200, -1234.ceil(-2))
  assert_equal 0, (-99).ceil(-2)
end

assert('Integer#ceil keeps a value that is already a multiple') do
  assert_equal 0, 0.ceil(-1)
  assert_equal 10, 10.ceil(-1)
  assert_equal 100, 100.ceil(-2)
  assert_equal 1200, 1200.ceil(-2)
  assert_equal(-100, -100.ceil(-2))
  assert_equal(-1200, -1200.ceil(-2))
end

assert('Integer#downto', '15.2.8.3.15') do
  a = 0
  3.downto(1) do |i|
    a += i
  end
  assert_equal 6, a
end

assert('Integer#eql?', '15.2.8.3.16') do
  a = 1.eql?(1)
  b = 1.eql?(2)
  c = 1.eql?(nil)

  assert_true a
  assert_false b
  assert_false c

  # `eql?` compares class as well as value, unlike `==`.
  if Object.const_defined?(:Float)
    assert_false 1.eql?(1.0)
    assert_true 1 == 1.0
  end
end

assert('Integer#floor', '15.2.8.3.17') do
  a = 1.floor

  assert_equal 1, a
  assert_equal 1, 1.floor(2)
  assert_equal 1200, 1234.floor(-2)
  assert_equal(-1300, -1234.floor(-2))
  assert_equal 0, 99.floor(-2)
end

assert('Integer#floor keeps a value that is already a multiple') do
  assert_equal 0, 0.floor(-1)
  assert_equal(-10, -10.floor(-1))
  assert_equal(-100, -100.floor(-2))
  assert_equal(-1200, -1200.floor(-2))
  assert_equal 100, 100.floor(-2)
  assert_equal 1200, 1200.floor(-2)
end

assert('Integer#next', '15.2.8.3.19') do
  assert_equal 2, 1.next
end

assert('Integer#round', '15.2.8.3.20') do
  assert_equal 1, 1.round
  assert_equal 1, 1.round(2)
  assert_equal 12300, 12345.round(-2)
  assert_equal 12350, 12345.round(-1)
  assert_equal 12000, 12345.round(-3)
  assert_equal(-12300, -12345.round(-2))
  assert_equal 0, 4.round(-1)
  assert_equal 100, 99.round(-2)
  assert_equal 0, 0.round(-1)
end

assert('Integer#round keeps a value that is already a multiple') do
  assert_equal 10, 10.round(-1)
  assert_equal 100, 100.round(-2)
  assert_equal 1200, 1200.round(-2)
  assert_equal(-100, -100.round(-2))
  assert_equal(-1200, -1200.round(-2))
end

assert('Integer#round breaks a tie away from zero') do
  assert_equal 10, 5.round(-1)
  assert_equal 20, 15.round(-1)
  assert_equal 30, 25.round(-1)
  assert_equal 200, 150.round(-2)
  assert_equal 300, 250.round(-2)
  assert_equal(-10, -5.round(-1))
  assert_equal(-20, -15.round(-1))
  assert_equal(-30, -25.round(-1))
  assert_equal(-200, -150.round(-2))
end

assert('Integer#succ', '15.2.8.3.21') do
  assert_equal 2, 1.succ
end

assert('Integer#times', '15.2.8.3.22') do
  a = 0
  3.times do
    a += 1
  end
  assert_equal 3, a
end

assert('Integer#to_f', '15.2.8.3.23') do
  skip unless Object.const_defined?(:Float)
  assert_equal 1.0, 1.to_f
end

assert('Integer#to_i', '15.2.8.3.24') do
  assert_equal 1, 1.to_i
end

assert('Integer#to_s', '15.2.8.3.25') do
  assert_equal "1", 1.to_s
  assert_equal "-1", -1.to_s
  assert_equal "1010", 10.to_s(2)
  assert_equal "a", 10.to_s(36)
  assert_equal "-a", -10.to_s(36)
  assert_equal "30071", 12345.to_s(8)
  assert_raise(ArgumentError) { 10.to_s(-1) }
  assert_raise(ArgumentError) { 10.to_s(0) }
  assert_raise(ArgumentError) { 10.to_s(1) }
  assert_raise(ArgumentError) { 10.to_s(37) }
end

assert('Integer#to_s(2) with the smallest Integer') do
  # The magnitude of the smallest Integer spends every bit an mrb_int has, so
  # base 2 is the one conversion that writes MRB_INT_BIT digits and still
  # takes a sign in front of them. Both widths are asked, so that this
  # build's is among them; the other one names a value that fits, or one out
  # of range without mruby-bigint, which is skipped.
  [31, 63].each do |e|
    begin
      min = -(1 << (e - 1)) * 2
    rescue RangeError
      next
    end
    assert_equal "-1" + "0" * e, min.to_s(2)
  end
end

assert('Integer#truncate', '15.2.8.3.26') do
  assert_equal 1, 1.truncate
  assert_equal 1, 1.truncate(2)
  assert_equal 1200, 1234.truncate(-2)
  assert_equal(-1200, -1234.truncate(-2))
  assert_equal 1200, 1200.truncate(-2)
  assert_equal 0, 99.truncate(-2)
end

assert('Integer rounding to a power of ten wider than an mrb_int') do
  # 10 ** 19 is wider than an mrb_int, so the four methods answer 0 rather than
  # reaching for a power they cannot hold.
  (19..25).each do |n|
    assert_equal 0, 12345.round(-n)
    assert_equal 0, (-12345).round(-n)
    assert_equal 0, 12345.truncate(-n)
    assert_equal 0, 12345.floor(-n)
    assert_equal 0, (-12345).ceil(-n)
  end
end

assert('Integer#upto', '15.2.8.3.27') do
  a = 0
  1.upto(3) do |i|
    a += i
  end
  assert_equal 6, a
end

assert('Integer#divmod', '15.2.8.3.30') do
  assert_equal [ 0,  0],   0.divmod(1)
  assert_equal [ 0,  1],   1.divmod(3)
  assert_equal [ 3,  0],   3.divmod(1)
  assert_equal [ 2,  6],  20.divmod(7)
  assert_equal [-1,  2],  -3.divmod(5)
  assert_equal [-2, -1],  25.divmod(-13)
  assert_equal [ 1, -6], -13.divmod(-7)
end
