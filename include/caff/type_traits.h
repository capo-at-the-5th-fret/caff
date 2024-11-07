#pragma once

#include <type_traits>
#include <tuple>

namespace caff
{
    template <typename... Ts>
    struct always_false : std::false_type
    {
    };

    template <typename... Ts>
    inline constexpr bool always_false_v = always_false<Ts...>::value;

    template <typename T,
              template <typename, typename>
              typename Trait,
              typename... Ts>
    struct is_all_of : std::conjunction<Trait<T, Ts>...>
    {
    };

    template <typename T,
              template <typename, typename>
              typename Trait,
              typename... Ts>
    inline constexpr bool is_all_of_v = is_all_of<T, Trait, Ts...>::value;

    template <typename T,
              template <typename, typename>
              typename Trait,
              typename... Ts>
    struct is_any_of : std::disjunction<Trait<T, Ts>...>
    {
    };

    template <typename T,
              template <typename, typename>
              typename Trait,
              typename... Ts>
    inline constexpr bool is_any_of_v = is_any_of<T, Trait, Ts...>::value;

    template <typename T,
              template <typename, typename>
              typename Trait,
              typename... Ts>
    struct is_none_of : std::negation<is_any_of<T, Trait, Ts...>>
    {
    };

    template <typename T,
              template <typename, typename>
              typename Trait,
              typename... Ts>
    inline constexpr bool is_none_of_v = is_none_of<T, Trait, Ts...>::value;

    // same
    template <typename T, typename... Ts>
    struct is_all_of_same : is_all_of<T, std::is_same, Ts...>
    {
    };

    template <typename T, typename... Ts>
    inline constexpr bool is_all_of_same_v = is_all_of_same<T, Ts...>::value;

    template <typename T, typename... Ts>
    struct is_any_of_same : is_any_of<T, std::is_same, Ts...>
    {
    };

    template <typename T, typename... Ts>
    inline constexpr bool is_any_of_same_v = is_any_of_same<T, Ts...>::value;

    template <typename T, typename... Ts>
    struct is_none_of_same : is_none_of<T, std::is_same, Ts...>
    {
    };

    template <typename T, typename... Ts>
    inline constexpr bool is_none_of_same_v = is_none_of_same<T, Ts...>::value;

    template <typename T>
    struct is_boolean : std::is_same<bool, std::remove_cv_t<T>>
    {
    };

    template <typename T>
    inline constexpr bool is_boolean_v = is_boolean<T>::value;

    template <typename T>
    struct is_standard_integer : std::conjunction<std::is_integral<T>,
        is_none_of_same<std::remove_cv_t<T>, bool, char, wchar_t, char8_t,
        char16_t, char32_t>> {};

    template <typename T>
    inline constexpr bool is_standard_integer_v = is_standard_integer<T>::value;

    // Reference:
    // https://en.cppreference.com/w/cpp/language/cv

    template <typename T>
    struct is_cv_qualifiable
        : std::bool_constant<!std::is_function_v<T> && !std::is_reference_v<T>>
    {
    };

    template <typename T>
    inline constexpr bool is_cv_qualifiable_v = is_cv_qualifiable<T>::value;

    // A cv qualified type set is a tuple where:
    // element 0 = T, element 1 = const T, element 2 = volatile T,
    // element 3 = const volatile T

    template <typename T>
    struct is_cv_qualified_set : std::false_type
    {
    };

    template <typename T, typename CT, typename VT, typename CVT>
        requires(is_cv_qualifiable_v<T> && !std::is_const_v<T> &&
                 !std::is_volatile_v<T> &&
                 std::is_same_v<CT, std::add_const_t<T>> &&
                 std::is_same_v<VT, std::add_volatile_t<T>> &&
                 std::is_same_v<CVT, std::add_cv_t<T>>)
    struct is_cv_qualified_set<std::tuple<T, CT, VT, CVT>> : std::true_type
    {
    };

    template <typename T>
    inline constexpr bool is_cv_qualified_set_v = is_cv_qualified_set<T>::value;

    template <typename T>
    concept cv_qualified_set = is_cv_qualified_set_v<T>;

    template <typename T>
        requires(is_cv_qualifiable_v<T>)
    using cv_qualified_set_t =
        std::tuple<std::remove_cv_t<T>,
                   std::add_const_t<std::remove_cv_t<T>>,
                   std::add_volatile_t<std::remove_cv_t<T>>,
                   std::add_cv_t<std::remove_cv_t<T>>>;

    template <typename T>
    requires cv_qualified_set<T>
    using non_qualified_type = std::tuple_element_t<0, T>;

    template <typename T>
    requires cv_qualified_set<T>
    using const_type = std::tuple_element_t<1, T>;

    template <typename T>
    requires cv_qualified_set<T>
    using volatile_type = std::tuple_element_t<2, T>;

    template <typename T>
    requires cv_qualified_set<T>
    using cv_type = std::tuple_element_t<3, T>;
}
