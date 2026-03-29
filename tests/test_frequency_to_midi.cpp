#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest/doctest.h>

#include <stdexcept>

#include "frequency_to_midi.hpp"

TEST_CASE("frequency_to_midi valid cases") {
  SUBCASE("exact frequencies") {
    CHECK(frequency_to_midi(440.0) == 69);
    CHECK(frequency_to_midi(82.41) == 40);
    CHECK(frequency_to_midi(1174.7) == 86);
    CHECK(frequency_to_midi(146.83) == 50);
  }

  SUBCASE("imprecise frequencies") {
    CHECK(frequency_to_midi(450.0) == 69);
    CHECK(frequency_to_midi(80.5) == 40);
    CHECK(frequency_to_midi(1205.7) == 86);
    CHECK(frequency_to_midi(143.0) == 50);
  }

  SUBCASE("rest") { CHECK(frequency_to_midi(0.0) == -1); }
}

TEST_CASE("frequency_to_midi exceptions") {
  SUBCASE("negative frequency") {
    CHECK_THROWS_AS(frequency_to_midi(-100.0), std::invalid_argument);
  }

  SUBCASE("frequency out of guitar range") {
    CHECK_THROWS_AS(frequency_to_midi(10.0), std::invalid_argument);
    CHECK_THROWS_AS(frequency_to_midi(10000.0), std::invalid_argument);
  }
}

TEST_CASE("duration_sum correct input") {
  SUBCASE("empty vector") {
    std::vector<NoteDuration> input;
    CHECK(duration_sum(input) == doctest::Approx(0.0));
  }

  SUBCASE("basic") {
    std::vector<NoteDuration> input = {
        {0, 60, 0.5}, {1, 62, 1.0}, {2, 64, 0.25}};
    CHECK(duration_sum(input) == doctest::Approx(1.75));
  }
}

TEST_CASE("max_duration correct input") {
  std::vector<NoteDuration> input = {{0, 60, 0.5}, {1, 62, 1.2}, {2, 64, 0.8}};
  CHECK(max_duration(input) == 1);
}

TEST_CASE("max_duration exceptions (empty vector)") {
  std::vector<NoteDuration> empty;
  CHECK_THROWS_AS(max_duration(empty), std::invalid_argument);
}

TEST_CASE("get_midi_beats correct input") {
  SUBCASE("empty vector") {
    std::vector<std::pair<double, double>> input;
    int bpm = 60;
    auto result = get_midi_beats(input, bpm);
    std::vector<std::pair<int, int>> expected;
    CHECK(result == expected);
  }

  SUBCASE("basic") {
    std::vector<std::pair<double, double>> input = {
        {440.0, 1.0}, {146.83, 0.5}, {0.0, 2.0}, {220.0, 0.5}};

    int bpm = 120;
    auto result = get_midi_beats(input, bpm);

    std::vector<std::pair<int, int>> expected = {
        {69, 8}, {50, 4}, {-1, 16}, {57, 4}};

    CHECK(result == expected);
  }

  SUBCASE("odd lengths") {
    std::vector<std::pair<double, double>> input = {
        {880.0, 1.6}, {440.0, 0.8},  {220.0, 0.7},  {110.0, 0.35},
        {220.0, 0.1}, {440.0, 1.15}, {220.0, 0.05}, {440.0, 0.05},
        {220.0, 2.3}, {110.0, 0.2},  {220.0, 0.3},  {110.0, 0.4}};

    int bpm = 60;
    auto result = get_midi_beats(input, bpm);

    std::vector<std::pair<int, int>> expected = {{81, 6}, {69, 4}, {57, 2},
                                                 {45, 2}, {69, 5}, {57, 9},
                                                 {45, 1}, {57, 1}, {45, 2}};

    CHECK(result == expected);
  }

  SUBCASE("get_midi_beats one note repeating") {
    std::vector<std::pair<double, double>> input = {{440.0, 1.1}, {440.0, 0.1},
                                                    {440.0, 0.4}, {440.0, 0.4},
                                                    {440.0, 2.2}, {440.0, 1.8}};

    int bpm = 60;
    auto result = get_midi_beats(input, bpm);

    std::vector<std::pair<int, int>> expected = {
        {69, 5}, {69, 1}, {69, 2}, {69, 9}, {69, 7}};

    CHECK(result == expected);
  }

  SUBCASE("get_midi_beats cut endind") {
    std::vector<std::pair<double, double>> input = {{440.0, 1.05}, {0.0, 0.1}};

    int bpm = 60;
    auto result = get_midi_beats(input, bpm);

    std::vector<std::pair<int, int>> expected = {{69, 4}, {-1, 1}};

    CHECK(result == expected);
  }
}

TEST_CASE("get_midi_beats exceptions") {
  SUBCASE("bpm is not positive") {
    std::vector<std::pair<double, double>> input;
    int bpm = -1;
    CHECK_THROWS_AS(get_midi_beats(input, 0), std::invalid_argument);
  }
  SUBCASE("duration is not positive") {
    std::vector<std::pair<double, double>> input = {{440.0, 1.0}, {0.0, -1.0}};
    int bpm = 60;
    CHECK_THROWS_AS(get_midi_beats(input, 0), std::invalid_argument);
  }
}