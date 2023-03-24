#pragma once

#include <functional>
#include <string>
#include <string_view>
#include "easy/type_traits.h"

namespace easy
{
    // Reference:
    // https://en.cppreference.com/w/cpp/container/unordered_map/find

    // clang-format off
    // Usage:
    // using my_set = std::unordered_set<std::string, string_hash, std::equal_to<>>;
    // using my_map = std::unordered_map<std::string, size_t, string_hash, std::equal_to<>>;
    // clang-format on

    template <typename CharT>
    struct basic_string_hash
    {
        using hash_type = std::hash<std::basic_string_view<CharT>>;
        using is_transparent = void;

        std::size_t operator()(const CharT* str) const
        {
            return hash_type{}(str);
        }
        std::size_t operator()(std::basic_string_view<CharT> str) const
        {
            return hash_type{}(str);
        }
        std::size_t operator()(std::basic_string<CharT> const& str) const
        {
            return hash_type{}(str);
        }
    };

    using string_hash = basic_string_hash<char>;
    using wstring_hash = basic_string_hash<wchar_t>;
    using u8string_hash = basic_string_hash<char8_t>;
    using u16string_hash = basic_string_hash<char16_t>;
    using u32string_hash = basic_string_hash<char32_t>;

    // Reference:
    // https://github.com/HowardHinnant/hash_append/issues/7
    // http://open-std.org/JTC1/SC22/WG21/docs/papers/2017/p0814r0.pdf

    namespace detail
    {
        template <typename T>
        void hash_combine_impl(std::size_t& hash_value, T const& t)
        {
            // 32-bit
            if constexpr (sizeof(std::size_t) == 4)
            {
                hash_value ^= std::hash<T>{}(t) + 0x9e3779b9U +
                              (hash_value << 6) + (hash_value >> 2);
            }
            // 64-bit
            else if constexpr (sizeof(std::size_t) == 8)
            {
                hash_value ^= std::hash<T>{}(t) + 0x9e3779b97f4a7c15LLU +
                              (hash_value << 12) + (hash_value >> 4);
            }
            else
            {
                static_assert(easy::always_false_v<T>,
                              "The size of std::size_t is not supported by "
                              "hash_combine");
            }
        }
    }

    template <typename T, typename... Types>
    inline void hash_combine(std::size_t& hash_value, const T& t,
        const Types&... args)
    {
        detail::hash_combine_impl(hash_value, t);
        ((void)detail::hash_combine_impl(hash_value, args), ...);
    }

    template <typename T, typename... Types>
    std::size_t hash(T const& t, Types const&... args)
    {
        auto ret = std::hash<T>{}(t);

        if constexpr (sizeof...(args) > 0)
        {
            ((void)detail::hash_combine_impl(ret, args), ...);
        }

        return ret;
    }
}
