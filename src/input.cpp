#define NOMINMAX
#include <algorithm>
#include <memory>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <limits>
#include <stdexcept>

#define MINIMP3_IMPLEMENTATION
#include <minimp3.h>
#include <minimp3_ex.h>

#include "input.hpp"

using namespace std;

AudioDecoder::AudioDecoder() : mp3Decoder(new mp3dec_t) {
    mp3dec_init(mp3Decoder.get());
}

AudioDecoder::AudioData AudioDecoder::loadMP3(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> fileBuffer(fileSize);
    file.read(reinterpret_cast<char*>(fileBuffer.data()), fileSize);

    mp3dec_file_info_t info;
    if (mp3dec_load_buf(mp3Decoder.get(), fileBuffer.data(), fileSize, &info, nullptr, nullptr)) {
        throw std::runtime_error("Failed to decode MP3 file");
    }

    AudioData result;
    result.sampleRate = info.hz;
    result.channels = info.channels;
    result.totalSamples = info.samples / info.channels;

    result.samples.reserve(info.samples);
    for (size_t i = 0; i < info.samples; i++) {
        result.samples.push_back(info.buffer[i] / 32768.0f);
    }

    return result;
}

void AudioDecoder::MP3DecoderDeleter::operator()(mp3dec_t* decoder) const {
    if (decoder) {
        // minimp3 не требует явного освобождения
    }
}

AudioAnalyzer::AmplitudeAnalysis AudioAnalyzer::analyzeAmplitudes(const std::vector<float>& samples, int channels) {
    AudioAnalyzer::AmplitudeAnalysis analysis;
    analysis.amplitudes.reserve(samples.size() / channels);

    for (size_t i = 0; i < samples.size(); i += channels) {
        analysis.amplitudes.push_back(samples[i]);
    }

    analysis.maxAmplitude = *std::max_element(analysis.amplitudes.begin(), analysis.amplitudes.end());
    analysis.minAmplitude = *std::min_element(analysis.amplitudes.begin(), analysis.amplitudes.end());

    float sum = 0.0f;
    for (float amp : analysis.amplitudes) {
        sum += std::abs(amp);
    }
    analysis.averageAmplitude = sum / analysis.amplitudes.size();

    computeSimpleSpectrum(analysis);
    return analysis;
}

void AudioAnalyzer::printAnalysisReport(const AmplitudeAnalysis& analysis) {
    std::cout << "Audio Analysis Report:\n";
    std::cout << "=================================\n";
    std::cout << "Total samples: " << analysis.amplitudes.size() << "\n";
    std::cout << "Max amplitude: " << analysis.maxAmplitude << "\n";
    std::cout << "Min amplitude: " << analysis.minAmplitude << "\n";
    std::cout << "Avg amplitude: " << analysis.averageAmplitude << "\n";
    std::cout << "=================================\n";
}

void AudioAnalyzer::visualizeWaveform(const std::vector<float>& amplitudes, size_t width, size_t height) {
    if (amplitudes.empty()) return;

    size_t step = amplitudes.size() / width;
    if (step == 0) step = 1;

    float min = *std::min_element(amplitudes.begin(), amplitudes.end());
    float max = *std::max_element(amplitudes.begin(), amplitudes.end());
    float range = max - min;

    std::vector<std::string> canvas(height, std::string(width, ' '));

    for (size_t x = 0; x < width; x++) {
        size_t sampleIdx = x * step;
        if (sampleIdx >= amplitudes.size()) break;

        float normalized = (amplitudes[sampleIdx] - min) / range;
        size_t y = static_cast<size_t>(normalized * (height - 1));

        if (y < height) {
            canvas[height - 1 - y][x] = '*';
        }
    }

    std::cout << "Waveform Visualization:\n";
    for (const auto& line : canvas) {
        std::cout << line << "\n";
    }
}

void AudioAnalyzer::computeSimpleSpectrum(AmplitudeAnalysis& analysis) {
    const size_t spectrumSize = 20;
    analysis.amplitudeSpectrum.resize(spectrumSize, 0.0f);

    size_t binSize = analysis.amplitudes.size() / spectrumSize;
    if (binSize == 0) binSize = 1;

    for (size_t i = 0; i < spectrumSize; i++) {
        size_t start = i * binSize;
        size_t end = std::min(start + binSize, analysis.amplitudes.size());

        float sum = 0.0f;
        for (size_t j = start; j < end; j++) {
            sum += std::abs(analysis.amplitudes[j]);
        }
        analysis.amplitudeSpectrum[i] = sum / (end - start);
    }
}

std::vector<float> AudioProcessor::processFile(const std::string& inputFile) {
    try {
        AudioDecoder decoder;
        auto audioData = decoder.loadMP3(inputFile);

        std::cout << "Decoding complete. Sample rate: " << audioData.sampleRate 
                  << ", Channels: " << audioData.channels 
                  << ", Samples: " << audioData.totalSamples << "\n";

        auto analysis = AudioAnalyzer::analyzeAmplitudes(audioData.samples, audioData.channels);
        AudioAnalyzer::printAnalysisReport(analysis);

        std::cout << "\nVisualizing waveform (first channel):\n";
        AudioAnalyzer::visualizeWaveform(analysis.amplitudes);

        return analysis.amplitudes;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return {};
    }
}
