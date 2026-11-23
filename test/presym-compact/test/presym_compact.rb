assert('compact presym table: names sharing more than 63 bytes') do
  assert_equal [true] * 8, presym_long_names
end
