#pragma once

#include <cmath>
#include "easy/concepts.h"

namespace easy
{
    // Reference:
    // https://www.youtube.com/watch?v=xVNYurap-lk&ab_channel=C%E1%90%A9%E1%90%A9WeeklyWithJasonTurner

    template <easy::standard_integer T>
    constexpr T mod_native(T x, T y)
    {
        return x % y;
    }

    template <easy::standard_integer T>
    constexpr T mod_truncated(T x, T y)
    {
        return x - y * T(x / static_cast<double>(y));
    }

    template <easy::standard_integer T>
    constexpr T mod_floored(T x, T y)
    {
        return x - y * T(std::floor(x / static_cast<double>(y)));
    }

    template <easy::standard_integer T>
    constexpr T mod_euclidean(T x, T y)
    {
        if constexpr (std::is_signed_v<T>)
        {
            return x - std::abs(y) * T(std::floor(x / double(std::abs(y))));
        }
        else
        {
            return x - y * T(std::floor(x / double(y)));
        }
    }

    template <easy::standard_integer T>
    constexpr bool is_even(T t)
    {
        return (t % 2) == 0;
    }

    template <easy::standard_integer T>
    constexpr bool is_odd(T t)
    {
        return (t % 2) != 0;
    }

    template <easy::standard_integer T, easy::standard_integer U>
    constexpr bool evenly_divisible(T t, U u)
    {
        return (t % u) == 0;
    }
}
