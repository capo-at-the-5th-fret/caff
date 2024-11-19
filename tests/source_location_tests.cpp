#include <catch2/catch_test_macros.hpp>
#include "caff/source_location.h"

#include <string_view>

namespace
{
    // placed outside of TEST_CASE to avoid any weirdness
    constexpr caff::source_location fake()
    {
        constexpr auto ret = caff::source_location::current();
        return ret;
    }
}

TEST_CASE("source_location")
{
    constexpr auto sl = fake();

#if __has_include(<source_location>) && __cpp_lib_source_location
    static_assert(sl.line() == 11);
    //static_assert(sl.column() == 56);
    static_assert(std::string_view{ sl.file_name() }.ends_with("source_location_tests.cpp"));
    static_assert(std::string_view{ sl.function_name() }.contains("fake"));
#else
    static_assert(sl.line() == 1);
    static_assert(sl.column() == 1);
    static_assert(sl.file_name() == std::string_view{ "file name" });
    static_assert(sl.function_name() == std::string_view{ "function name" });
#endif
}
