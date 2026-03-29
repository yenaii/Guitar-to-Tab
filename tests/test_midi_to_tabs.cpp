#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <iostream>
#include <stdexcept>

#include "midi_to_tabs.hpp"

TEST_CASE("read_range valid cases") {
  SUBCASE("basic input") {
    std::istringstream input("1 2 3\n4 5 6\nstop\n");
    auto result = read_range(input);
    std::vector<std::array<int, 3>> expected = {{1, 2, 3}, {4, 5, 6}};
    CHECK(result == expected);
  }

  SUBCASE("empty") {
    std::istringstream input("stop\n");
    auto result = read_range(input);
    CHECK(result.empty());
  }
}

TEST_CASE("read_range exceptions") {
  SUBCASE("not enough numbers") {
    std::istringstream input("1 2\n");
    CHECK_THROWS_AS(read_range(input), std::invalid_argument);
  }
  SUBCASE("not a number") {
    std::istringstream input2("1 a 3\n");
    CHECK_THROWS_AS(read_range(input2), std::invalid_argument);
  }
}

TEST_CASE("apply_range valid inputs") {
  SUBCASE("single note fits into given range") {
    std::vector<std::array<int, 3>> ranges = {{0, 5, 1}};
    std::vector<std::pair<int, int>> midi_beats = {{43, 1}};

    auto result = apply_range(ranges, midi_beats);
    std::vector<std::array<int, 3>> expected = {{5, 3, 1}};

    CHECK(result == expected);
  }

  SUBCASE("empty") {
    std::vector<std::array<int, 3>> ranges;
    std::vector<std::pair<int, int>> midi_beats;

    auto result = apply_range(ranges, midi_beats);
    std::vector<std::array<int, 3>> expected;

    CHECK(result == expected);
  }

  SUBCASE("Note outside all ranges' durations but fits default fret range") {
    std::vector<std::array<int, 3>> ranges = {{0, 3, 1}};
    std::vector<std::pair<int, int>> midi_beats = {{52, 1}};

    auto result = apply_range(ranges, midi_beats);
    std::vector<std::array<int, 3>> expected = {{3, 2, 1}};

    CHECK(result == expected);
  }

  SUBCASE("Multiple ranges and notes") {
    std::vector<std::array<int, 3>> ranges = {
        {0, 5, 3}, {6, 10, 5}, {15, 18, 4}};

    std::vector<std::pair<int, int>> midi_beats = {
        {50, 32}, {-1, 64}, {66, 160}, {86, 16}};

    auto result = apply_range(ranges, midi_beats);
    std::vector<std::array<int, 3>> expected = {
        {4, 5, 32},
        {5, -1, 64},
        {1, 7, 160},
        {0, 22, 16},
    };

    CHECK(result == expected);
  }
}

TEST_CASE("apply_range exceptions") {
  SUBCASE("negative duration in range") {
    std::vector<std::array<int, 3>> ranges = {{0, 5, -1}};
    std::vector<std::pair<int, int>> midi_beats = {{45, 1}};
    CHECK_THROWS_AS(apply_range(ranges, midi_beats), std::invalid_argument);
  }

  SUBCASE("fret range out of bounds") {
    std::vector<std::array<int, 3>> ranges = {{0, 30, 1}};
    std::vector<std::pair<int, int>> midi_beats = {{45, 1}};
    CHECK_THROWS_AS(apply_range(ranges, midi_beats), std::invalid_argument);
  }

  SUBCASE("invalid order of range limits") {
    std::vector<std::array<int, 3>> ranges = {{3, 2, 1}};
    std::vector<std::pair<int, int>> midi_beats = {{85, 1}};
    CHECK_THROWS_AS(apply_range(ranges, midi_beats), std::invalid_argument);
  }
}

