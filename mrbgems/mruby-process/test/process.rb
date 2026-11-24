assert('Process.clock_gettime returns a Float by default') do
  t = Process.clock_gettime(Process::CLOCK_MONOTONIC)
  assert_true t.is_a?(Float)
end

assert('Process.clock_gettime units') do
  s = Process.clock_gettime(Process::CLOCK_REALTIME, :second)
  assert_true s.is_a?(Integer)
  assert_true Process.clock_gettime(Process::CLOCK_REALTIME, :nanosecond).is_a?(Integer)
end

assert('Process.clock_gettime rejects an unknown unit') do
  assert_raise(ArgumentError) { Process.clock_gettime(Process::CLOCK_REALTIME, :bogus) }
end

assert('Process.clock_gettime monotonic does not go back') do
  a = Process.clock_gettime(Process::CLOCK_MONOTONIC)
  b = Process.clock_gettime(Process::CLOCK_MONOTONIC)
  assert_true b >= a
end

assert('Process.pid is an Integer') do
  assert_true Process.pid.is_a?(Integer)
end
