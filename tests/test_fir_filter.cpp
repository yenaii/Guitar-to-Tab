#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <cmath>
#include <vector>

#include "fir_filter.hpp"

std::vector<float> generate_sine(float freq, float sampleRate, float duration) {
  int N = static_cast<int>(sampleRate * duration);
  std::vector<float> data(N);
  for (int i = 0; i < N; ++i) {
    data[i] = std::sin(6.283f * freq * i / sampleRate);
  }
  return data;
}

std::vector<float> generate_step(float value, int N) {
  return std::vector<float>(N, value);
}

TEST_CASE("FIRFilter сглаживает синусоиду") {
  auto input = generate_sine(440.0f, 44100.0f, 0.01f);
  auto output = FIRFilter::apply(input, 5);
  float avg_in = 0, avg_out = 0;
  for (float v : input) avg_in += v;
  for (float v : output) avg_out += v;
  avg_in /= input.size();
  avg_out /= output.size();
  CHECK(std::abs(avg_in - avg_out) < 0.01f);
}

TEST_CASE("FIRFilter сглаживает ступенчатый сигнал") {
  auto input = generate_step(1.0f, 100);
  input[50] = 10.0f; 
  auto output = FIRFilter::apply(input, 7);
  CHECK(output[50] < 5.0f);
  CHECK(output[50] > 1.0f);
}

TEST_CASE("FIRFilter не меняет постоянный сигнал") {
  auto input = generate_step(3.14f, 100);
  auto output = FIRFilter::apply(input, 9);
  for (float v : output) {
    CHECK(std::abs(v - 3.14f) < 1e-4f);
  }
}