TEST_CASE("generate_tab valid input") {
  SUBCASE("empty input") {
    std::vector<std::array<int, 3>> notes;

    std::ostringstream oss;
    generate_tab(notes, oss);
    auto output = oss.str();

    std::string expected =
        "e|-\n"
        "B|-\n"
        "G|-\n"
        "D|-\n"
        "A|-\n"
        "E|-\n"
        "   \n"
        "   \n"
        "\n";
    CHECK(output == expected);
  }

  SUBCASE("one note") {
    std::vector<std::array<int, 3>> notes = {{5, 1, 4}};

    std::ostringstream oss;
    generate_tab(notes, oss);
    auto output = oss.str();

    std::string expected =
        "e|------\n"
        "B|------\n"
        "G|------\n"
        "D|------\n"
        "A|------\n"
        "E|-1----\n"
        "   |    \n"
        "   |    \n"
        "\n";
    CHECK(output == expected);
  }
  SUBCASE("different notes, different lengths") {
    std::vector<std::array<int, 3>> notes = {{5, 1, 3}, {5, -1, 2}, {0, 10, 7}};

    std::ostringstream oss;
    generate_tab(notes, oss);
    auto output = oss.str();

    std::string expected =
        "e|----------------10~~~10~~~10---\n"
        "B|-------------------------------\n"
        "G|-------------------------------\n"
        "D|-------------------------------\n"
        "A|-------------------------------\n"
        "E|-1~~~~1----R-------------------\n"
        "   |    |-   |    |    |    |-   \n"
        "   |-   |-   |-   |    |-   |-   \n"
        "\n";
    CHECK(output == expected);
  }

  SUBCASE("bar overcome") {
    std::vector<std::array<int, 3>> notes = {
        {4, 1, 18},
    };

    std::ostringstream oss;
    generate_tab(notes, oss);
    auto output = oss.str();

    std::string expected =
        "e|------|------\n"
        "B|------|------\n"
        "G|------|------\n"
        "D|------|------\n"
        "A|-1~~~~|-1----\n"
        "E|------|------\n"
        "   o      |    \n"
        "          |-   \n"
        "\n";
    CHECK(output == expected);
  }

  SUBCASE("max_width overcome") {
    std::vector<std::array<int, 3>> notes = {
        {4, 1, 256},
    };

    std::ostringstream oss;
    generate_tab(notes, oss);
    auto output = oss.str();

    std::string expected =
        "e|------|------|------|------|------|------|------|------|------|-----"
        "-|------|------|------|------\n"
        "B|------|------|------|------|------|------|------|------|------|-----"
        "-|------|------|------|------\n"
        "G|------|------|------|------|------|------|------|------|------|-----"
        "-|------|------|------|------\n"
        "D|------|------|------|------|------|------|------|------|------|-----"
        "-|------|------|------|------\n"
        "A|-1~~~~|-1~~~~|-1~~~~|-1~~~~|-1~~~~|-1~~~~|-1~~~~|-1~~~~|-1~~~~|-1~~~"
        "~|-1~~~~|-1~~~~|-1~~~~|-1~~~~\n"
        "E|------|------|------|------|------|------|------|------|------|-----"
        "-|------|------|------|------\n"
        "   o      o      o      o      o      o      o      o      o      o   "
        "   o      o      o      o    \n"
        "                                                                      "
        "                             \n"
        "\n"
        "|------|------\n"
        "|------|------\n"
        "|------|------\n"
        "|------|------\n"
        "|-1~~~~|-1----\n"
        "|------|------\n"
        "  o      o    \n"
        "              \n"
        "\n";
    CHECK(output == expected);
  }
}

TEST_CASE("generate_tab exceptions") {
  SUBCASE("invalid string") {
    std::vector<std::array<int, 3>> notes = {
        {9, 1, 1},
    };

    std::ostringstream oss;
    CHECK_THROWS_AS(generate_tab(notes, oss), std::invalid_argument);
  }

  SUBCASE("invalid fret") {
    std::vector<std::array<int, 3>> notes = {
        {0, -2, 1},
    };

    std::ostringstream oss;
    CHECK_THROWS_AS(generate_tab(notes, oss), std::invalid_argument);
  }

  SUBCASE("invalid duration") {
    std::vector<std::array<int, 3>> notes = {
        {0, 0, 0},
    };

    std::ostringstream oss;
    CHECK_THROWS_AS(generate_tab(notes, oss), std::invalid_argument);
  }
}