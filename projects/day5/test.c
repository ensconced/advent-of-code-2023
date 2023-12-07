#include "./lib.h"
#include "./parser.h"
#include <assert.h>
#include <stdio.h>

void assert_almanacs_are_equal(Almanac a, Almanac b) {
  assert(a.seeds_len == b.seeds_len);
  assert(a.range_maps_len == b.range_maps_len);
  for (size_t i = 0; i < a.seeds_len; ++i) {
    assert(a.seeds[i] == b.seeds[i]);
  }
  for (size_t i = 0; i < a.range_maps_len; ++i) {
    assert(a.range_maps[i].interval_maps_len ==
           b.range_maps[i].interval_maps_len);
    for (size_t j = 0; j < a.range_maps[i].interval_maps_len; ++j) {
      IntervalMap a_range = a.range_maps[i].interval_maps[j];
      IntervalMap b_range = b.range_maps[i].interval_maps[j];
      assert(interval_equal(a_range.src_interval, b_range.src_interval));
      assert(interval_equal(a_range.dst_interval, b_range.dst_interval));
    }
  }
}

void test_parse_input(void) {
  static const size_t expected_seeds_len = 4;
  static const size_t expected_range_map_len = 7;

  static const size_t expected_seed_to_soil_ranges_len = 2;
  IntervalMap expected_seed_to_soil_ranges[expected_seed_to_soil_ranges_len] = {
      (IntervalMap){
          .src_interval = (interval_interval){.start = 98, .end = 100},
          .dst_interval = (interval_interval){.start = 50, .end = 52},
      },
      (IntervalMap){
          .src_interval = (interval_interval){.start = 50, .end = 98},
          .dst_interval = (interval_interval){.start = 52, .end = 100},
      },
  };
  RangeMap expected_seed_to_soil_range_map = {
      .interval_maps = expected_seed_to_soil_ranges,
      .interval_maps_len = expected_seed_to_soil_ranges_len,
  };

  static const size_t expected_soil_to_fertilizer_ranges_len = 3;
  IntervalMap expected_soil_to_fertilizer_ranges
      [expected_soil_to_fertilizer_ranges_len] = {
          (IntervalMap){
              .src_interval = (interval_interval){.start = 15, .end = 52},
              .dst_interval = (interval_interval){.start = 0, .end = 37},
          },
          (IntervalMap){
              .src_interval = (interval_interval){.start = 52, .end = 54},
              .dst_interval = (interval_interval){.start = 37, .end = 39},
          },
          (IntervalMap){
              .src_interval = (interval_interval){.start = 0, .end = 15},
              .dst_interval = (interval_interval){.start = 39, .end = 54},
          },
      };
  RangeMap expected_soil_to_fertilizer_range_map = {
      .interval_maps = expected_soil_to_fertilizer_ranges,
      .interval_maps_len = expected_soil_to_fertilizer_ranges_len,
  };

  static const size_t expected_fertilizer_to_water_ranges_len = 4;
  IntervalMap expected_fertilizer_to_water_ranges
      [expected_fertilizer_to_water_ranges_len] = {
          (IntervalMap){
              .src_interval = (interval_interval){.start = 53, .end = 61},
              .dst_interval = (interval_interval){.start = 49, .end = 57},
          },
          (IntervalMap){
              .src_interval = (interval_interval){.start = 11, .end = 53},
              .dst_interval = (interval_interval){.start = 0, .end = 42},
          },
          (IntervalMap){
              .src_interval = (interval_interval){.start = 0, .end = 7},
              .dst_interval = (interval_interval){.start = 42, .end = 49},
          },
          (IntervalMap){
              .src_interval = (interval_interval){.start = 7, .end = 11},
              .dst_interval = (interval_interval){.start = 57, .end = 61},
          },
      };
  RangeMap expected_fertilizer_to_water_range_map = {
      .interval_maps = expected_fertilizer_to_water_ranges,
      .interval_maps_len = expected_fertilizer_to_water_ranges_len,
  };

  static const size_t expected_water_to_light_ranges_len = 2;
  IntervalMap
      expected_water_to_light_ranges[expected_water_to_light_ranges_len] = {
          (IntervalMap){
              .src_interval = (interval_interval){.start = 18, .end = 25},
              .dst_interval = (interval_interval){.start = 88, .end = 95},
          },
          (IntervalMap){
              .src_interval = (interval_interval){.start = 25, .end = 95},
              .dst_interval = (interval_interval){.start = 18, .end = 88},
          },
      };
  RangeMap expected_water_to_light_range_map = {
      .interval_maps = expected_water_to_light_ranges,
      .interval_maps_len = expected_water_to_light_ranges_len,
  };

  static const size_t expected_light_to_temperature_ranges_len = 3;
  IntervalMap expected_light_to_temperature_ranges
      [expected_light_to_temperature_ranges_len] = {
          (IntervalMap){
              .src_interval = (interval_interval){.start = 77, .end = 100},
              .dst_interval = (interval_interval){.start = 45, .end = 68},
          },
          (IntervalMap){
              .src_interval = (interval_interval){.start = 45, .end = 64},
              .dst_interval = (interval_interval){.start = 81, .end = 100},
          },
          (IntervalMap){
              .src_interval = (interval_interval){.start = 64, .end = 77},
              .dst_interval = (interval_interval){.start = 68, .end = 81},
          },
      };
  RangeMap expected_light_to_temperature_range_map = {
      .interval_maps = expected_light_to_temperature_ranges,
      .interval_maps_len = expected_light_to_temperature_ranges_len,
  };

  static const size_t expected_temperature_to_humidity_ranges_len = 2;
  IntervalMap expected_temperature_to_humidity_ranges
      [expected_temperature_to_humidity_ranges_len] = {
          (IntervalMap){
              .src_interval = (interval_interval){.start = 69, .end = 70},
              .dst_interval = (interval_interval){.start = 0, .end = 1},
          },
          (IntervalMap){
              .src_interval = (interval_interval){.start = 0, .end = 69},
              .dst_interval = (interval_interval){.start = 1, .end = 70},
          },
      };
  RangeMap expected_temperature_to_humidity_range_map = {
      .interval_maps = expected_temperature_to_humidity_ranges,
      .interval_maps_len = expected_temperature_to_humidity_ranges_len,
  };

  static const size_t expected_humidity_to_location_ranges_len = 2;
  IntervalMap expected_humidity_to_location_ranges
      [expected_humidity_to_location_ranges_len] = {
          (IntervalMap){
              .src_interval = (interval_interval){.start = 56, .end = 93},
              .dst_interval = (interval_interval){.start = 60, .end = 97},
          },
          (IntervalMap){
              .src_interval = (interval_interval){.start = 93, .end = 97},
              .dst_interval = (interval_interval){.start = 56, .end = 60},
          },
      };
  RangeMap expected_humidity_to_location_range_map = {
      .interval_maps = expected_humidity_to_location_ranges,
      .interval_maps_len = expected_humidity_to_location_ranges_len,
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
  assert(part2_lowest_location(parse_input("./inputs/example_input.txt")) ==
         46);
  //   assert(part2_lowest_location(parse_input("./inputs/input.txt")) ==
  //   60294664);
}
