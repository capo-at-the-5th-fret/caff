#include <doctest/doctest.h>
#include "caff/variant.h"

#include <string>
#include <utility>
#include <vector>
#include "caff/type_traits.h"
#include "caff/tuple.h"
#include "caff/test/type_list.h"

TEST_CASE("overload")
{
    using var_t = std::variant<int, long, double, std::string>;
    std::vector<std::pair<var_t,int>> vec =
    {
        { 10, 1 },
        { 15l, 1 },
        { 1.5, 2 },
        { "hello", 3 }
    };

    for (int i{ 0 }; const auto [v, expected] : vec)
    {
        CAPTURE(i);

        auto r = std::visit(caff::overload
        {
            [](auto arg) { return 1; },
            [](double arg) { return 2; },
            [](const std::string& arg) { return 3; }
        }, v);

        REQUIRE(r == expected);

        ++i;
    }
}

TEST_CASE_TEMPLATE_DEFINE("variant_contains_type", TestType,
    variant_contains_type_test_id)
{
    using var_t = std::variant<int, volatile double, const bool,
        const volatile char>;

    if constexpr (caff::is_cv_qualifiable_v<TestType>)
    {
        using qts_t = caff::cv_qualified_set_t<TestType>;
        caff::tuple_enumerate_types<qts_t>([]<auto I, typename T>()
        {
            CAPTURE(I);

            constexpr bool expected = std::disjunction_v
            <
                std::is_same<T, int>,
                std::is_same<T, volatile double>,
                std::is_same<T, const bool>,
                std::is_same<T, const volatile char>
            >;

            static_assert(caff::variant_contains_type<T, var_t>::value ==
                expected);
            static_assert(caff::variant_contains_type_v<T, var_t> ==
                expected);
        });
    }
    else
    {
        static_assert(!caff::variant_contains_type<TestType, var_t>::value);
        static_assert(!caff::variant_contains_type_v<TestType, var_t>);
    }
}
TEST_CASE_TEMPLATE_APPLY(variant_contains_type_test_id,
    caff::test::primary_types);
