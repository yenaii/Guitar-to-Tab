#pragma once
#include <vector>
/**
 * @file fir_filter.hpp
 * @brief Класс для фильтрации аудиосигнала методом скользящего среднего (FIR-фильтр).
 */

/**
 * @class FIRFilter
 * @brief Реализует FIR-фильтр для подавления шумов в аудиосигнале.
 */
class FIRFilter {
 public:
  /**
   * @brief Применяет FIR-фильтр к сигналу.
   * @param input Входной сигнал (амплитуды).
   * @param windowSize Размер окна усреднения (количество сэмплов).
   * @return Отфильтрованный сигнал (амплитуды).
   */
  static std::vector<float> apply(const std::vector<float>& input,
                                  int windowSize);
};
