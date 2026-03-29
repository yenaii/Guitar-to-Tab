#include "fir_filter.hpp"

#include <numeric>
#include <vector>

std::vector<float> FIRFilter::apply(const std::vector<float>& input,
                                    int windowSize) {
  if (windowSize <= 1 || input.empty()) return input;
  std::vector<float> output(input.size(), 0.0f);
  float sum = 0.0f;
  int half = windowSize / 2;
  for (size_t i = 0; i < input.size(); ++i) {
    int count = 0;
    sum = 0.0f;
    for (int j = -half; j <= half; ++j) {
      int idx = static_cast<int>(i) + j;
      if (idx >= 0 && idx < static_cast<int>(input.size())) {
        sum += input[idx];
        ++count;
      }
    }
    output[i] = sum / count;
  }
  return output;
}
