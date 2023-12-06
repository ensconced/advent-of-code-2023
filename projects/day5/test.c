#include "./lib.h"
#include <assert.h>
#include <stdio.h>

void assert_almanacs_are_equal(Almanac a, Almanac b) {
  assert(a.seeds_len == b.seeds_len);
  assert(a.range_maps_len == b.range_maps_len);
  for (size_t i = 0; i < a.seeds_len; ++i) {
    assert(a.seeds[i] == b.seeds[i]);
  }
  for (size_t i = 0; i < a.range_maps_len; ++i) {
    assert(a.range_maps[i].ranges_len == b.range_maps[i].ranges_len);
    for (size_t j = 0; j < a.range_maps[i].ranges_len; ++j) {
      Range a_range = a.range_maps[i].ranges[j];
      Range b_range = b.range_maps[i].ranges[j];
      assert(a_range.dest_range_start == b_range.dest_range_start);
      assert(a_range.source_range_start == b_range.source_range_start);
      assert(a_range.range_len == b_range.range_len);
    }
  }
}

void test_parse_input(void) {
  static const size_t expected_seeds_len = 4;
  static const size_t expected_range_map_len = 7;

  static const size_t expected_seed_to_soil_ranges_len = 2;
  Range expected_seed_to_soil_ranges[expected_seed_to_soil_ranges_len] = {
      (Range){
          .dest_range_start = 50,
          .source_range_start = 98,
          .range_len = 2,
      },
      (Range){
          .dest_range_start = 52,
          .source_range_start = 50,
          .range_len = 48,
      },
  };
  RangeMap expected_seed_to_soil_range_map = {
      .ranges = expected_seed_to_soil_ranges,
      .ranges_len = expected_seed_to_soil_ranges_len,
  };

  static const size_t expected_soil_to_fertilizer_ranges_len = 3;
  Range expected_soil_to_fertilizer_ranges
      [expected_soil_to_fertilizer_ranges_len] = {
          (Range){
              .dest_range_start = 0,
              .source_range_start = 15,
              .range_len = 37,
          },
          (Range){
              .dest_range_start = 37,
              .source_range_start = 52,
              .range_len = 2,
          },
          (Range){
              .dest_range_start = 39,
              .source_range_start = 0,
              .range_len = 15,
          },
      };
  RangeMap expected_soil_to_fertilizer_range_map = {
      .ranges = expected_soil_to_fertilizer_ranges,
      .ranges_len = expected_soil_to_fertilizer_ranges_len,
  };

  static const size_t expected_fertilizer_to_water_ranges_len = 4;
  Range expected_fertilizer_to_water_ranges
      [expected_fertilizer_to_water_ranges_len] = {
          (Range){
              .dest_range_start = 49,
              .source_range_start = 53,
              .range_len = 8,
          },
          (Range){
              .dest_range_start = 0,
              .source_range_start = 11,
              .range_len = 42,
          },
          (Range){
              .dest_range_start = 42,
              .source_range_start = 0,
              .range_len = 7,
          },
          (Range){
              .dest_range_start = 57,
              .source_range_start = 7,
              .range_len = 4,
          },
      };
  RangeMap expected_fertilizer_to_water_range_map = {
      .ranges = expected_fertilizer_to_water_ranges,
      .ranges_len = expected_fertilizer_to_water_ranges_len,
  };

  static const size_t expected_water_to_light_ranges_len = 2;
  Range expected_water_to_light_ranges[expected_water_to_light_ranges_len] = {
      (Range){
          .dest_range_start = 88,
          .source_range_start = 18,
          .range_len = 7,
      },
      (Range){
          .dest_range_start = 18,
          .source_range_start = 25,
          .range_len = 70,
      },
  };
  RangeMap expected_water_to_light_range_map = {
      .ranges = expected_water_to_light_ranges,
      .ranges_len = expected_water_to_light_ranges_len,
  };

  static const size_t expected_light_to_temperature_ranges_len = 3;
  Range expected_light_to_temperature_ranges
      [expected_light_to_temperature_ranges_len] = {
          (Range){
              .dest_range_start = 45,
              .source_range_start = 77,
              .range_len = 23,
          },
          (Range){
              .dest_range_start = 81,
              .source_range_start = 45,
              .range_len = 19,
          },
          (Range){
              .dest_range_start = 68,
              .source_range_start = 64,
              .range_len = 13,
          },
      };
  RangeMap expected_light_to_temperature_range_map = {
      .ranges = expected_light_to_temperature_ranges,
      .ranges_len = expected_light_to_temperature_ranges_len,
  };

  static const size_t expected_temperature_to_humidity_ranges_len = 2;
  Range expected_temperature_to_humidity_ranges
      [expected_temperature_to_humidity_ranges_len] = {
          (Range){
              .dest_range_start = 0,
              .source_range_start = 69,
              .range_len = 1,
          },
          (Range){
              .dest_range_start = 1,
              .source_range_start = 0,
              .range_len = 69,
          },
      };
  RangeMap expected_temperature_to_humidity_range_map = {
      .ranges = expected_temperature_to_humidity_ranges,
      .ranges_len = expected_temperature_to_humidity_ranges_len,
  };

  static const size_t expected_humidity_to_location_ranges_len = 2;
  Range expected_humidity_to_location_ranges
      [expected_humidity_to_location_ranges_len] = {
          (Range){
              .dest_range_start = 60,
              .source_range_start = 56,
              .range_len = 37,
          },
          (Range){
              .dest_range_start = 56,
              .source_range_start = 93,
              .range_len = 4,
          },
      };
  RangeMap expected_humidity_to_location_range_map = {
      .ranges = expected_humidity_to_location_ranges,
      .ranges_len = expected_humidity_to_location_ranges_len,
  };

  RangeMap range_maps[expected_range_map_len] = {
      expected_seed_to_soil_range_map,
      expected_soil_to_fertilizer_range_map,
      expected_fertilizer_to_water_range_map,
      expected_water_to_light_range_map,
      expected_light_to_temperature_range_map,
      expected_temperature_to_humidity_range_map,
      expected_humidity_to_location_range_map,
  };

  unsigned long expected_seeds[expected_seeds_len] = {79, 14, 55, 13};

  Almanac expected_almanac = {
      .seeds = expected_seeds,
      .seeds_len = expected_seeds_len,
      .range_maps = range_maps,
      .range_maps_len = expected_range_map_len,
  };

  Almanac almanac = parse_input("./inputs/example_input.txt");
  assert_almanacs_are_equal(almanac, expected_almanac);
}

int main(void) {
  test_parse_input();
  assert(part1_lowest_location(parse_input("./inputs/example_input.txt")) ==
         35);
  assert(part1_lowest_location(parse_input("./inputs/input.txt")) == 346433842);
  printf("part 2 example input result: %lu\n",
         part2_lowest_location(parse_input("./inputs/example_input.txt")));
  //   assert(part2_lowest_location(parse_input("./inputs/example_input.txt"))
  //   ==
  //          46);
  //   assert(part2_lowest_location(parse_input("./inputs/input.txt")) ==
  //   60294664);
}
