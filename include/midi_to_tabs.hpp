#pragma once
#include <array>
#include <sstream>
#include <utility>
#include <vector>

const std::array<int, 6> strings_midi = {64, 59, 55, 50, 45, 40};
const int MAX_WIDTH = 100;

/**
 * @brief Считывает диапазоны ладов и длительности из потока ввода.
 *
 * @param in Поток ввода.
 * @return std::vector<std::array<int, 3>> Вектор тройных значений: начальный
 * лад, конечный лад, количество долей.
 * @throws std::invalid_argument Если не удаётся распарсить строку.
 */
std::vector<std::array<int, 3>> read_range(std::istream& in);
/**
 * @brief Применяет диапазоны ладов к MIDI-нотам и формирует ноты с указанием
 * струны.
 *
 * @param ranges Вектор диапазонов в формате [начальный лад, конечный лад,
 * длительность].
 * @param midi_beats Вектор пар <MIDI-номер, длительность>.
 * @return std::vector<std::array<int, 3>> Вектор тройных значений: струна, лад,
 * длительность (в 16-ых долях).
 * @throws std::invalid_argument При недопустимых значениях или несовпадении
 * диапазона.
 */
std::vector<std::array<int, 3>> apply_range(
    std::vector<std::array<int, 3>> ranges,
    std::vector<std::pair<int, int>> midi_beats);
/**
 * @brief Генерирует табулатуру по заданным нотам и выводит её в поток.
 *
 * @param notes Вектор из троек: струна, лад, длительность (в 16-х долях).
 * @param out Поток вывода.
 * @throws std::invalid_argument При ошибках в структуре нот.
 */
void generate_tab(std::vector<std::array<int, 3>>& notes, std::ostream& out);