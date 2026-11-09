##
# Builtin == / hash / initialize_copy / const_added are run without a send;
# a user redefinition of any of them must still be the one that runs.

class DynEqCount
  attr_reader :calls
  def initialize(r); @r = r; @calls = 0; end
  def ==(o); @calls += 1; @r; end
end

class DynStr < String
  def ==(o); true; end
end

assert('Array#== sends a user-defined element ==') do
  a = DynEqCount.new(true)
  b = DynEqCount.new(false)
  assert_true [a] == [b]
  assert_equal 1, a.calls
  assert_equal 0, b.calls
  f = DynEqCount.new(false)
  assert_false [f] == [f]            # no identity shortcut inside Array#==
  assert_equal 1, f.calls
  assert_true [DynStr.new("a")] == ["zzz"]
  assert_false ["zzz"] == [DynStr.new("a")]
end

assert('Array#== builtin elements') do
  assert_true  [1, 2.0, "a", :b, nil, true, [1, [2]]] == [1.0, 2, "a", :b, nil, true, [1, [2]]]
  assert_false [1, 2] == [1, 3]
  assert_false [1] == [1, 2]
  assert_false ["a"] == [:a]
  assert_false [:a] == ["a"]
  assert_false [1] == [nil]
  assert_false [Float::NAN] == [Float::NAN]
  a = [1]; a << a
  b = [1]; b << b
  assert_false a == b                # recursion guard still applies
end

assert('mrb_equal callers use builtin and user ==') do
  assert_true  [1, 2, 3].include?(2.0)
  assert_equal 1, [1, 2, 3].index(2)
  assert_true  ["a", "b"].include?("b")
  assert_true  [:a].include?(:a)
  assert_false [:a].include?("a")
  assert_true  [DynStr.new("x")].include?("anything")
  assert_false ["anything"].include?(DynStr.new("x"))
  assert_true  ({a: "anything"}).value?(DynStr.new("x"))
  assert_false ({a: DynStr.new("x")}).value?("anything")
  assert_true  ((1..2) == (1..2))
  assert_true  ((1..2) == (1.0..2.0))
end

assert('redefined Integer#== is honored by Array#==') do
  n = 0
  Integer.class_eval do
    alias __dyn_orig_eq ==
    define_method(:==) { |o| n += 1; __dyn_orig_eq(o) }
  end
  begin
    assert_true [1, 2] == [1, 2]
    assert_equal 2, n
    assert_true [5].include?(5)
    assert_true n > 2
  ensure
    Integer.class_eval do
      alias == __dyn_orig_eq
    end
  end
  assert_true [1, 2] == [1, 2]
  assert_equal 1, [1, 2].index(2)
end

# a small Hash compares keys linearly; only a large one hashes them
def dyn_big_hash
  h = {}
  40.times { |i| h[i] = i }
  h
end

class DynKey
  attr_reader :v
  def initialize(v); @v = v; end
end
class DynKeyH < DynKey
  def hash; @v.hash; end
  def eql?(o); o.is_a?(DynKeyH) && @v == o.v; end
end

assert('Hash key hash: default Kernel#hash') do
  a = DynKey.new(1)
  b = DynKey.new(1)
  h = dyn_big_hash
  h[a] = :a
  assert_equal :a, h[a]
  assert_nil h[b]
  h[b] = :b
  assert_equal 42, h.size
  assert_equal :a, h[a]
end

assert('Hash key hash: user-defined hash is called') do
  h = dyn_big_hash
  h[DynKeyH.new(7)] = :x
  assert_equal :x, h[DynKeyH.new(7)]
  assert_nil h[DynKeyH.new(8)]
  h[DynKeyH.new(7)] = :y
  assert_equal 41, h.size
  h2 = dyn_big_hash
  k = DynKey.new(1)
  def k.hash; 42; end
  h2[k] = 1
  assert_equal 1, h2[k]
  assert_equal k, h2.keys.last
end

assert('initialize_copy: default and user-defined') do
  s = "abc"; t = s.dup
  assert_equal "abc", t
  t[0] = "z"
  assert_equal "abc", s
  a = [1, [2]]; b = a.dup
  assert_equal a, b
  b << 3
  assert_equal [1, [2]], a
  h = Hash.new(5); h[:a] = 1
  g = h.dup
  assert_equal({a: 1}, g)
  assert_equal 5, g[:zz]
  g[:b] = 2
  assert_equal({a: 1}, h)
  assert_equal "abc", s.clone
  assert_true "abc".freeze.clone.frozen?
  assert_false "abc".freeze.dup.frozen?

  log = []
  sc = Class.new(String) { define_method(:initialize_copy) { |o| log << :str; super(o) } }
  assert_equal "q", sc.new("q").dup
  ac = Class.new(Array) { define_method(:initialize_copy) { |o| log << :ary; super(o) } }
  assert_equal [1], ac.new([1]).dup
  hc = Class.new(Hash) { define_method(:initialize_copy) { |o| log << :hash; super(o) } }
  x = hc.new; x[1] = 2
  assert_equal({1 => 2}, x.dup)
  oc = Class.new { define_method(:initialize_copy) { |o| log << :obj } }
  oc.new.clone
  assert_equal [:str, :ary, :hash, :obj], log
  assert_equal 3, [3].dup.first
