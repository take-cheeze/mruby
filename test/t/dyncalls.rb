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

# hash and initialize_copy run the method they find, in the frame mrb_funcall_argv would push; the cases below are the
# ones that differ from a plain call: a method that is missing, private, prepended, a cfunc other than Kernel's, or a
# different one for each key class.
class DynMissingHash < DynKey
  undef_method :hash
  def method_missing(n, *a); n == :hash ? @v * 7 : super; end
  def respond_to_missing?(n, p = false); n == :hash || super; end
  def eql?(o); o.is_a?(DynMissingHash) && @v == o.v; end
end

class DynPrivateHash < DynKey
  def eql?(o); o.is_a?(DynPrivateHash) && @v == o.v; end
  private def hash; @v + 100; end
end

module DynPrependedHash
  def hash; $dyn_pre << :hash; super; end
end

class DynPrependedKey < DynKey
  prepend DynPrependedHash
  def hash; @v.hash; end
  def eql?(o); o.is_a?(DynPrependedKey) && @v == o.v; end
end

class DynRaisingHash < DynKey
  def hash; raise ArgumentError, "dyn hash"; end
end

assert('Hash key hash: undefined hash goes to method_missing') do
  h = dyn_big_hash
  h[DynMissingHash.new(3)] = :m
  assert_equal :m, h[DynMissingHash.new(3)]
  assert_nil h[DynMissingHash.new(4)]
  assert_equal 41, h.size
end

assert('Hash key hash: a private hash is called') do
  h = dyn_big_hash
  h[DynPrivateHash.new(5)] = :p
  assert_equal :p, h[DynPrivateHash.new(5)]
  assert_nil h[DynPrivateHash.new(6)]
end

assert('Hash key hash: a prepended hash wraps the builtin') do
  $dyn_pre = []
  h = dyn_big_hash
  h[DynPrependedKey.new(1)] = :q
  assert_equal :q, h[DynPrependedKey.new(1)]
  assert_equal [:hash, :hash], $dyn_pre
end

assert('Hash key hash: an exception from hash reaches the caller') do
  h = dyn_big_hash
  assert_raise(ArgumentError) { h[DynRaisingHash.new(1)] = 1 }
  assert_raise(ArgumentError) { h[DynRaisingHash.new(1)] }
  assert_equal 40, h.size
end

assert('Hash key hash: keys of different classes in one Hash') do
  h = dyn_big_hash
  keys = [DynKeyH.new(1), DynKey.new(2), DynPrivateHash.new(3), DynMissingHash.new(4), DynKeyH.new(5), DynKey.new(6)]
  keys.each_with_index { |k, i| h[k] = i }
  keys.each_with_index { |k, i| assert_equal i, h[k] }
  assert_equal 46, h.size
  assert_equal :v, ({DynKeyH.new(9) => :v})[DynKeyH.new(9)]
end

assert('Hash key hash: a cfunc hash other than Kernel#hash') do
  h = dyn_big_hash
  h[1..2] = :r
  assert_equal :r, h[1..2]
  assert_nil h[2..1]
  h[[1, [2]]] = :a
  assert_equal :a, h[[1, [2]]]
  assert_equal 42, h.size
end

assert('Hash key hash: redefining hash on the class re-hashes new keys') do
  c = Class.new(DynKey) do
    def eql?(o); o.is_a?(self.class) && @v == o.v; end
  end
  h = dyn_big_hash
  a = c.new(1)
  h[a] = 1
  assert_equal 1, h[a]
  calls = []
  $dyn_redef = calls
  c.class_eval { def hash; $dyn_redef << :new; 11; end }
  b = c.new(2)
  h[b] = 2
  assert_equal [:new], calls
  assert_equal 2, h[b]
  calls.clear
  c.class_eval { def hash; $dyn_redef << :newer; 12; end }
  d = c.new(3)
  h[d] = 3
  assert_equal [:newer], calls
  assert_equal 3, h[d]
end

class DynCopyMissing
  def initialize(v); @v = v; end
  attr_reader :v
  undef_method :initialize_copy
  def method_missing(n, *a)
    return (@v = :via_method_missing) if n == :initialize_copy
    super
  end
  def respond_to_missing?(n, p = false); n == :initialize_copy || super; end
end

class DynCopyPrivate
  attr_reader :log
  def initialize; @log = []; end
  private def initialize_copy(o); @log = o.log + [:private]; super; end
end

module DynCopyPrepended
  def initialize_copy(o); $dyn_copy << :pre; super; end
end
class DynCopyPre
  prepend DynCopyPrepended
end

class DynCopyRaises
  def initialize_copy(o); raise IndexError, "dyn copy"; end
end

