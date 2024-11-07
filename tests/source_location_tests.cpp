#include <doctest/doctest.h>
#include "caff/source_location.h"

#include <string_view>

TEST_CASE("source_location")
{
    constexpr auto sl = caff::source_location::current();

#if __has_include(<source_location>) && __cpp_lib_source_location
    static_assert(sl.line() == 8);
    //static_assert(sl.column() == 56);
    //static_assert(sl.file_name() == std::string_view{ "file name" });
    //static_assert(sl.function_name() == std::string_view{ "function name" });
#else
    static_assert(sl.line() == 1);
    static_assert(sl.column() == 1);
    static_assert(sl.file_name() == std::string_view{ "file name" });
    static_assert(sl.function_name() == std::string_view{ "function name" });
#endif
}
