#include <doctest/doctest.h>
#include "caff/cmath.h"

#include <array>
#include "caff/type_list.h"

TEST_CASE_TEMPLATE_DEFINE("mod functions using signed types", TestType,
    mod_functions_using_signed_types_test_id)
{
    using T = TestType;

    constexpr std::array test_values =
    {
        // lhs, native, truncated, floored, euclidean

        // positive, positive
        std::tuple{ T{ 0 }, T{ 0 }, T{ 0 }, T{ 0 }, T{ 0 } },
        std::tuple{ T{ 1 }, T{ 1 }, T{ 1 }, T{ 1 }, T{ 1 } },
        std::tuple{ T{ 2 }, T{ 2 }, T{ 2 }, T{ 2 }, T{ 2 } },
        std::tuple{ T{ 3 }, T{ 0 }, T{ 0 }, T{ 0 }, T{ 0 } },
        std::tuple{ T{ 4 }, T{ 1 }, T{ 1 }, T{ 1 }, T{ 1 } },
        std::tuple{ T{ 5 }, T{ 2 }, T{ 2 }, T{ 2 }, T{ 2 } },
        std::tuple{ T{ 6 }, T{ 0 }, T{ 0 }, T{ 0 }, T{ 0 } },

        // negative, positive
        std::tuple{ T{ -1 }, T{ -1 }, T{ -1 }, T{ 2 }, T{ 2 } },
        std::tuple{ T{ -2 }, T{ -2 }, T{ -2 }, T{ 1 }, T{ 1 } },
        std::tuple{ T{ -3 }, T{ 0 },  T{ 0 },  T{ 0 }, T{ 0 } },
        std::tuple{ T{ -4 }, T{ -1 }, T{ -1 }, T{ 2 }, T{ 2 } },
        std::tuple{ T{ -5 }, T{ -2 },  T{ -2 }, T{ 1 }, T{ 1 } },
        std::tuple{ T{ -6 }, T{ 0 },  T{ 0 },  T{ 0 }, T{ 0 } },

        // positive, negative
        std::tuple{ T{ 1 }, T{ 1 }, T{ 1 }, T{ -2 }, T{ 1 } },
        std::tuple{ T{ 2 }, T{ 2 }, T{ 2 }, T{ -1 }, T{ 2 } },
        std::tuple{ T{ 3 }, T{ 0 }, T{ 0 }, T{ 0 },  T{ 0 } },
        std::tuple{ T{ 4 }, T{ 1 }, T{ 1 }, T{ -2 }, T{ 1 } },
        std::tuple{ T{ 5 }, T{ 2 }, T{ 2 }, T{ -1 }, T{ 2 } },
        std::tuple{ T{ 6 }, T{ 0 }, T{ 0 }, T{ 0 },  T{ 0 } },

        // negative, negative
        std::tuple{ T{ -1 }, T{ -1 }, T{ -1 }, T{ -1 }, T{ 2 } },
        std::tuple{ T{ -2 }, T{ -2 }, T{ -2 }, T{ -2 }, T{ 1 } },
        std::tuple{ T{ -3 }, T{ 0 },  T{ 0 },  T{ 0 },  T{ 0 } },
        std::tuple{ T{ -4 }, T{ -1 }, T{ -1 }, T{ -1 }, T{ 2 } },
        std::tuple{ T{ -5 }, T{ -2 }, T{ -2 }, T{ -2 }, T{ 1 } },
        std::tuple{ T{ -6 }, T{ 0 },  T{ 0 },  T{ 0 },  T{ 0 } },
    };

    for (int i = 0; const auto& [lhs, native_expected, truncated_expected,
        floored_expected, euclidean_expected] :
        test_values)
    {
        CAPTURE(i);

        const T rhs{ static_cast<T>((i < 13) ? 3 : -3) };
        REQUIRE(caff::mod_native(lhs, rhs) == native_expected);
        REQUIRE(caff::mod_truncated(lhs, rhs) == truncated_expected);
        REQUIRE(caff::mod_floored(lhs, rhs) == floored_expected);
        REQUIRE(caff::mod_euclidean(lhs, rhs) == euclidean_expected);
        REQUIRE(caff::mod_native(lhs, rhs) == caff::mod_truncated(lhs, rhs));
        ++i;
    }
}
TEST_CASE_TEMPLATE_APPLY(mod_functions_using_signed_types_test_id,
    caff::signed_standard_integer_types);

TEST_CASE_TEMPLATE_DEFINE("mod functions using unsigned types", TestType,
    mod_functions_using_unsigned_types_test_id)
{
    using T = TestType;

    constexpr std::array test_values =
    {
        std::tuple{ T{ 0 }, T{ 0 } },
        std::tuple{ T{ 1 }, T{ 1 } },
        std::tuple{ T{ 2 }, T{ 2 } },
        std::tuple{ T{ 3 }, T{ 0 } },
        std::tuple{ T{ 4 }, T{ 1 } },
        std::tuple{ T{ 5 }, T{ 2 } },
        std::tuple{ T{ 6 }, T{ 0 } },
    };

    const T rhs{ 3 };

    for (int i = 0; const auto& [lhs, expected] : test_values)
    {
        CAPTURE(i);
        REQUIRE(caff::mod_native(lhs, rhs) == expected);
        REQUIRE(caff::mod_truncated(lhs, rhs) == expected);
        REQUIRE(caff::mod_floored(lhs, rhs) == expected);
        REQUIRE(caff::mod_euclidean(lhs, rhs) == expected);
        REQUIRE(caff::mod_native(lhs, rhs) == caff::mod_truncated(lhs, rhs));
        ++i;
    }
}
TEST_CASE_TEMPLATE_APPLY(mod_functions_using_unsigned_types_test_id,
    caff::unsigned_standard_integer_types);

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
        REQUIRE(caff::is_even(test_value) == expected);

        if constexpr (std::is_signed_v<TestType>)
        {
            REQUIRE(caff::is_even(-test_value) == expected);
        }
    }
}
TEST_CASE_TEMPLATE_APPLY(is_even_test_id, caff::standard_integer_types);

TEST_CASE_TEMPLATE_DEFINE("is_odd", TestType, is_odd_test_id)
{
    for (int i = 0; auto const& [test_value, even_expected] :
        is_even_test_values)
    {
        const bool odd_expected{ !even_expected };
        CAPTURE(i++);
        REQUIRE(caff::is_odd(test_value) == odd_expected);

        if constexpr (std::is_signed_v<TestType>)
        {
            REQUIRE(caff::is_odd(-test_value) == odd_expected);
        }
    }
}
TEST_CASE_TEMPLATE_APPLY(is_odd_test_id, caff::standard_integer_types);

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
    using caff::evenly_divisible;

    caff::tuple_for_each_type<test_types_t>([]<typename T>
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
    caff::standard_integer_types);
