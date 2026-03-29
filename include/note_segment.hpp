#pragma once
#include <vector>
#include "pitch_detector.hpp"
/**
 * @file note_segment.hpp
 * @brief Модуль для группировки извлечённых высот в музыкальные сегменты (ноты).
 */

/**
 * @struct NoteSegment
 * @brief Описывает сегмент (ноту): частота и длительность звучания.
 */
struct NoteSegment {
  float frequency;   ///< Частота (Гц)
  float duration;    ///< Длительность (секунды)
};

/**
 * @brief Группирует последовательные окна с одинаковой (или близкой) частотой в сегменты.
 * @param pitches Результаты pitch detection (PitchResult).
 * @param sampleRate Частота дискретизации.
 * @param freqTolerance Порог для объединения частот (Гц).
 * @return Вектор сегментов (частота, длительность).
 */
std::vector<NoteSegment> extract_note_segments(
    const std::vector<PitchResult>& pitches, float sampleRate,
    float freqTolerance = 1.0f);