assert('initialize_copy: undefined goes to method_missing') do
  d = DynCopyMissing.new(1).dup
  assert_equal :via_method_missing, d.v
  assert_equal :via_method_missing, DynCopyMissing.new(2).clone.v
end

assert('initialize_copy: a private user method and a prepended one') do
  assert_equal [:private], DynCopyPrivate.new.dup.log
  assert_equal [:private], DynCopyPrivate.new.clone.log
  $dyn_copy = []
  DynCopyPre.new.dup
  DynCopyPre.new.clone
  assert_equal [:pre, :pre], $dyn_copy
end

assert('initialize_copy: an exception reaches the caller') do
  assert_raise(IndexError) { DynCopyRaises.new.dup }
  assert_raise(IndexError) { DynCopyRaises.new.clone }
end

assert('initialize_copy: a singleton override is used for that object only') do
  a = Object.new
  b = Object.new
  def a.initialize_copy(o); $dyn_sing << :a; super; end
  $dyn_sing = []
  a.clone                            # a clone carries the singleton class, so its initialize_copy
  b.clone
  assert_equal [:a], $dyn_sing
  $dyn_sing = []
  a.dup                              # a dup does not
  assert_equal [], $dyn_sing
end

# respond_to_missing?, an Array key's eql? and == keep one answer per receiver type, so receivers of different types
# interleave without losing a user override on one of them; classes of one type still evict each other correctly.
class DynRtmA
  def respond_to_missing?(n, p = false); n == :zork || super; end
end
class DynRtmB
  def respond_to_missing?(n, p = false); n == :quux || super; end
end

assert('respond_to?: respond_to_missing? with receivers of several types interleaved') do
  a = DynRtmA.new
  b = DynRtmB.new
  3.times do
    assert_false 1.respond_to?(:zork)
    assert_true  a.respond_to?(:zork)
    assert_false "s".respond_to?(:zork)
    assert_false b.respond_to?(:zork)       # same type as a, different class
    assert_true  b.respond_to?(:quux)
    assert_false a.respond_to?(:quux)
    assert_false nil.respond_to?(:zork)
    assert_false [].respond_to?(:zork)
    assert_false :s.respond_to?(:zork)
  end
end

assert('respond_to?: a respond_to_missing? defined after use, and a singleton one') do
  a = DynRtmA.new
  o = Object.new
  assert_false o.respond_to?(:late)
  assert_false a.respond_to?(:late)
  DynRtmA.class_eval { def respond_to_missing?(n, p = false); n == :late || super; end }
  assert_true  a.respond_to?(:late)
  assert_false o.respond_to?(:late)
  def o.respond_to_missing?(n, p = false); n == :late; end
  assert_true  o.respond_to?(:late)
  assert_false Object.new.respond_to?(:late)
  assert_false 1.respond_to?(:late)
end

class DynNeverEql < String
  def eql?(o); false; end
end
class DynAnyEql < String
  def eql?(o); true; end
end

assert('Hash key: Array keys whose elements are of several types and eql? overrides') do
  h = dyn_big_hash
  h[[1, :a, "s", 2.5, nil]] = :plain
  assert_equal :plain, h[[1, :a, "s", 2.5, nil]]
  assert_nil h[[1, :a, "s", 2.5, false]]
  n = DynNeverEql.new("a")
  h[[1, n, :s]] = :never
  assert_nil h[[1, DynNeverEql.new("a"), :s]]        # eql? is false, so the key is not found
  assert_nil h[[1, n, :s]]                           # not even with the same element: Array#eql? asks eql?
  h[[:x, DynAnyEql.new("a"), 3]] = :any
  assert_equal :any, h[[:x, DynAnyEql.new("a"), 3]]
  assert_nil h[[:x, DynAnyEql.new("a"), 4]]
  3.times { assert_equal :plain, h[[1, :a, "s", 2.5, nil]] }
end

assert('==: receivers of several types alternate with a user ==') do
  u = DynEqCount.new(true)
  3.times do
    assert_true  [1, "a", u, :s, 2.5] == [1.0, "a", DynEqCount.new(false), :s, 2.5]
    assert_false [1, "a", :s] == [1, "b", :s]
    assert_true  [nil, true, false] == [nil, true, false]
  end
  assert_equal 3, u.calls
end

# convert_type searches once and runs what it found; a missing converter still raises or answers nil, and a private
# or method_missing-less converter behaves as before.
class DynToProc
  def to_proc; Proc.new { |x| x * 2 }; end
end
class DynPrivToProc
  private def to_proc; Proc.new { |x| x + 1 }; end
end

assert('&obj converts with to_proc: public, private, missing') do
  assert_equal [2, 4], [1, 2].map(&DynToProc.new)
  assert_equal [2, 3], [1, 2].map(&DynPrivToProc.new)
  assert_raise(TypeError) { [1].map(&Object.new) }
end
