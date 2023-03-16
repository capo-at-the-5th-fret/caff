#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

// NOTE: Building using doctest_with_main wouldn't work in clang-tidy workflow.
// Caused the doctest.cpp file to be built using c++11. Tried setting target std
// after FetchContent_MakeAvailable(doctest), but it didn't work. Linking to
// doctest and using this 'bare' main.cpp worked.
