#pragma once

#include "easy/tuple.h"

namespace easy
{
    // Reference:
    // https://en.cppreference.com/w/cpp/language/types

    // clang-format off

    using standard_character_types = std::tuple
    <
        char,
        char8_t,
        char16_t,
        char32_t,
        wchar_t
    >;

    using signed_integer_types = std::tuple
    <
        short,
        int,
        long,
        long long
    >;

    using unsigned_integer_types = std::tuple
    <
        unsigned short,
        unsigned int,
        unsigned long,
        unsigned long long
    >;

    using integer_types = tuple_cat_t
    <
        signed_integer_types,
        unsigned_integer_types
    >;

    using signed_standard_integer_types = tuple_cat_t
    <
        std::tuple<signed char>,
        signed_integer_types
    >;

    using unsigned_standard_integer_types = tuple_cat_t
    <
        std::tuple<unsigned char>,
        unsigned_integer_types
    >;

    using standard_integer_types = tuple_cat_t
    <
        signed_standard_integer_types,
        unsigned_standard_integer_types
    >;

    using integral_types = tuple_cat_t
    <
        std::tuple<bool>,
        standard_character_types,
        standard_integer_types
    >;

    using standard_floating_point_types = std::tuple
    <
        float,
        double,
        long double
    >;

    // clang-format on
}
