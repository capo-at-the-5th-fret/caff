#pragma once

#include <type_traits>
#include <variant>

namespace easy
{
    // clang-format off

    // Reference:
    // https://en.cppreference.com/w/cpp/utility/variant/visit

    template <typename... Ts>
    struct overload : Ts...
    {
        using Ts::operator()...;
    };

    template <typename... Ts>
    overload(Ts...) -> overload<Ts...>;

    template <typename T, typename Variant>
    constexpr inline bool variant_contains_type_v =
        []<std::size_t... Is>(std::index_sequence<Is...>)
    {
        return (... ||
            std::is_same_v<std::variant_alternative_t<Is, Variant>, T>);
    }(std::make_index_sequence<std::variant_size_v<Variant>>());
    // clang-format on

    template <typename T, typename Variant>
    struct variant_contains_type
        : std::bool_constant<variant_contains_type_v<T, Variant>>
    {
    };
}
