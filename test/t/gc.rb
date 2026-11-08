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
