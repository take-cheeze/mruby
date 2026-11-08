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
  # Wide arrays and hashes hold far more gray children than the gray stack has room for, so marking depends on the
  # heap rescan; every element must survive repeated collections.
  live = Array.new(20) { |c| Array.new(3000) { |i| [c, i, "v#{c}:#{i}"] } }
  hs = Array.new(6) { |c| h = {}; 2000.times { |i| h[i] = "x#{c}-#{i}" }; h }
  6.times do |r|
    GC.start
    5000.times { |i| [i, r.to_s] }
  end
  bad = 0
  live.each_with_index { |a, c| a.each_with_index { |e, i| bad += 1 unless e[0] == c && e[1] == i && e[2] == "v#{c}:#{i}" } }
  hs.each_with_index { |h, c| h.each { |i, v| bad += 1 unless v == "x#{c}-#{i}" } }
  assert_equal 0, bad
end
