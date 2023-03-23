#include <doctest/doctest.h>
#include "easy/concepts.h"

#include "easy/test/type_list.h"

TEST_CASE_TEMPLATE_DEFINE("boolean", TestType, boolean_test_id)
{
    if constexpr (easy::is_cv_qualifiable_v<TestType>)
    {
        using qts_t = easy::cv_qualified_set_t<TestType>;

        easy::tuple_enumerate_types<qts_t>([]<auto I, typename T>()
        {
            CAPTURE(I);

            constexpr bool expected = std::is_same_v<std::remove_cv_t<T>, bool>;
            static_assert(easy::boolean<T> == expected);
        });
    }
    else
    {
        static_assert(!easy::boolean<TestType>);
    }
}
TEST_CASE_TEMPLATE_APPLY(boolean_test_id, easy::test::primary_types);

TEST_CASE_TEMPLATE_DEFINE("standard_integer", TestType,
    standard_integer_test_id)
{
    if constexpr (easy::is_cv_qualifiable_v<TestType>)
    {
        using qts_t = easy::cv_qualified_set_t<TestType>;
    
        easy::tuple_enumerate_types<qts_t>([]<auto I, typename T>()
        {
            CAPTURE(I);
    
            constexpr bool expected = easy::tuple_contains_type_v<
                std::remove_cv_t<T>, easy::standard_integer_types>;
        
            static_assert(easy::standard_integer<T> == expected);
        });
    }
    else
    {
        static_assert(!easy::standard_integer<TestType>);
    }
}
TEST_CASE_TEMPLATE_APPLY(standard_integer_test_id, easy::test::primary_types);

TEST_CASE("cv_qualifiable")
{
    easy::tuple_for_each_type<easy::test::primary_types>([]<typename T>
    {
        constexpr bool expected_value = (!std::is_function_v<T> &&
            !std::is_reference_v<T>);
        static_assert(easy::cv_qualifiable<T> == expected_value);
    });
}
