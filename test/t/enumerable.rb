##
# Enumerable ISO Test

assert('Enumerable', '15.3.2') do
  assert_equal(Module, Enumerable.class)
end

assert('Enumerable#all?', '15.3.2.2.1') do
  assert_true([1,2,3].all?)
  assert_false([1,false,3].all?)

  a = [2,4,6]
  all = a.all? do |e|
    e % 2 == 0
  end
  assert_true(all)

  a = [2,4,7]
  all = a.all? do |e|
    e % 2 == 0
  end
  assert_false(all)
end

assert('Enumerable#any?', '15.3.2.2.2') do
  assert_true([false,true,false].any?)
  assert_false([false,false,false].any?)

  a = [1,3,6]
  any = a.any? do |e|
    e % 2 == 0
  end
  assert_true(any)

  a = [1,3,5]
  any = a.any? do |e|
    e % 2 == 0
  end
  assert_false(any)
end

assert('Enumerable#collect', '15.3.2.2.3') do
  assert_true [1,2,3].collect { |i| i + i } == [2,4,6]
end

assert('Enumerable#detect', '15.3.2.2.4') do
  assert_equal 1, [1,2,3].detect() { true }
  assert_equal 'a', [1,2,3].detect(->{"a"}) { false }
end

assert('Array#each_with_index', '15.3.2.2.5') do
  a = nil
  b = nil

  [1].each_with_index {|e,i| a = e; b = i}

  assert_equal(1, a)
  assert_equal(0, b)
end

assert('Enumerable#entries', '15.3.2.2.6') do
  assert_equal([1], [1].entries)
end

assert('Enumerable#find', '15.3.2.2.7') do
  assert_equal 1, [1,2,3].find() { true }
  assert_equal 'a', [1,2,3].find(->{"a"}) { false }
end

assert('Enumerable#find_all', '15.3.2.2.8') do
  assert_equal [2,4,6,8], [1,2,3,4,5,6,7,8,9].find_all() {|i| i%2 == 0}
end

assert('Enumerable#grep', '15.3.2.2.9') do
  assert_equal [4,5,6], [1,2,3,4,5,6,7,8,9].grep(4..6)
end

assert('Enumerable#include?', '15.3.2.2.10') do
  assert_true [1,2,3,4,5,6,7,8,9].include?(5)
  assert_false [1,2,3,4,5,6,7,8,9].include?(0)
end

assert('Enumerable#inject', '15.3.2.2.11') do
  assert_equal 21, [1,2,3,4,5,6].inject() {|s, n| s + n}
  assert_equal 22, [1,2,3,4,5,6].inject(1) {|s, n| s + n}
end

assert('Enumerable#inject native (symbol, reduce, edge cases)') do
  assert_equal 21, [1,2,3,4,5,6].inject(:+)
  assert_equal 22, [1,2,3,4,5,6].inject(1, :+)
  assert_equal 21, [1,2,3,4,5,6].reduce(:+)
  assert_equal 22, [1,2,3,4,5,6].reduce(1) {|s, n| s + n}
  assert_nil [].inject(:+)
  assert_nil [].inject {|s, n| s + n}
  assert_equal 7, [].inject(7, :+)
  assert_equal 4, [4].inject(:+)
  assert_equal "abc", ["a", "b", "c"].inject(:+)
  assert_equal 9, [1,2,3].inject { break 9 }
  assert_raise(ArgumentError) { [1].inject(1, 2, 3) {} }
  # a multi-value yield is one value, as |*val| with __svalue gave it
  multi = Object.new.extend(Enumerable)
  def multi.each; yield 1, 2; yield 3; yield; end
  assert_equal [[1, 2], 3, nil], multi.inject([]) {|a, v| a << v}
  # the symbol is sent with __send__ semantics (private methods too)
  klass = Class.new do
    include Enumerable
    attr_reader :v
    def initialize(v); @v = v; end
    def each; yield self; yield self.class.new(3); end
    private def add(o); self.class.new(@v + o.v); end
  end
  assert_equal 5, klass.new(2).inject(:add).v
end

assert('Enumerable#map', '15.3.2.2.12') do
  assert_equal [2,4,6], [1,2,3].map { |i| i + i }
end

assert('Enumerable#max', '15.3.2.2.13') do
  a = ['aaa', 'bb', 'c']
  assert_equal 'c', a.max
  assert_equal 'aaa', a.max {|i1,i2| i1.length <=> i2.length}
end

assert('Enumerable#min', '15.3.2.2.14') do
  a = ['aaa', 'bb', 'c']
  assert_equal 'aaa', a.min
  assert_equal 'c', a.min {|i1,i2| i1.length <=> i2.length}
end

assert('Enumerable#member?', '15.3.2.2.15') do
  assert_true [1,2,3,4,5,6,7,8,9].member?(5)
  assert_false [1,2,3,4,5,6,7,8,9].member?(0)
end

assert('Enumerable#partition', '15.3.2.2.16') do
  partition = [0,1,2,3,4,5,6,7,8,9].partition do |i|
    i % 2 == 0
  end
  assert_equal [[0,2,4,6,8], [1,3,5,7,9]], partition
end

assert('Enumerable#reject', '15.3.2.2.17') do
  reject = [0,1,2,3,4,5,6,7,8,9].reject do |i|
    i % 2 == 0
  end
  assert_equal [1,3,5,7,9], reject
end

assert('Enumerable#select', '15.3.2.2.18') do
  assert_equal [2,4,6,8], [1,2,3,4,5,6,7,8,9].select() {|i| i%2 == 0}
end

assert('Enumerable#sort', '15.3.2.2.19') do
  assert_equal [1,2,3,4,6,7], [7,3,1,2,6,4].sort
  assert_equal [7,6,4,3,2,1], [7,3,1,2,6,4].sort {|e1,e2|e2<=>e1}
end

assert('Enumerable#to_a', '15.3.2.2.20') do
  assert_equal [1], [1].to_a
end
