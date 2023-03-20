#include <doctest/doctest.h>
#include "easy/type_traits.h"

#include "easy/test/type_list.h"

TEST_CASE_TEMPLATE_DEFINE("always_false", TestType, always_false_test_id)
{
    if constexpr (easy::is_cv_qualifiable_v<TestType>)
    {
        using qts_t = easy::make_cv_qualified_type_set<TestType>;

        easy::tuple_enumerate_types<qts_t>([]<auto I, typename T>()
        {
            CAPTURE(I);

            static_assert(!easy::always_false<TestType>::value);
            static_assert(!easy::always_false_v<TestType>);

            static_assert(!easy::always_false<TestType, double>::value);
            static_assert(!easy::always_false_v<TestType, double>);
        });
    }
    else
    {
        static_assert(!easy::always_false<TestType>::value);
        static_assert(!easy::always_false_v<TestType>);

        static_assert(!easy::always_false<TestType, double>::value);
        static_assert(!easy::always_false_v<TestType, double>);
    }
}

TEST_CASE_TEMPLATE_APPLY(always_false_test_id, easy::test::primary_types);

TEST_CASE_TEMPLATE_DEFINE("is_boolean", TestType, is_boolean_test_id)
{
    if constexpr (easy::is_cv_qualifiable_v<TestType>)
    {
        using qts_t = easy::make_cv_qualified_type_set<TestType>;

        easy::tuple_enumerate_types<qts_t>([]<auto I, typename T>()
        {
            CAPTURE(I);

            constexpr bool expected = std::is_same_v<qts_t,
                easy::make_cv_qualified_type_set<bool>>;

            static_assert(easy::is_boolean<T>::value == expected);
            static_assert(easy::is_boolean_v<T> == expected);
        });
    }
    else
    {
        static_assert(!easy::is_boolean<TestType>::value);
        static_assert(!easy::is_boolean_v<TestType>);
    }
}
TEST_CASE_TEMPLATE_APPLY(is_boolean_test_id, easy::test::primary_types);

TEST_CASE("is_cv_qualifiable")
{
    easy::tuple_for_each_type<easy::test::primary_types>([]<typename T>
    {
        constexpr bool expected_value = (!std::is_function_v<T> &&
            !std::is_reference_v<T>);
        static_assert(easy::is_cv_qualifiable<T>::value == expected_value);
        static_assert(easy::is_cv_qualifiable_v<T> == expected_value);

        if constexpr (expected_value)
        {
            static_assert(std::is_const_v<std::add_const_t<T>>);
            static_assert(std::is_volatile_v<std::add_volatile_t<T>>);
            using cv_t = std::add_cv_t<T>;
            static_assert(std::is_const_v<cv_t> && std::is_volatile_v<cv_t>);
        }
    });
}
