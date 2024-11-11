include(FetchContent)

# Tags: https://github.com/doctest/doctest/tags
set(NEEDED_DOCTEST_VERSION "2.4.10")

find_package(doctest ${NEEDED_DOCTEST_VERSION} QUIET)

if (NOT doctest_FOUND)

    FetchContent_Declare(
        doctest
        GIT_REPOSITORY "https://github.com/doctest/doctest"
        GIT_TAG        v${NEEDED_DOCTEST_VERSION}
        GIT_SHALLOW    TRUE
    )

    FetchContent_MakeAvailable(doctest)

    # include the doctest module that contains doctest_discover_tests
    list(APPEND CMAKE_MODULE_PATH ${doctest_SOURCE_DIR}/scripts/cmake)

endif()

# this needs to be called for the doctest_discover_tests function to work
include(doctest)

add_executable(testrunner

    main.cpp

    bit_tests.cpp
    cmath_tests.cpp
    concepts_tests.cpp
    flags_tests.cpp
    format_tests.cpp
    functional_tests.cpp
    tuple_tests.cpp
    type_list_tests.cpp
    type_traits_tests.cpp
    utility_tests.cpp
    variant_tests.cpp

    test/test_type_list_tests.cpp
)

target_link_libraries(testrunner
    PRIVATE
        doctest::doctest
        caff::caff
)

doctest_discover_tests(testrunner)
