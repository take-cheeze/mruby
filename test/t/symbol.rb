##
# Symbol ISO Test

assert('Symbol') do
  assert_equal :"a", :a
  assert_equal :"a#{1}", :a1
  assert_equal :'a', :a
  assert_equal :'a#{1}', :"a\#{1}"
end

assert('Symbol', '15.2.11') do
  assert_equal Class, Symbol.class
end

assert('Symbol#===', '15.2.11.3.1') do
  assert_true :abc === :abc
  assert_false :abc === :cba
end

assert('Symbol#to_s', '15.2.11.3.3') do
  assert_equal  'abc', :abc.to_s
end

assert('Symbol#to_sym', '15.2.11.3.4') do
  assert_equal :abc, :abc.to_sym
end

assert('Symbol#to_proc') do
  assert_equal 5, :abs.to_proc[-5]
end

assert('Symbol#to_proc forwards arguments, keywords, block; ignores visibility') do
  c = Class.new do
    def priv(a, k: 1, &b); [a, k, b && b.call]; end
    private :priv
  end
  o = c.new
  pr = :priv.to_proc
  assert_equal [5, 3, 7], pr.call(o, 5, k: 3) { 7 }
  assert_equal [5, 1, nil], pr.call(o, 5)
  assert_raise(ArgumentError) { pr.call }
  assert_raise(ArgumentError) { :to_s.to_proc.call(1, 2, 3) }
  assert_equal [[1, 2]], [[[1, 2]]].map(&:first)
  assert_equal ["A", "B"], %w[a b].map(&:upcase)
end

assert('Symbol#to_proc uses method_missing') do
  o = Object.new
  def o.method_missing(n, *a); [n, a]; end
  assert_equal [:zork, [1]], :zork.to_proc.call(o, 1)
  assert_raise(NoMethodError) { [1].each(&:no_such_method) }
end

assert('Symbol#to_proc sending :call to a cfunc proc') do
  # funcall of Proc#call whose body is a cfunc must return to C, not resume a bytecode frame
  assert_equal "A", :call.to_proc.call(:upcase.to_proc, "a")
  assert_equal [1, 2], [lambda { 1 }, lambda { 2 }].map(&:call)
end

assert('Symbol table with many symbols') do
  # Past the linear-search threshold the table is hashed; a bucket count fixed at 256 made every lookup walk a chain
  # proportional to the number of symbols.
  syms = []
  5000.times { |i| syms << "many_symbols_test_#{i}".to_sym }
  seen = {}
  syms.each { |s| seen[s] = true }
  assert_equal 5000, seen.size
  5000.times do |i|
    name = "many_symbols_test_#{i}"
    assert_same syms[i], name.to_sym if i % 97 == 0
    assert_equal name, syms[i].to_s if i % 89 == 0
  end
end
