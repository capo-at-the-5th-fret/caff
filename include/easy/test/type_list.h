#pragma once

#include "easy/type_list.h"
#include "easy/test/dummy.h"

namespace easy::test
{
    // A sample type from each of the 14 primary type categories
    // REFERENCE:
    // https://en.cppreference.com/w/cpp/header/type_traits

    using primary_types = easy::tuple_cat_t
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

    using cv_qualifiable_types = easy::tuple_cat_t
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
    //static_assert(is_qualifiable_type<qualifiable_types>)
}
