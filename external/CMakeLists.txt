include(FetchContent)

FetchContent_Declare(doctest
	GIT_REPOSITORY https://github.com/doctest/doctest.git
	GIT_TAG origin/master)
FetchContent_GetProperties(doctest)
FetchContent_MakeAvailable(doctest)

FetchContent_Declare(minimp3
	GIT_REPOSITORY https://github.com/lieff/minimp3.git
	GIT_TAG origin/master)
FetchContent_GetProperties(minimp3)
FetchContent_MakeAvailable(minimp3)
