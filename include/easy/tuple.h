#pragma once

#include <type_traits>
#include <concepts>
#include <tuple>

namespace easy
{
    // Reference:
    // https://stackoverflow.com/questions/68443804/c20-concept-to-check-tuple-like-types

    template<typename T, std::size_t I>
    concept has_tuple_element =
    requires(T t)
    {
        typename std::tuple_element_t<I, std::remove_const_t<T>>;
        { get<I>(t) } -> std::convertible_to<const std::tuple_element_t<I,T>&>;
    };

    // NOTE: This lambda won't compile on clang, so the detail function was made
    // as a workaround
    namespace detail
    {
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

    namespace detail
    {
        template<typename T, typename Tuple, typename Sequence>
        struct tuple_has_type_impl;

        template <typename T, tuple_like Tuple, std::size_t... Is>
        struct tuple_has_type_impl<T, Tuple, std::index_sequence<Is...>> :
            std::disjunction<std::is_same<std::tuple_element_t<Is,Tuple>,T>...>
        {
        };
    }

    template <typename T, tuple_like Tuple>
    struct tuple_has_type : std::bool_constant<
        detail::tuple_has_type_impl<T, Tuple,
        std::make_index_sequence<std::tuple_size_v<Tuple>>>::value>
    {
    };

    template <typename T, tuple_like Tuple>
    inline constexpr bool tuple_has_type_v = tuple_has_type<T, Tuple>::value;

    // Reference:
    // https://stackoverflow.com/questions/53394100/concatenating-tuples-as-types

    template <typename... Tuples>
    using tuple_cat_t = decltype(std::tuple_cat(std::declval<Tuples>()...));

// iteration functions
    template <typename Tuple, typename F>
    requires tuple_like<std::remove_cvref_t<Tuple>>
    constexpr void for_each(Tuple&& t, F&& f)
    {
        using tuple_t = std::remove_cvref_t<Tuple>;
        [&]<auto... Is>(std::index_sequence<Is...>)
        {
            (f(std::get<Is>(std::forward<Tuple>(t))), ...);
        }(std::make_index_sequence<std::tuple_size_v<tuple_t>>{});
    }

    template <typename Tuple, typename F>
    requires tuple_like<std::remove_cvref_t<Tuple>>
    constexpr void enumerate(Tuple&& t, F&& f)
    {
        using tuple_t = std::remove_cvref_t<Tuple>;
        [&]<auto... Is>(std::index_sequence<Is...>)
        {
            (f.template operator()<Is>(
                std::get<Is>(std::forward<Tuple>(t))), ...);
        }(std::make_index_sequence<std::tuple_size_v<tuple_t>>{});
    }
}
