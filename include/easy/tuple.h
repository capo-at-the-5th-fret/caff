#pragma once

#include <concepts>
#include <tuple>
#include "easy/type_traits.h"

namespace easy
{
    // References:
    // https://stackoverflow.com/questions/68443804/c20-concept-to-check-tuple-like-types
    // https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2021/p2165r2.pdf

    // clang-format off
    namespace detail
    {
        template<typename T, std::size_t I>
        concept has_tuple_element =
        requires(T t)
        {
            typename std::tuple_element_t<I, std::remove_const_t<T>>;

            // NOTE: This causes a "cannot form a reference to 'void'"
            // compile-time error with std::tuple<void>;
            //{ get<I>(t) } -> std::convertible_to<const std::tuple_element_t<I,T>&>;
        };

        template <typename T, std::size_t... I>
        constexpr bool has_tuple_elements(std::index_sequence<I...>)
        { 
            return (has_tuple_element<T,I> && ...);
        };
    }

    template<typename T>
    concept tuple_like = !std::is_reference_v<T> &&
    requires(T t)
    { 
        typename std::tuple_size<T>::type;
        requires std::derived_from<
            std::tuple_size<T>,
            std::integral_constant<std::size_t, std::tuple_size_v<T>>
        >;
    } && detail::has_tuple_elements<T>(
        std::make_index_sequence<std::tuple_size_v<T>>());
    // NOTE: This lambda won't compile on clang, so the detail function was made
    // as a workaround
    /*&& []<std::size_t... I>(std::index_sequence<I...>)
    { 
        return (has_tuple_element<T,I> && ...); 
    }(std::make_index_sequence<std::tuple_size_v<T>>());*/

    // Reference:
    // https://stackoverflow.com/questions/25958259/how-do-i-find-out-if-a-tuple-contains-a-type

    template <typename T, tuple_like Tuple>
    constexpr inline bool tuple_contains_type_v =
        []<auto... Is>(std::index_sequence<Is...>)
        {
            return (... || std::is_same_v<std::tuple_element_t<Is,Tuple>,T>);
        }(std::make_index_sequence<std::tuple_size_v<Tuple>>());

    template <typename T, tuple_like Tuple>
    struct tuple_contains_type :
        std::bool_constant<tuple_contains_type_v<T, Tuple>> {};

// operations

    // Reference:
    // https://stackoverflow.com/questions/53394100/concatenating-tuples-as-types

    template <tuple_like... Tuples>
    using tuple_cat_t = decltype(std::tuple_cat(std::declval<Tuples>()...));

    template <tuple_like Tuple, typename... Types>
    using tuple_append_t = tuple_cat_t<Tuple, std::tuple<Types...>>;

    template <tuple_like Tuple, typename... Types>
    using tuple_prepend_t = tuple_cat_t<std::tuple<Types...>, Tuple>;

// special types

    // A cv qualified type set is a tuple where:
    // element 0 = T, element 1 = const T, element 2 = volatile T,
    // element 3 = const volatile T

    template <typename T>
    struct is_cv_qualified_type_set : std::false_type {};

    template <typename T, typename CT, typename VT, typename CVT>
    requires (is_cv_qualifiable_v<T> &&
        !std::is_const_v<T> && !std::is_volatile_v<T> &&
        std::is_same_v<CT, std::add_const_t<T>> &&
        std::is_same_v<VT, std::add_volatile_t<T>> &&
        std::is_same_v<CVT, std::add_cv_t<T>>)
    struct is_cv_qualified_type_set<std::tuple<T,CT,VT,CVT>> : std::true_type {};

    template <typename T>
    inline constexpr bool is_cv_qualified_type_set_v =
        is_cv_qualified_type_set<T>::value;

    template <typename T>
    concept cv_qualified_type_set = is_cv_qualified_type_set_v<T>;

    template <typename T>
    requires (cv_qualifiable_type<T>)
    using make_cv_qualified_type_set = std::tuple<
        std::remove_cv_t<T>,
        std::add_const_t<std::remove_cv_t<T>>,
        std::add_volatile_t<std::remove_cv_t<T>>,
        std::add_cv_t<std::remove_cv_t<T>>
    >;

    template <typename T>
    requires cv_qualified_type_set<T>
    using non_qualified_type = std::tuple_element_t<0,T>;

    template <typename T>
    requires cv_qualified_type_set<T>
    using const_type = std::tuple_element_t<1,T>;

    template <typename T>
    requires cv_qualified_type_set<T>
    using volatile_type = std::tuple_element_t<2,T>;

    template <typename T>
    requires cv_qualified_type_set<T>
    using cv_type = std::tuple_element_t<3,T>;

// iteration functions

    template <typename Tuple, typename F>
    requires tuple_like<std::remove_cvref_t<Tuple>>
    constexpr void tuple_for_each_index(F&& f)
    {
        using tuple_t = std::remove_cvref_t<Tuple>;
        [&]<std::size_t... Is>(std::index_sequence<Is...>)
        {
            (f.template operator()<Is>(), ...);
        }(std::make_index_sequence<std::tuple_size_v<tuple_t>>{});
    }

    template <typename Tuple, typename F>
    requires tuple_like<std::remove_cvref_t<Tuple>>
    constexpr void tuple_for_each_type(F&& f)
    {
        using tuple_t = std::remove_cvref_t<Tuple>;
        [&]<std::size_t... Is>(std::index_sequence<Is...>)
        {
            (f.template operator()<std::tuple_element_t<Is, tuple_t>>(), ...);
        }(std::make_index_sequence<std::tuple_size_v<tuple_t>>{});
    }

    template <typename Tuple, typename F>
    requires tuple_like<std::remove_cvref_t<Tuple>>
    constexpr void tuple_enumerate_types(F&& f)
    {
        using tuple_t = std::remove_cvref_t<Tuple>;
        [&]<std::size_t... Is>(std::index_sequence<Is...>)
        {
            (f.template operator()<Is,
                std::tuple_element_t<Is, tuple_t>>(), ...);
        }(std::make_index_sequence<std::tuple_size_v<tuple_t>>{});
    }

    template <typename Tuple, typename F>
    requires tuple_like<std::remove_cvref_t<Tuple>>
    constexpr void tuple_for_each(Tuple&& t, F&& f)
    {
        using tuple_t = std::remove_cvref_t<Tuple>;
        [&]<std::size_t... Is>(std::index_sequence<Is...>)
        {
            (f(std::get<Is>(std::forward<Tuple>(t))), ...);
        }(std::make_index_sequence<std::tuple_size_v<tuple_t>>{});
    }

    template <typename Tuple, typename F>
    requires tuple_like<std::remove_cvref_t<Tuple>>
    constexpr void tuple_enumerate(Tuple&& t, F&& f)
    {
        using tuple_t = std::remove_cvref_t<Tuple>;
        [&]<std::size_t... Is>(std::index_sequence<Is...>)
        {
            (f.template operator()<Is>(
                std::get<Is>(std::forward<Tuple>(t))), ...);
        }(std::make_index_sequence<std::tuple_size_v<tuple_t>>{});
    }

    // clang-format on
}
