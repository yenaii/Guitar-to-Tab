# CMake generated Testfile for 
# Source directory: D:/Guitar-To-Tabs
# Build directory: D:/Guitar-To-Tabs/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(GuitarToTabsInputTests "D:/Guitar-To-Tabs/build/test_input.exe")
set_tests_properties(GuitarToTabsInputTests PROPERTIES  _BACKTRACE_TRIPLES "D:/Guitar-To-Tabs/CMakeLists.txt;49;add_test;D:/Guitar-To-Tabs/CMakeLists.txt;0;")
add_test(GuitarToTabsFIRFilterTests "D:/Guitar-To-Tabs/build/test_fir_filter.exe")
set_tests_properties(GuitarToTabsFIRFilterTests PROPERTIES  _BACKTRACE_TRIPLES "D:/Guitar-To-Tabs/CMakeLists.txt;50;add_test;D:/Guitar-To-Tabs/CMakeLists.txt;0;")
add_test(GuitarToTabsPitchDetectorTests "D:/Guitar-To-Tabs/build/test_pitch_detector.exe")
set_tests_properties(GuitarToTabsPitchDetectorTests PROPERTIES  _BACKTRACE_TRIPLES "D:/Guitar-To-Tabs/CMakeLists.txt;51;add_test;D:/Guitar-To-Tabs/CMakeLists.txt;0;")
add_test(GuitarToTabsNoteSegmentTests "D:/Guitar-To-Tabs/build/test_note_segment.exe")
set_tests_properties(GuitarToTabsNoteSegmentTests PROPERTIES  _BACKTRACE_TRIPLES "D:/Guitar-To-Tabs/CMakeLists.txt;52;add_test;D:/Guitar-To-Tabs/CMakeLists.txt;0;")
add_test(GuitarToTabsFrequencyToMidiTests "D:/Guitar-To-Tabs/build/test_frequency_to_midi.exe")
set_tests_properties(GuitarToTabsFrequencyToMidiTests PROPERTIES  _BACKTRACE_TRIPLES "D:/Guitar-To-Tabs/CMakeLists.txt;53;add_test;D:/Guitar-To-Tabs/CMakeLists.txt;0;")
add_test(GuitarToTabsMidiToTabsTests "D:/Guitar-To-Tabs/build/test_midi_to_tabs.exe")
set_tests_properties(GuitarToTabsMidiToTabsTests PROPERTIES  _BACKTRACE_TRIPLES "D:/Guitar-To-Tabs/CMakeLists.txt;54;add_test;D:/Guitar-To-Tabs/CMakeLists.txt;0;")
subdirs("external")
