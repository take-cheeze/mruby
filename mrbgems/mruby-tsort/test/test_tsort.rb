# frozen_string_literal: true

# require 'tsort'
# require 'test/unit'

class TSortHash < Hash # :nodoc:
  include TSort
  alias tsort_each_node each_key
  def tsort_each_child(node, &block)
    self[node].each(&block)
  end

  def self.[](h)
    self.new.replace h
  end
end

class TSortArray < Array # :nodoc:
  include TSort
  alias tsort_each_node each_index
  def tsort_each_child(node, &block)
    __t_printstr__ node.inspect
    fetch(node).each(&block)
  end

  def self.[](*args)
    self.new(args.size) {|i| args[i] }
  end
end

assert 'dag' do
  h = TSortHash[{1=>[2, 3], 2=>[3], 3=>[]}]
  assert_equal([3, 2, 1], h.tsort)
  assert_equal([[3], [2], [1]], h.strongly_connected_components)
end

assert 'cycle' do
  h = TSortHash[{1=>[2], 2=>[3, 4], 3=>[2], 4=>[]}]
  assert_equal([[4], [2, 3], [1]],
               h.strongly_connected_components.map {|nodes| nodes.sort})
  assert_raise(TSort::Cyclic) { h.tsort }
end

assert 'array' do
  a = TSortArray[[1], [0], [0], [2]]
  assert_equal([[0, 1], [2], [3]],
               a.strongly_connected_components.map {|nodes| nodes.sort})

  a = TSortArray[[], [0]]
  assert_equal([[0], [1]],
               a.strongly_connected_components.map {|nodes| nodes.sort})
end

assert 's_tsort' do
  g = {1=>[2, 3], 2=>[4], 3=>[2, 4], 4=>[]}
  each_node = lambda {|&b| g.each_key(&b) }
  each_child = lambda {|n, &b| g[n].each(&b) }
  assert_equal([4, 2, 3, 1], TSort.tsort(each_node, each_child))
  g = {1=>[2], 2=>[3, 4], 3=>[2], 4=>[]}
  assert_raise(TSort::Cyclic) { TSort.tsort(each_node, each_child) }
end

assert 's_tsort_each' do
  g = {1=>[2, 3], 2=>[4], 3=>[2, 4], 4=>[]}
  each_node = lambda {|&b| g.each_key(&b) }
  each_child = lambda {|n, &b| g[n].each(&b) }
  r = []
  TSort.tsort_each(each_node, each_child) {|n| r << n }
  assert_equal([4, 2, 3, 1], r)

  r = TSort.tsort_each(each_node, each_child).map {|n| n.to_s }
  assert_equal(['4', '2', '3', '1'], r)
end

assert 's_strongly_connected_components' do
  g = {1=>[2, 3], 2=>[4], 3=>[2, 4], 4=>[]}
  each_node = lambda {|&b| g.each_key(&b) }
  each_child = lambda {|n, &b| g[n].each(&b) }
  assert_equal([[4], [2], [3], [1]],
               TSort.strongly_connected_components(each_node, each_child))
  g = {1=>[2], 2=>[3, 4], 3=>[2], 4=>[]}
  assert_equal([[4], [2, 3], [1]],
               TSort.strongly_connected_components(each_node, each_child))
end

assert 's_each_strongly_connected_component' do
  g = {1=>[2, 3], 2=>[4], 3=>[2, 4], 4=>[]}
  each_node = lambda {|&b| g.each_key(&b) }
  each_child = lambda {|n, &b| g[n].each(&b) }
  r = []
  TSort.each_strongly_connected_component(each_node, each_child) {|scc|
    r << scc
  }
  assert_equal([[4], [2], [3], [1]], r)
  g = {1=>[2], 2=>[3, 4], 3=>[2], 4=>[]}
  r = []
  TSort.each_strongly_connected_component(each_node, each_child) {|scc|
    r << scc
  }
  assert_equal([[4], [2, 3], [1]], r)

  r = TSort.each_strongly_connected_component(each_node, each_child).map {|scc|
    scc.map(&:to_s)
  }
  assert_equal([['4'], ['2', '3'], ['1']], r)
end

assert 's_each_strongly_connected_component_from' do
  g = {1=>[2], 2=>[3, 4], 3=>[2], 4=>[]}
  each_child = lambda {|n, &b| g[n].each(&b) }
  r = []
  TSort.each_strongly_connected_component_from(1, each_child) {|scc|
    r << scc
  }
  assert_equal([[4], [2, 3], [1]], r)

  r = TSort.each_strongly_connected_component_from(1, each_child).map {|scc|
    scc.map(&:to_s)
  }
  assert_equal([['4'], ['2', '3'], ['1']], r)
end

