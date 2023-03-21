#pragma once

#include "easy/concepts.h"

namespace easy
{
    template <easy::standard_integer T>
    inline constexpr bool is_even(T t)
    {
        return (t % 2) == 0;
    }

    template <easy::standard_integer T>
    inline constexpr bool is_odd(T t)
    {
        return (t % 2) != 0;
    }

    template <easy::standard_integer T, easy::standard_integer U>
    inline constexpr bool evenly_divisible(T t, U u)
    {
        return (t % u) == 0;
    }
}
