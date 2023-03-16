#pragma once

#include <type_traits>
#include <tuple>

namespace easy
{
    template <typename... Types>
    struct type_list
    {
        static constexpr std::size_t size =
            std::tuple_size_v<std::tuple<Types...>>;

        template <std::size_t I>
        using type = std::tuple_element_t<I, std::tuple<Types...>>;
    };
}

template <typename... Types>
struct std::tuple_size<easy::type_list<Types...>> :
    std::integral_constant<std::size_t, easy::type_list<Types...>::size> { };

/*template<std::size_t I, typename... Types>
struct std::tuple_element<I, easy::type_list<Types...>>
{
    static_assert(I < 2, "std::pair has only 2 elements!");
};*/
 
template <std::size_t I, typename... Types>
struct std::tuple_element<I, easy::type_list<Types...>>
{
    using type = typename easy::type_list<Types...>::template type<I>;
};
 
namespace easy
{
    // clang-format off
    using char_type_list = easy::type_list
    <
        //signed char,
        //unsigned char,
        char,
        wchar_t,
        char8_t,
        char16_t,
        char32_t
    >;
    // clang-format on


}