cmake_minimum_required(VERSION 3.14)
project(Guitar-To-Tabs LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Пути к заголовочным файлам
include_directories(include)
include_directories(${CMAKE_BINARY_DIR}/_deps/minimp3-src)

# === Подключение doctest через FetchContent ===
add_subdirectory(external)

# Главная библиотека
add_library(guitar_to_tabs STATIC
    src/input.cpp
    src/frequency_to_midi.cpp
    src/midi_to_tabs.cpp
    src/fir_filter.cpp
    src/pitch_detector.cpp
    src/note_segment.cpp
)

# Исполняемый файл
add_executable(main src/main.cpp)
target_link_libraries(main PRIVATE guitar_to_tabs)

# Тесты
enable_testing()

add_executable(test_input tests/test_input.cpp)
target_link_libraries(test_input PRIVATE guitar_to_tabs doctest)

add_executable(test_fir_filter tests/test_fir_filter.cpp)
target_link_libraries(test_fir_filter PRIVATE guitar_to_tabs doctest)

add_executable(test_pitch_detector tests/test_pitch_detector.cpp)
target_link_libraries(test_pitch_detector PRIVATE guitar_to_tabs doctest)

add_executable(test_note_segment tests/test_note_segment.cpp)
target_link_libraries(test_note_segment PRIVATE guitar_to_tabs doctest)

add_executable(test_frequency_to_midi tests/test_frequency_to_midi.cpp)
target_link_libraries(test_frequency_to_midi PRIVATE guitar_to_tabs doctest)

add_executable(test_midi_to_tabs tests/test_midi_to_tabs.cpp)
target_link_libraries(test_midi_to_tabs PRIVATE guitar_to_tabs doctest)

add_test(NAME GuitarToTabsInputTests COMMAND test_input)
add_test(NAME GuitarToTabsFIRFilterTests COMMAND test_fir_filter)
add_test(NAME GuitarToTabsPitchDetectorTests COMMAND test_pitch_detector)
add_test(NAME GuitarToTabsNoteSegmentTests COMMAND test_note_segment)
add_test(NAME GuitarToTabsFrequencyToMidiTests COMMAND test_frequency_to_midi)
add_test(NAME GuitarToTabsMidiToTabsTests COMMAND test_midi_to_tabs)