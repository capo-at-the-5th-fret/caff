#include <doctest/doctest.h>
#include "easy/type_list.h"

TEST_CASE("standard_character_types")
{
    using T = easy::standard_character_types;

    static_assert(std::tuple_size_v<T> == 5);
    static_assert(std::is_same_v<std::tuple_element_t<0, T>, char>);
    static_assert(std::is_same_v<std::tuple_element_t<1, T>, wchar_t>);
    static_assert(std::is_same_v<std::tuple_element_t<2, T>, char8_t>);
    static_assert(std::is_same_v<std::tuple_element_t<3, T>, char16_t>);
    static_assert(std::is_same_v<std::tuple_element_t<4, T>, char32_t>);

    REQUIRE(std::tuple_size_v<T> == 5);
    REQUIRE(std::is_same_v<std::tuple_element_t<0, T>, char>);
    REQUIRE(std::is_same_v<std::tuple_element_t<1, T>, wchar_t>);
    REQUIRE(std::is_same_v<std::tuple_element_t<2, T>, char8_t>);
    REQUIRE(std::is_same_v<std::tuple_element_t<3, T>, char16_t>);
    REQUIRE(std::is_same_v<std::tuple_element_t<4, T>, char32_t>);
}
