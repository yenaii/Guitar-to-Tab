#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <cmath>
#include <vector>

#include "pitch_detector.hpp"

std::vector<float> generate_sine(float freq, float sampleRate, float duration) {
  int N = static_cast<int>(sampleRate * duration);
  std::vector<float> data(N);
  for (int i = 0; i < N; ++i) {
    data[i] = static_cast<float>(std::sin(6.283 * freq * i / sampleRate));
  }
  return data;
}

std::vector<float> generate_two_tone(float freq1, float freq2, float sampleRate,
                                     float duration) {
  int N = static_cast<int>(sampleRate * duration);
  std::vector<float> data(N);
  for (int i = 0; i < N / 2; ++i) {
    data[i] = static_cast<float>(std::sin(6.283 * freq1 * i / sampleRate));
  }
  for (int i = N / 2; i < N; ++i) {
    data[i] = static_cast<float>(std::sin(6.283 * freq2 * i / sampleRate));
  }
  return data;
}

TEST_CASE("PitchDetector::detectYIN находит частоту синусоиды 440 Гц") {
  float freq = 440.0f;
  float sampleRate = 44100.0f;
  auto signal = generate_sine(freq, sampleRate, 0.1f);
  int windowSize = 1024;
  int hopSize = 256;
  auto results =
      PitchDetector::detectYIN(signal, sampleRate, windowSize, hopSize);
  int count = 0;
  for (const auto& r : results) {
    if (r.frequency > 0.0f) CHECK(std::abs(r.frequency - 440.0f) < 5.0f);
    if (r.frequency > 0.0f) ++count;
  }
  CHECK(count >= 2);
}

TEST_CASE("PitchDetector::detectYIN различает две разные частоты") {
  float sampleRate = 44100.0f;
  auto signal = generate_two_tone(440.0f, 660.0f, sampleRate, 0.2f);
  int windowSize = 1024;
  int hopSize = 256;
  auto results =
      PitchDetector::detectYIN(signal, sampleRate, windowSize, hopSize);
  int ok_first = 0, ok_second = 0;
  for (size_t i = 0; i < results.size(); ++i) {
    size_t start = results[i].startSample;
    if (start < signal.size() / 2 && results[i].frequency > 0.0f)
      if (std::abs(results[i].frequency - 440.0f) < 60.0f) ++ok_first;
    if (start >= signal.size() / 2 && results[i].frequency > 0.0f)
      if (std::abs(results[i].frequency - 660.0f) < 60.0f) ++ok_second;
  }
  CHECK(ok_first >= 1);
  CHECK(ok_second >= 1);
}

TEST_CASE("PitchDetector::detectYIN возвращает 0 на тишине") {
  std::vector<float> silence(44100, 0.0f);
  auto results = PitchDetector::detectYIN(silence, 44100.0f, 2048, 512);
  for (const auto& r : results) {
    CHECK(r.frequency == doctest::Approx(0.0f));
  }
}
