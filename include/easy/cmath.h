#pragma once

#include <cmath>
#include "easy/concepts.h"

namespace easy
{
    // Reference:
    // https://www.youtube.com/watch?v=xVNYurap-lk&ab_channel=C%E1%90%A9%E1%90%A9WeeklyWithJasonTurner

    template <standard_integer T>
    constexpr T mod_native(T x, T y)
    {
        return x % y;
    }

    template <standard_integer T>
    constexpr T mod_truncated(T x, T y)
    {
        // NOTE: The C++ modulo operator uses truncated algorithm
        return x % y;
        //return x - y * T(x / static_cast<double>(y));
    }

    template <standard_integer T>
    constexpr T mod_floored(T x, T y)
    {
        // NOTE: std::floor requires constexpr cmath feature
        // NOTE: The floor algorithm is faster
    #ifdef __cpp_lib_constexpr_cmath
        return x - y * T(std::floor(x / static_cast<double>(y)));
    #else
        return ((x % y) + y) % y;
    #endif
    }

    // NOTE: constexpr mod_euclidean requires __cpp_lib_constexpr_cmath
    template <standard_integer T>
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

    template <standard_integer T>
    constexpr bool is_even(T t)
    {
        return (t % 2) == 0;
    }

    template <standard_integer T>
    constexpr bool is_odd(T t)
    {
        return (t % 2) != 0;
    }

    template <standard_integer T, standard_integer U>
    constexpr bool evenly_divisible(T t, U u)
    {
        return (t % u) == 0;
    }
}
