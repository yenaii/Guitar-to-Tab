#include "input.hpp"
#include "fir_filter.hpp"
#include "pitch_detector.hpp"
#include "note_segment.hpp"
#include "frequency_to_midi.hpp"
#include "midi_to_tabs.hpp"
#include <iostream>
#include <vector>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <input.mp3> [bpm=60] [fir_window=5]" << std::endl;
        return 1;
    }
    std::string mp3file = argv[1];
    int bpm = (argc > 2) ? bpm = std::stoi(argv[2]) : 60;
    int fir_window = (argc > 3) ? std::stoi(argv[3]) : 5;

    // 1. Декодирование MP3
    AudioDecoder decoder;
    auto audio = decoder.loadMP3(mp3file);
    std::cout << "Loaded: " << mp3file << ", samples: " << audio.samples.size() << ", sampleRate: " << audio.sampleRate << std::endl;

    // 2. Формирование амплитуд (берём только первый канал)
    auto analysis = AudioAnalyzer::analyzeAmplitudes(audio.samples, audio.channels);
    auto amplitudes = analysis.amplitudes;
    std::cout << "analisis cmpleted " << amplitudes.size() << " samples." << std::endl;

    // 3. Фильтрация
    auto filtered = FIRFilter::apply(amplitudes, fir_window);
    std::cout << "filtered " << filtered.size() << " samples." << std::endl;

    // 4. Pitch detection
    int windowSize = 1024;
    int hopSize = 256;
    auto pitches = PitchDetector::detectYIN(filtered, audio.sampleRate, windowSize, hopSize);
    std::cout << "pitched " << pitches.size() << " pitch results." << std::endl;

    // 5. Сегментация нот
    auto notes = extract_note_segments(pitches, audio.sampleRate);
    std::cout << "extracted " << notes.size() << " note segments." << std::endl;

    // Фильтрация по диапазону гитары (E2–C6)
    std::vector<NoteSegment> filtered_notes;
    for (const auto& n : notes) {
        if (n.frequency >= 82.0f && n.frequency <= 1100.0f)
            filtered_notes.push_back(n);
        else
            filtered_notes.push_back({0.0f, n.duration}); // Пауза
    }

    // 6. Преобразование в MIDI и вывод
    std::vector<std::pair<double, double>> freq_dur;
    
    for (const auto& n : filtered_notes) {
        if (n.frequency > 0.0f) {
            try {
                int midi = frequency_to_midi(n.frequency);
                freq_dur.push_back({n.frequency, n.duration});
            } catch (const std::exception& e) {
                std::cerr << "Ошибка преобразования частоты " << n.frequency << " Гц в MIDI: " << e.what() << std::endl;
                freq_dur.push_back({-1, n.duration});
            }
        } else {
            freq_dur.push_back({-1, n.duration});
        }
    }
    
    std::cout << "Using BPM: " << bpm << std::endl;
    auto midi_beats = get_midi_beats(freq_dur, bpm);
    std::cout << "midi completed with " << midi_beats.size() << " beats." << std::endl;

    // 7. Генерация табулатуры
    // Диапазоны ладов: по умолчанию для всех струн 0-22, вся длительность
    std::vector<std::array<int, 3>> ranges = read_range(std::cin);
    try {
        auto notes_for_tab = apply_range(ranges, midi_beats);
        std::cout << "\nGenerated Guitar Tab:\n";
        generate_tab(notes_for_tab, std::cout);
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при применении диапазонов: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
