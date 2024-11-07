#pragma once

#include "caff/type_list.h"
#include "caff/test/dummy.h"

namespace caff::test
{
    // A sample type from each of the 14 primary type categories
    // REFERENCE:
    // https://en.cppreference.com/w/cpp/header/type_traits

    using primary_types = tuple_cat_t
    <
        std::tuple<void, std::nullptr_t>,
        integral_types,
        standard_floating_point_types,
        std::tuple
        <
            int[],
            test::dummy_enum,
            test::dummy_union,
            test::dummy_class,
            decltype(test::dummy_function),
            int*,
            int&,
            int&&,
            decltype(&test::dummy_class::member_variable),
            decltype(&test::dummy_class::member_function)
        >
    >;

    // Reference:
    // https://en.cppreference.com/w/cpp/language/cv

    // For any type T (including incomplete types), other than function type or
    // reference type, there are three more distinct types in the C++ type
    // system: const-qualified T, volatile-qualified T, and
    // const-volatile-qualified T.

    using cv_qualifiable_types = tuple_cat_t
    <
        std::tuple<void, std::nullptr_t>,
        integral_types,
        standard_floating_point_types,
        std::tuple
        <
            int[],
            test::dummy_enum,
            test::dummy_union,
            test::dummy_class,
            //decltype(test::dummy_function),
            int*,
            //int&,
            //int&&,
            decltype(&test::dummy_class::member_variable),
            decltype(&test::dummy_class::member_function)
        >
    >;

    // ensure all types in cv_qualifiable_types are cv_qualifiable
    static_assert([]<auto... Is>(std::index_sequence<Is...>)
    {
        return (... && is_cv_qualifiable_v<
            std::tuple_element_t<Is, cv_qualifiable_types>>);
    }(std::make_index_sequence<std::tuple_size_v<cv_qualifiable_types>>{}));
}
