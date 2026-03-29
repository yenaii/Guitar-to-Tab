#include "note_segment.hpp"

#include <cmath>

std::vector<NoteSegment> extract_note_segments(
    const std::vector<PitchResult>& pitches, float sampleRate,
    float freqTolerance) {
  std::vector<NoteSegment> segments;
  if (pitches.empty()) return segments;

  float prevFreq = pitches[0].frequency;
  size_t startSample = pitches[0].startSample;
  size_t endSample = startSample + pitches[0].length;

  for (size_t i = 1; i < pitches.size(); ++i) {
    float freq = pitches[i].frequency;
    if (std::abs(freq - prevFreq) < freqTolerance) {
      endSample = pitches[i].startSample + pitches[i].length;
    } else {
      float duration = (endSample - startSample) / sampleRate;
      segments.push_back({prevFreq, duration});
      prevFreq = freq;
      startSample = pitches[i].startSample;
      endSample = startSample + pitches[i].length;
    }
  }
  float duration = (endSample - startSample) / sampleRate;
  segments.push_back({prevFreq, duration});
  return segments;
}
