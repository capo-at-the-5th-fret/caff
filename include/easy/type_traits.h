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

    template <typename T>
    struct is_boolean : std::is_same<bool, std::remove_cv_t<T>>
    {
    };

    template <typename T>
    inline constexpr bool is_boolean_v = is_boolean<T>::value;

    // Reference:
    // https://en.cppreference.com/w/cpp/language/cv

    template <typename T>
    struct is_cv_qualifiable
        : std::bool_constant<!std::is_reference_v<T> && !std::is_function_v<T>>
    {
    };

    template <typename T>
    inline constexpr bool is_cv_qualifiable_v =
        is_cv_qualifiable<T>::value;

    template <typename T>
    concept cv_qualifiable_type = is_cv_qualifiable_v<T>;

}
