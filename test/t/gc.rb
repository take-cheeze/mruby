# Not ISO specified

assert('GC.enable') do
  assert_false GC.disable
  assert_true GC.enable
  assert_false GC.enable
end

assert('GC.disable') do
  begin
    assert_false GC.disable
    assert_true GC.disable
  ensure
    GC.enable
  end
end

assert('GC.interval_ratio=') do
  origin = GC.interval_ratio
  begin
    assert_equal 150, (GC.interval_ratio = 150)
  ensure
    GC.interval_ratio = origin
  end
end

assert('GC.step_ratio=') do
  origin = GC.step_ratio
  begin
    assert_equal 150, (GC.step_ratio = 150)
  ensure
    GC.step_ratio = origin
  end
end

assert('GC.generational_mode=') do
  origin = GC.generational_mode
assert('GC.step_ratio= / GC.interval_ratio= reject an out-of-int-range value') do
  # The ratios are stored in a C int. A value past its range must be rejected,
  # not truncated: truncating 2**32 to 0 leaves the incremental collector with
  # a zero step budget and it stops making progress.
  origin_s = GC.step_ratio
  origin_i = GC.interval_ratio
  begin
    huge = 1 << 40 # fits a 64-bit mrb_int, overflows a 32-bit C int
  rescue RangeError
    skip 'mrb_int is not wider than a C int on this build'
  end
  begin
    # On a wide mrb_int the setter's own bounds check rejects it (ArgumentError);
    # on a narrower one the argument conversion rejects it first (RangeError).
    assert_raise(ArgumentError, RangeError) { GC.step_ratio = huge }
    assert_raise(ArgumentError, RangeError) { GC.interval_ratio = huge }
  ensure
    GC.step_ratio = origin_s
    GC.interval_ratio = origin_i
  end
end

  begin
    assert_false (GC.generational_mode = false)
    assert_true (GC.generational_mode = true)
    assert_true (GC.generational_mode = true)
  ensure
    GC.generational_mode = origin
  end
end

assert('GC marks a heap whose gray objects overflow the gray stack') do
  # One array and one hash with more children than the 1024-entry gray stack holds, so marking depends on the heap
  # rescan; every element must survive collections. (Small on purpose: MRB_GC_STRESS collects at every allocation.)
  live = Array.new(1200) { |i| "v#{i}" }
  h = {}
  1200.times { |i| h[i] = "x#{i}" }
  3.times do
    GC.start
    50.times { |i| [i, i.to_s] }
  end
  bad = 0
  live.each_with_index { |s, i| bad += 1 unless s == "v#{i}" }
  h.each { |i, v| bad += 1 unless v == "x#{i}" }
  assert_equal 0, bad
end
assert('OP_ENTER keeps the block alive while it lays out a short argument list') do
  # A call that passes fewer positional arguments than the `*rest` and post
  # parameters span has its post arguments moved to the end of that span,
  # and when nothing but the required arguments came, or they came packed in
  # one array with a keyword hash after it, that is the register the block
  # arrived in.  The empty `rest` is allocated right after, with the block
  # held in a C local and nowhere the marker looks, so a collection landing
  # on that allocation freed the `Proc`, and what the method then read as its
  # block was whatever the cell was reused for.  Enough calls for the
  # collector to land there.
  def gc_test_short_args(x, *r, y, &blk); blk; end
  def gc_test_short_args_kw(x, *r, y, **o, &blk); blk; end
  args = [1, 2]
  kw = {}
  bad = 0
  i = 0
  while i < 5000
    b = gc_test_short_args(1, 2) { :c }
    bad += 1 unless b.call == :c
    b = gc_test_short_args_kw(*args, **kw) { :c }
    bad += 1 unless b.call == :c
    i += 1
  end
  assert_equal 0, bad
end