end

assert('const_added: default and user-defined') do
  m = Module.new
  m.const_set(:A, 1)
  assert_equal 1, m::A
  $dyn_seen = []
  class << m
    def const_added(n); $dyn_seen << n; end
  end
  m.const_set(:B, 2)
  assert_equal [:B], $dyn_seen
  $dyn_seen2 = []
  k = Class.new do
    def self.const_added(n); $dyn_seen2 << n; super; end
  end
  k.const_set(:E, 5)
  assert_equal [:E], $dyn_seen2
  assert_equal 5, k::E
end

assert('redefinition after the builtin was used is seen') do
  c = Class.new do
    def initialize(v); @v = v; end
  end
  h = {}
  40.times { |i| h[i] = i }
  x = c.new(1)
  h[x] = 1; h[x]; x.dup; c.new(2).dup
  [x] == [x]
  c.const_set(:K0, 0)
  seen = []
  $dyn_late = seen
  c.class_eval do
    def hash; $dyn_late << :hash; 7; end
    def eql?(o); true; end
    def ==(o); $dyn_late << :eq; true; end
    def initialize_copy(o); $dyn_late << :copy; super; end
    def self.const_added(n); $dyn_late << n; end
  end
  y = c.new(3)
  h[y] = 2
  assert_true seen.include?(:hash)
  assert_true [x] == [c.new(9)]
  assert_true seen.include?(:eq)
  y.dup
  assert_true seen.include?(:copy)
  c.const_set(:K1, 1)
  assert_true seen.include?(:K1)
end

# Class#new, ==, eql? and <=> run the method they find, in the frame the send
# would push: overrides, privacy, arity, blocks and recursion guards as before.
class DynNew
  attr_reader :a, :k, :b
  def initialize(a, k: 0, &b); @a = a; @k = k; @b = b; end
end

class DynMissingInit
  undef_method :initialize
  attr_reader :mm
  def method_missing(n, *a); n == :initialize ? (@mm = a) : super; end
end

assert('Class#new runs the initialize and allocate it finds') do
  x = DynNew.new(1, k: 2) { 3 }
  assert_equal [1, 2, 3], [x.a, x.k, x.b.call]
  e1 = (DynNew.new rescue $!)
  e2 = (DynNew.allocate.__send__(:initialize) rescue $!)
  assert_equal ArgumentError, e1.class
  assert_equal e2.message, e1.message
  c = Class.new(DynNew)
  assert_equal 5, c.new(5).a
  c.class_eval { def initialize(*a); @a = a; end }
  assert_equal [7, 8], c.new(7, 8).a
  $dyn_alloc = []
  class << c
    def allocate; $dyn_alloc << :alloc; super; end
  end
  assert_equal [9], c.new(9).a
  assert_equal [:alloc], $dyn_alloc
  assert_equal [4], DynMissingInit.new(4).mm
end

assert('== found once still runs Hash/Array/user ==, recursion guards intact') do
  assert_true [{a: [1]}] == [{a: [1.0]}]
  assert_false [{a: [1]}] == [{a: [2]}]
  h1 = {}; h1[:a] = h1
  h2 = {}; h2[:a] = h2
  assert_nothing_raised { [h1] == [h2] }
  a1 = [1]; a1 << a1
  a2 = [1]; a2 << a2
  assert_nothing_raised { [a1] == [a2] }
  k = Class.new { def ==(o); :yes; end }
  assert_true [k.new] == [1]
  k.class_eval { def ==(o); false; end }
  assert_false [k.new] == [1]
  k.class_eval { private def ==(o); true; end }
  assert_true [k.new] == [1]                 # a C-level == ignores visibility, as the send did
end

assert('eql? found once still runs a user eql?') do
  seen = []
  k = Class.new do
    define_method(:eql?) { |o| seen << :eql; true }
    def hash; 1; end
  end
  a = k.new
  b = k.new
  assert_equal :v, {a => :v}[b]
  assert_true seen.include?(:eql)
  k.class_eval { def eql?(o); false; end }
  assert_nil({a => :v}[b])
end

assert('<=> found once still runs a user <=>') do
  k = Class.new do
    attr_reader :v
    def initialize(v); @v = v; end
    def <=>(o); o.v <=> @v; end              # descending
  end
  assert_equal [3, 2, 1], [k.new(1), k.new(3), k.new(2)].sort.map(&:v)
  k.class_eval { def <=>(o); @v <=> o.v; end }
  assert_equal [1, 2, 3], [k.new(1), k.new(3), k.new(2)].sort.map(&:v)
  assert_equal [[1, 2], [1, 3], [2]], [[2], [1, 3], [1, 2]].sort
end
