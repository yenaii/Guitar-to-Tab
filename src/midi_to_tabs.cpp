#include "midi_to_tabs.hpp"

#include <array>
#include <cmath>
#include <iostream>
#include <sstream>
#include <stdexcept>

std::vector<std::array<int, 3>> read_range(std::istream& in) {
  std::vector<std::array<int, 3>> result;
  std::string line;

  std::cout << "Enter fret ranges: [min.fret max.fret duration_in_bar] (print 'stop' to end): " << line << std::endl;
  while (std::getline(in, line)) {
    if (line == "stop") {
      break;
    }
    std::istringstream iss(line);
    std::array<int, 3> triplet;
    if (!(iss >> triplet[0] >> triplet[1] >> triplet[2])) {
      throw std::invalid_argument("Failed to parse triplet: " + line);
    }
    result.push_back(triplet);
  }
  return result;
}

std::vector<std::array<int, 3>> apply_range(
    std::vector<std::array<int, 3>> ranges,
    std::vector<std::pair<int, int>> midi_beats) {
  std::vector<std::array<int, 3>> notes;

  int cumulative_sum = 0;
  for (std::array<int, 3>& arr : ranges) {
    if (arr[2] <= 0) {
      throw std::invalid_argument("invalid duration");
    }
    if (arr[0] < 0 || arr[0] > 22 || arr[1] < 0 || arr[1] > 22 ||
        arr[0] > arr[1]) {
      throw std::invalid_argument("invalid fret range");
    }

    cumulative_sum += arr[2];
    arr[2] = cumulative_sum * 16;
  }

  cumulative_sum = 0;
  for (std::pair<int, int> note : midi_beats) {
    cumulative_sum += note.second;
    if (note.first == -1) {
      notes.push_back({5, -1, note.second});
      goto exit;
    }
    for (std::array<int, 3> range : ranges) {
      int note_start = cumulative_sum - note.second;
      if (note_start <= range[2]) {
        for (int st = 5; st >= 0; --st) {
          if (note.first - strings_midi[st] >= range[0] &&
              note.first - strings_midi[st] <= range[1]) {
            notes.push_back({st, note.first - strings_midi[st], note.second});
            goto exit;
          }
        }
        throw std::invalid_argument("midi is not in given range");
      }
    }
    for (int st = 5; st >= 0; --st) {
      if (note.first - strings_midi[st] >= 0 &&
          note.first - strings_midi[st] <= 22) {
        notes.push_back({st, note.first - strings_midi[st], note.second});
        goto exit;
      }
    }
    throw std::invalid_argument("midi is not in given range");
  exit:;
  }

  return notes;
}

void generate_tab(std::vector<std::array<int, 3>>& notes, std::ostream& out) {
  for (const auto& note : notes) {
    if (note[0] < 0 || note[0] > 5)
      throw std::invalid_argument("Invalid string index");
    if (note[1] < -1 || note[1] > 22)
      throw std::invalid_argument("Invalid fret value");
    if (note[2] <= 0) throw std::invalid_argument("Invalid note length");
  }
  std::ostringstream lines[8];

  lines[0] << "e|-";
  lines[1] << "B|-";
  lines[2] << "G|-";
  lines[3] << "D|-";
  lines[4] << "A|-";
  lines[5] << "E|-";
  lines[6] << "   ";
  lines[7] << "   ";

  int bar = 16;
  int strin;
  int len;
  while (!notes.empty()) {
    if (bar == 0) {
      bar = 16;
      for (size_t i = 0; i < 6; ++i) {
        lines[i] << "|-";
      }
      lines[6] << "  ";
      lines[7] << "  ";
    }

    len = 16;
    if (notes[0][2] <= bar) {
      for (int i = 4; i >= 0; --i) {
        len = static_cast<int>(std::pow(2, i));
        if (len <= notes[0][2]) break;
      }
    } else {
      for (int i = 4; i >= 0; --i) {
        len = static_cast<int>(std::pow(2, i));
        if (len <= bar) break;
      }
    }

    switch (len) {
      case 1:
        lines[6] << "|-   ";
        lines[7] << "|-   ";
        break;
      case 2:
        lines[6] << "|    ";
        lines[7] << "|-   ";
        break;
      case 4:
        lines[6] << "|    ";
        lines[7] << "|    ";
        break;
      case 8:
        lines[6] << "o    ";
        lines[7] << "|    ";
        break;
      case 16:
        lines[6] << "o    ";
        lines[7] << "     ";
        break;
      default:
        throw std::invalid_argument("switch case broken");
    }

    if (notes[0][1] == -1)
      lines[notes[0][0]] << "R";
    else
      lines[notes[0][0]] << notes[0][1];

    strin = notes[0][0];
    if (len < notes[0][2]) {
      if (notes[0][1] >= 10)
        lines[strin] << "~~~";
      else
        lines[strin] << "~~~~";
      notes[0][2] -= len;
    } else {
      if (notes[0][1] >= 10)
        lines[strin] << "---";
      else
        lines[strin] << "----";
      notes.erase(notes.begin());
    }

    for (int i = 0; i < 6; ++i) {
      if (i != strin) {
        lines[i] << "-----";
      }
    }
    bar -= len;
  }

  bool has_more = true;
  while (has_more) {
    has_more = false;

    std::string str = lines[0].str();
    size_t split_pos = str.rfind('|', MAX_WIDTH);

    if (split_pos == std::string::npos) {
      split_pos = MAX_WIDTH;
    }

    for (int i = 0; i < 8; ++i) {
      std::string str = lines[i].str();
      if (str.empty()) {
        out << '\n';
        continue;
      }

      if (str.length() > MAX_WIDTH) {
        out << str.substr(0, split_pos) << '\n';
        lines[i].str(str.substr(split_pos));
        lines[i].clear();
        has_more = true;
      } else {
        out << str << '\n';
        lines[i].str("");
        lines[i].clear();
      }
    }

    out << '\n';
  }
}
