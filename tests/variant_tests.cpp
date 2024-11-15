#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include "caff/variant.h"

#include <string>
#include <utility>
#include <vector>
#include "caff/type_traits.h"
#include "caff/tuple.h"
#include "caff/test/type_list.h"

TEST_CASE("overload", "[variant]")
{
    using var_t = std::variant<int, long, double, std::string>;

    auto [v, expected] = GENERATE(table<var_t,int>({
        { 10, 1 },
        { 15l, 2 },
        { 1.5, 3 },
        { "hello", 4 }
    }));

    auto r = std::visit(caff::overload
    {
        [](long arg) { return 2; },
        [](double arg) { return 3; },
        [](const std::string& arg) { return 4; },
        [](auto arg) { return 1; },
    }, v);

    CHECK(r == expected);
}

TEMPLATE_LIST_TEST_CASE("variant_contains_type", "[variant]",
    caff::test::primary_types)
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

    CHECK(true);
}
