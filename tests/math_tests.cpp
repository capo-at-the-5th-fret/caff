#include <doctest/doctest.h>
#include "easy/math.h"

#include <array>
#include "easy/type_list.h"

namespace
{
    constexpr std::array is_even_test_values =
    {
        std::tuple{ 0, true },
        std::tuple{ 1, false },
        std::tuple{ 2, true },
        std::tuple{ 3, false },
        std::tuple{ 4, true },
    };
}

TEST_CASE_TEMPLATE_DEFINE("is_even", TestType, is_even_test_id)
{
    for (int i = 0; auto const& [test_value, expected] :
        is_even_test_values)
    {
        CAPTURE(i++);
        REQUIRE(easy::is_even(test_value) == expected);

        if constexpr (std::is_signed_v<TestType>)
        {
            REQUIRE(easy::is_even(-test_value) == expected);
        }
    }
}
TEST_CASE_TEMPLATE_APPLY(is_even_test_id, easy::standard_integer_types);

TEST_CASE_TEMPLATE_DEFINE("is_odd", TestType, is_odd_test_id)
{
    for (int i = 0; auto const& [test_value, even_expected] :
        is_even_test_values)
    {
        const bool odd_expected{ !even_expected };
        CAPTURE(i++);
        REQUIRE(easy::is_odd(test_value) == odd_expected);

        if constexpr (std::is_signed_v<TestType>)
        {
            REQUIRE(easy::is_odd(-test_value) == odd_expected);
        }
    }
}
TEST_CASE_TEMPLATE_APPLY(is_odd_test_id, easy::standard_integer_types);

namespace
{
    // clang-format off
    template <typename T>
    using evenly_divisible_test_types =
        std::conditional_t<
            std::is_same_v<T, int>,
            std::tuple<std::tuple<int, int>>,   // only need to test int,int,
                                                // the others are redundant
            std::tuple
            <
                std::tuple<T,T>,                // test T,T/T,int/int,T
                std::tuple<T,int>,
                std::tuple<int,T>
            >
        >;
    // clang-format on
}

TEST_CASE_TEMPLATE_DEFINE("evenly_divisible", TestType,
    evenly_divisible_test_id)
{
    using test_types_t = evenly_divisible_test_types<TestType>;
    using easy::evenly_divisible;

    easy::tuple_for_each_type<test_types_t>([]<typename T>
    {
        using lhs_t = std::tuple_element_t<0,T>;
        using rhs_t = std::tuple_element_t<1,T>;

        const rhs_t rhs{ 3 };
        REQUIRE(evenly_divisible(lhs_t{0}, rhs));
        REQUIRE_FALSE(evenly_divisible(lhs_t{1}, rhs));
        REQUIRE_FALSE(evenly_divisible(lhs_t{2}, rhs));
        REQUIRE(evenly_divisible(lhs_t{3}, rhs));
        REQUIRE_FALSE(evenly_divisible(lhs_t{4}, rhs));
        REQUIRE_FALSE(evenly_divisible(lhs_t{5}, rhs));
        REQUIRE(evenly_divisible(lhs_t{6}, rhs));

        if constexpr (std::is_signed_v<TestType>)
        {
            REQUIRE_FALSE(evenly_divisible(lhs_t{-1}, rhs));
            REQUIRE_FALSE(evenly_divisible(lhs_t{-2}, rhs));
            REQUIRE(evenly_divisible(lhs_t{-3}, rhs));
            REQUIRE_FALSE(evenly_divisible(lhs_t{-4}, rhs));
            REQUIRE_FALSE(evenly_divisible(lhs_t{-5}, rhs));
            REQUIRE(evenly_divisible(lhs_t{-6}, rhs));
        }
    });
    
}
TEST_CASE_TEMPLATE_APPLY(evenly_divisible_test_id,
    easy::standard_integer_types);
