#pragma once

#include <type_traits>

namespace easy
{
    template <typename... Ts>
    struct always_false : std::false_type
    {
    };

    template <typename... Ts>
    inline constexpr bool always_false_v = always_false<Ts...>::value;
}
