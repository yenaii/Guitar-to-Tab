#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <vector>

#include "note_segment.hpp"

TEST_CASE("extract_note_segments объединяет одинаковые частоты") {
  std::vector<PitchResult> pitches = {{440.0f, 0, 100},   {440.1f, 100, 100},
                                      {440.2f, 200, 100}, {450.0f, 300, 100},
                                      {450.1f, 400, 100}, {0.0f, 500, 100},
                                      {0.0f, 600, 100}};
  float sampleRate = 1000.0f;
  auto segments = extract_note_segments(pitches, sampleRate, 1.0f);
  REQUIRE(segments.size() == 3);
  CHECK(std::abs(segments[0].frequency - 440.0f) < 1.0f);
  CHECK(segments[0].duration == doctest::Approx(0.3f));
  CHECK(std::abs(segments[1].frequency - 450.0f) < 1.0f);
  CHECK(segments[1].duration == doctest::Approx(0.2f));
  CHECK(segments[2].frequency == doctest::Approx(0.0f));
  CHECK(segments[2].duration == doctest::Approx(0.2f));
}
