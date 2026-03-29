#pragma once
#include <utility>
#include <vector>

const int min_midi = 40;
const int max_midi = 86;

/**
 * @brief Структура, описывающая одну MIDI-ноту с её длительностью.
 *
 * Используется для хранения информации о том, какая нота (в формате MIDI)
 * играет в какой момент, и сколько шестнадцатых долей она длится.
 */
struct NoteDuration {
  /**
   * @brief Порядковый номер ноты (используется для отслеживания позиции в
   * меняющемся midi_duration).
   */
  size_t num;
  /**
   * @brief MIDI-номер ноты (-1 для паузы).
   */
  int midi;
  /**
   * @brief Длительность ноты в секундах (или долях, в зависимости от
   * контекста).
   */
  double duration = 0.0;
};

/**
 * @brief Преобразует частоту в значение MIDI.
 *
 * @param freq Частота в герцах.
 * @return int Номер MIDI-ноты или -1 для паузы.
 * @throws std::invalid_argument Если частота отрицательна или вне диапазона
 * гитары.
 */
int frequency_to_midi(double freq);
/**
 * @brief Суммирует длительности всех нот в векторе.
 *
 * @param midi_duration Вектор структур NoteDuration.
 * @return double Суммарная длительность.
 */
double duration_sum(const std::vector<NoteDuration>& midi_duration);
/**
 * @brief Находит индекс ноты с максимальной длительностью.
 *
 * @param midi_duration Вектор структур NoteDuration.
 * @return int Индекс ноты с максимальной длительностью.
 * @throws std::invalid_argument Если вектор пуст.
 */
int max_duration(const std::vector<NoteDuration>& midi_duration);
/**
 * @brief Преобразует пары частота-длительность в пары MIDI-доля (в 16-ых).
 *
 * @param frequency_duration Вектор пар <частота, длительность>.
 * @param bpm Темп в ударах в минуту.
 * @return std::vector<std::pair<int, int>> Вектор пар <MIDI, длительность в
 * 16-ых>.
 * @throws std::invalid_argument Если BPM ≤ 0 или есть длительность ≤ 0.
 */
std::vector<std::pair<int, int>> get_midi_beats(
    std::vector<std::pair<double, double>> frequency_duration, int bpm);
