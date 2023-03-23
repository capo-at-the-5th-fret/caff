#pragma once

#include "easy/type_traits.h"

namespace easy
{
    template <typename T>
    concept boolean = is_boolean_v<T>;

    template <typename T>
    concept standard_integer = is_standard_integer_v<T>;

    template <typename T>
    concept signed_standard_integer =
        easy::is_standard_integer_v<T> && std::is_signed_v<T>;

    template <typename T>
    concept unsigned_standard_integer =
        easy::is_standard_integer_v<T> && std::is_unsigned_v<T>;

    template <typename T>
    concept cv_qualifiable = is_cv_qualifiable_v<T>;

    template <typename T, typename... Types>
    concept all_of_same = easy::is_all_of_same_v<T, Types...>;

    template <typename T, typename... Types>
    concept any_of_same = easy::is_any_of_same_v<T, Types...>;

    template <typename T, typename... Types>
    concept none_of_same = easy::is_none_of_same_v<T, Types...>;

    // NOTE: Use count_of if there's a need to get the number of type T in the
    // list of types. i.e. count_of<int, int, double, int> = 2

    // There must be N types in the list of types, all of which are T
    // i.e. n_of_same<int, 4, ...> means the list of types must consist of
    // exactly 4 ints
    template <typename T, std::size_t N, typename... Types>
    concept n_of_same = ((sizeof...(Types) == N) &&
        easy::is_all_of_same_v<T, Types...>);

    namespace detail
    {
        // n_range requires a MIN <= MAX
        template <std::size_t MIN, std::size_t MAX, typename T = void>
        struct valid_n_range
        {
            static_assert(easy::always_false_v<T>, "MIN must be <= MAX");
        };

        template <std::size_t MIN, std::size_t MAX>
        struct valid_n_range<MIN, MAX,
            typename std::enable_if_t<(MIN <= MAX)>> : std::true_type {};

        template <std::size_t MIN, std::size_t MAX>
        inline constexpr bool valid_n_range_v = valid_n_range<MIN,MAX>::value;
    }

    // There must be MIN to MAX (inclusive) types in the list of types, all of
    // which are T
    // i.e. n_range_of_same<int, 3, 5, ...> means the list of types must consist
    // of 3, 4 or 5 ints
    template <typename T, std::size_t MIN, std::size_t MAX, typename... Types>
    concept n_range_of_same = (detail::valid_n_range_v<MIN,MAX> &&
                             (sizeof...(Types) >= MIN &&
                              sizeof...(Types) <= MAX) &&
                             easy::is_all_of_same_v<T, Types...>);
}
