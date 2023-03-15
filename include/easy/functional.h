#pragma once

#include <functional>
#include <string_view>

namespace easy
{
    // Reference:
    // https://en.cppreference.com/w/cpp/container/unordered_map/find

    template <typename CharT>
    struct basic_string_hash
    {
        using hash_type = std::hash<std::basic_string_view<CharT>>;
        using is_transparent = void;

        std::size_t operator()(const CharT* str) const { return hash_type{}(str); }
        std::size_t operator()(std::basic_string_view<CharT> str) const { return hash_type{}(str); }
        std::size_t operator()(std::basic_string<CharT> const& str) const { return hash_type{}(str); }
    };

    using string_hash = basic_string_hash<char>;
    using wstring_hash = basic_string_hash<wchar_t>;

    // Usage:
    // using my_set = std::unordered_set<std::string, string_hash, std::equal_to<>>;
    // using my_map = std::unordered_map<std::string, size_t, string_hash, std::equal_to<>>;
}