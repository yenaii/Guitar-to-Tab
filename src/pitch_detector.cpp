#include "pitch_detector.hpp"

#include <cmath>
#include <limits>
#include <vector>

std::vector<PitchResult> PitchDetector::detectYIN(
    const std::vector<float>& signal, float sampleRate, int windowSize,
    int hopSize, float threshold) {
  std::vector<PitchResult> results;
  size_t N = signal.size();
  for (size_t start = 0; start + windowSize < N; start += hopSize) {
    std::vector<float> diff(windowSize, 0.0f);
    for (int tau = 1; tau < windowSize; ++tau) {
      float sum = 0.0f;
      for (int i = 0; i < windowSize - tau; ++i) {
        float d = signal[start + i] - signal[start + i + tau];
        sum += d * d;
      }
      diff[tau] = sum;
    }

    std::vector<float> cumulative(windowSize, 0.0f);
    cumulative[0] = 1.0f;
    float runningSum = 0.0f;
    for (int tau = 1; tau < windowSize; ++tau) {
      runningSum += diff[tau];
      cumulative[tau] = diff[tau] / ((runningSum / tau) + 1e-8f);
    }

    int tauEstimate = -1;
    for (int tau = 2; tau < windowSize; ++tau) {
      if (cumulative[tau] < threshold) {
        while (tau + 1 < windowSize && cumulative[tau + 1] < cumulative[tau]) ++tau;
        tauEstimate = tau;
        break;
      }
    }
    float freq = 0.0f;
    if (tauEstimate > 0 && cumulative[tauEstimate] < threshold) {
      freq = sampleRate / tauEstimate;
      if (freq > sampleRate / 4) freq = 0.0f;
    }
    results.push_back({freq, start, static_cast<size_t>(windowSize)});
  }
  return results;
}
