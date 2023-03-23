#include <doctest/doctest.h>
#include "easy/variant.h"

#include "easy/type_traits.h"
#include "easy/tuple.h"
#include "easy/test/type_list.h"

TEST_CASE_TEMPLATE_DEFINE("variant_contains_type", TestType,
    variant_contains_type_test_id)
{
    using var_t = std::variant<int, volatile double, const bool,
        const volatile char>;

    if constexpr (easy::is_cv_qualifiable_v<TestType>)
    {
        using qts_t = easy::cv_qualified_set_t<TestType>;
        easy::tuple_enumerate_types<qts_t>([]<auto I, typename T>()
        {
            CAPTURE(I);

            constexpr bool expected = std::disjunction_v
            <
                std::is_same<T, int>,
                std::is_same<T, volatile double>,
                std::is_same<T, const bool>,
                std::is_same<T, const volatile char>
            >;

            static_assert(easy::variant_contains_type<T, var_t>::value ==
                expected);
            static_assert(easy::variant_contains_type_v<T, var_t> ==
                expected);
        });
    }
    else
    {
        static_assert(!easy::variant_contains_type<TestType, var_t>::value);
        static_assert(!easy::variant_contains_type_v<TestType, var_t>);
    }
}
TEST_CASE_TEMPLATE_APPLY(variant_contains_type_test_id,
    easy::test::primary_types);
