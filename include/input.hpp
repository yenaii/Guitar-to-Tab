#pragma once
#include <vector>
#include <string>
#include <stdexcept>
#include <memory>
#include "minimp3.h" 
/**
 * @file input.hpp
 * @brief Модули для декодирования MP3, анализа амплитуд и базовой обработки аудиосигнала.
 */

/**
 * @class AudioDecoder
 * @brief Декодирует MP3-файлы в PCM-амплитуды (float).
 */
class AudioDecoder {
public:
    /**
     * @struct AudioData
     * @brief Структура с результатом декодирования: сэмплы, частота, каналы, количество сэмплов.
     */
    
    struct AudioData {
        std::vector<float> samples; ///< Данные PCM-сэмплов (float)
        int sampleRate; ///<    частота дискретизации (Гц)
        int channels; ///< Количество каналов (1 — моно, 2 — стерео)
        size_t totalSamples; ///< Общее количество сэмплов
    };
    /**
     * @brief Конструктор, инициализирует декодер.
     */
    AudioDecoder();
    /**
     * @brief Загружает MP3-файл и возвращает декодированные данные.
     * @param filename Путь к MP3-файлу.
     * @return AudioData с PCM-сэмплами.
     * @throws std::runtime_error при ошибке открытия или декодирования.
     */
    AudioData loadMP3(const std::string& filename);
private:
    struct MP3DecoderDeleter {
        void operator()(mp3dec_t* decoder) const; ///< Освобождает ресурсы декодера
    };
    std::unique_ptr<mp3dec_t, MP3DecoderDeleter> mp3Decoder; ///< Уникальный указатель на декодер MP3
};

/**
 * @class AudioAnalyzer
 * @brief Анализирует амплитуды PCM-сигнала, вычисляет спектр, максимум, минимум и среднее.
 */
class AudioAnalyzer {
public:
    /**
     * @struct AmplitudeAnalysis
     * @brief Результаты анализа амплитуд: значения, максимум, минимум, среднее, спектр.
     */
    struct AmplitudeAnalysis {
        std::vector<float> amplitudes; ///< Вектор амплитуд (float)
        float maxAmplitude; ///< Максимальная амплитуда
        float minAmplitude; ///< Минимальная амплитуда
        float averageAmplitude; ///< Средняя амплитуда
        std::vector<float> amplitudeSpectrum; ///< Спектр амплитуд (частотная характеристика)
    };
    /**
     * @brief Анализирует амплитуды (выделяет канал, считает максимум, минимум, среднее, спектр).
     * @param samples Входные сэмплы (float).
     * @param channels Количество каналов (1 — моно, 2 — стерео).
     * @return Структура с результатами анализа.
     */
    static AmplitudeAnalysis analyzeAmplitudes(const std::vector<float>& samples, int channels);
    /**
     * @brief Печатает краткий отчёт по анализу амплитуд.
     */
    static void printAnalysisReport(const AmplitudeAnalysis& analysis);
    /**
     * @brief Визуализирует форму волны в консоли.
     */
    static void visualizeWaveform(const std::vector<float>& amplitudes, size_t width = 80, size_t height = 20);
private:
    /**
     * @brief Вычисляет спектр амплитуд с помощью простого метода (например, БПФ).
     * @param analysis Результаты анализа амплитуд.
     */
    static void computeSimpleSpectrum(AmplitudeAnalysis& analysis);
};

/**
 * @class AudioProcessor
 * @brief Примерный пайплайн: декодирование, анализ, визуализация.
 */
class AudioProcessor {
public:
    /**
     * @brief Обрабатывает файл: декодирует, анализирует, визуализирует.
     * @param inputFile Путь к MP3-файлу.
     * @return Вектор амплитуд первого канала.
     */
    std::vector<float> processFile(const std::string& inputFile);
};
