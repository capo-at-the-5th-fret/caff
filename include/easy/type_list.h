#pragma once

#include <tuple>

namespace easy
{
    // clang-format off
    using standard_character_types = std::tuple
    <
        char,
        wchar_t,
        char8_t,
        char16_t,
        char32_t
    >;
    // clang-format on

}