#pragma once
#include <cstddef>
#include <vector>
/**
 * @file pitch_detector.hpp
 * @brief Модуль для извлечения основного тона (pitch detection) из аудиосигнала.
 */

/**
 * @struct PitchResult
 * @brief Результат pitch detection для одного окна: частота, начало, длина окна.
 */
struct PitchResult {
  float frequency;     ///< Частота (Гц)
  size_t startSample;  ///< Индекс первого сэмпла окна
  size_t length;       ///< Длина окна (в сэмплах)
};

/**
 * @class PitchDetector
 * @brief Реализует алгоритм YIN для извлечения основного тона из аудиосигнала.
 */
class PitchDetector {
 public:
  /**
   * @brief Извлекает основную частоту (pitch) с помощью YIN-алгоритма.
   * @param signal Входной сигнал (моно).
   * @param sampleRate Частота дискретизации.
   * @param windowSize Размер окна (обычно 1024-2048).
   * @param hopSize Шаг окна (обычно 256-512).
   * @param threshold Порог для YIN (0.1-0.2).
   * @return Вектор PitchResult для каждого окна.
   */
  static std::vector<PitchResult> detectYIN(const std::vector<float>& signal,
                                            float sampleRate, int windowSize,
                                            int hopSize,
                                            float threshold = 0.15f);
};
