#pragma once

#include <cstdint>
#include <type_traits>
#include <concepts>

namespace easy
{
    // clang-format off
    template <typename T>
    concept enum_bitmask = std::is_enum_v<T> &&
        std::is_unsigned_v<std::underlying_type_t<T>> &&
    requires(T t)
    {
        //T::none;
        //T::all;
        { ~t } -> std::same_as<T>;
        { t & t } -> std::same_as<T>;
        { t | t } -> std::same_as<T>;
        { t ^ t } -> std::same_as<T>;
        { t &= t } -> std::same_as<T&>;
        { t |= t } -> std::same_as<T&>;
        { t ^= t } -> std::same_as<T&>;
    };
    // clang-format on

    template <enum_bitmask B, enum_bitmask... Bs>
    constexpr void set(B& b, B bit, Bs... bits) noexcept
    {
        b |= (bit | ... | bits);
    }

    template <enum_bitmask B, enum_bitmask... Bs>
    constexpr void reset(B& b, Bs... bits) noexcept
    {
        if constexpr (sizeof...(Bs) > 0)
        {
            b &= ~(... | bits);
        }
        else
        {
            b = static_cast<B>(0);
        }
    }

    template <enum_bitmask B, enum_bitmask... Bs>
    constexpr bool test(B b, B bit, Bs... bits) noexcept
    {
        const auto test_bits = (bit | ... | bits);
        return ((b & test_bits) == test_bits);
    }

    template <enum_bitmask B, enum_bitmask... Bs>
    constexpr void flip(B& b, B bit, Bs... bits) noexcept
    {
        b ^= (bit | ... | bits);
    }

    template <enum_bitmask B, enum_bitmask... Bs>
    constexpr bool test_all_of(B b, B bit, Bs... bits) noexcept
    {
        const auto test_bits = (bit | ... | bits);
        return ((b & test_bits) == test_bits);
    }

    template <enum_bitmask B, enum_bitmask... Bs>
    constexpr bool test_any_of(B b, B bit, Bs... bits) noexcept
    {
        return ((b & (bit | ... | bits)) != static_cast<B>(0));
    }

    template <enum_bitmask B, enum_bitmask... Bs>
    constexpr bool test_none_of(B b, B bit, Bs... bits) noexcept
    {
        return ((b & (bit | ... | bits)) == static_cast<B>(0));
    }
}
