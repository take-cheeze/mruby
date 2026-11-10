# Array/Hash == run without a frame; a cycle is cut by the pairs on the C stack.
assert('recursive Array == terminates') do
  a = [1]; a << a
  b = [1]; b << b
  assert_false a == b
  assert_true a == a
  assert_true [1, [2, [3]]] == [1, [2, [3]]]
  assert_false [1, [2, [3]]] == [1, [2, [4]]]
end

assert('recursive Hash == terminates') do
  h = {a: 1}; h[:b] = h
  g = {a: 1}; g[:b] = g
  assert_false h == g
  assert_true({a: [1, {b: 2}]} == {a: [1, {b: 2}]})
end

assert('mixed Integer/Float elements') do
  assert_true [1, {a: 1}] == [1.0, {a: 1.0}]
  assert_false [1, 2] == [1, 2.5]
end

assert('a cycle through a == written in Ruby terminates') do
  c = Class.new do
    attr_accessor :arr
    def ==(o) = @arr == o.arr
  end
  o1 = c.new; a1 = [o1]; o1.arr = a1
  o2 = c.new; a2 = [o2]; o2.arr = a2
  assert_false a1 == a2
end

assert('a raise in an element == leaves no state behind') do
  r = Class.new { def ==(o) = raise("boom") }
  x = [r.new]; y = [r.new]
  2.times { assert_raise(RuntimeError) { x == y } }
  p1 = [1, 2]; p2 = [1, 2]
  assert_true p1 == p2
  c1 = [1]; c1 << c1; c2 = [1]; c2 << c2
  assert_false c1 == c2
  x << x; y << y
  assert_raise(RuntimeError) { x == y }
  assert_false c1 == c2
end

assert('a == written in Ruby is still dispatched') do
  sub = Class.new(Array) { def ==(o) = true }
  assert_true [sub.new] == [[1]]
  assert_true({k: sub.new} == {k: [1]})
  one = [1]
  def one.==(o) = true
  assert_true [one] == [[2]]
  assert_false [[1]] == [[2]]
end
