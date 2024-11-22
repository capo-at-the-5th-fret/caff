#include <catch2/catch_template_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include "caff/cmath.h"

#include "caff/type_list.h"

TEMPLATE_LIST_TEST_CASE("mod functions using signed types", "[cmath][mod]",
    caff::signed_standard_integer_types)
{
    using T = TestType;
    static_assert(std::is_signed_v<T>);

    auto [x, y, native_expected, truncated_expected, floored_expected, euclidean_expected] =
        GENERATE(table<T,T,T,T,T,T>(
    {
        // x, y, native, truncated, floored, euclidean

        // positive, positive
        { T{ 0 }, T{ 3 }, T{ 0 }, T{ 0 }, T{ 0 }, T{ 0 } },
        { T{ 1 }, T{ 3 }, T{ 1 }, T{ 1 }, T{ 1 }, T{ 1 } },
        { T{ 2 }, T{ 3 }, T{ 2 }, T{ 2 }, T{ 2 }, T{ 2 } },
        { T{ 3 }, T{ 3 }, T{ 0 }, T{ 0 }, T{ 0 }, T{ 0 } },
        { T{ 4 }, T{ 3 }, T{ 1 }, T{ 1 }, T{ 1 }, T{ 1 } },
        { T{ 5 }, T{ 3 }, T{ 2 }, T{ 2 }, T{ 2 }, T{ 2 } },
        { T{ 6 }, T{ 3 }, T{ 0 }, T{ 0 }, T{ 0 }, T{ 0 } },

        // positive, negative
        { T{ 0 }, T{ -3 }, T{ 0 }, T{ 0 }, T{ 0 }, T{ 0 } },
        { T{ 1 }, T{ -3 }, T{ 1 }, T{ 1 }, T{ -2 }, T{ 1 } },
        { T{ 2 }, T{ -3 }, T{ 2 }, T{ 2 }, T{ -1 }, T{ 2 } },
        { T{ 3 }, T{ -3 }, T{ 0 }, T{ 0 }, T{ 0 },  T{ 0 } },
        { T{ 4 }, T{ -3 }, T{ 1 }, T{ 1 }, T{ -2 }, T{ 1 } },
        { T{ 5 }, T{ -3 }, T{ 2 }, T{ 2 }, T{ -1 }, T{ 2 } },
        { T{ 6 }, T{ -3 }, T{ 0 }, T{ 0 }, T{ 0 },  T{ 0 } },

        // negative, positive
        { T{ -0 }, T{ 3 }, T{ 0 }, T{ 0 }, T{ 0 }, T{ 0 } },
        { T{ -1 }, T{ 3 }, T{ -1 }, T{ -1 }, T{ 2 }, T{ 2 } },
        { T{ -2 }, T{ 3 }, T{ -2 }, T{ -2 }, T{ 1 }, T{ 1 } },
        { T{ -3 }, T{ 3 }, T{ 0 },  T{ 0 },  T{ 0 }, T{ 0 } },
        { T{ -4 }, T{ 3 }, T{ -1 }, T{ -1 }, T{ 2 }, T{ 2 } },
        { T{ -5 }, T{ 3 }, T{ -2 },  T{ -2 }, T{ 1 }, T{ 1 } },
        { T{ -6 }, T{ 3 }, T{ 0 },  T{ 0 },  T{ 0 }, T{ 0 } },

        // negative, negative
        { T{ -0 }, T{ -3 }, T{ 0 }, T{ 0 }, T{ 0 }, T{ 0 } },
        { T{ -1 }, T{ -3 }, T{ -1 }, T{ -1 }, T{ -1 }, T{ 2 } },
        { T{ -2 }, T{ -3 }, T{ -2 }, T{ -2 }, T{ -2 }, T{ 1 } },
        { T{ -3 }, T{ -3 }, T{ 0 },  T{ 0 },  T{ 0 },  T{ 0 } },
        { T{ -4 }, T{ -3 }, T{ -1 }, T{ -1 }, T{ -1 }, T{ 2 } },
        { T{ -5 }, T{ -3 }, T{ -2 }, T{ -2 }, T{ -2 }, T{ 1 } },
        { T{ -6 }, T{ -3 }, T{ 0 },  T{ 0 },  T{ 0 },  T{ 0 } },
    }));

    CHECK(caff::mod_native(x, y) == native_expected);
    CHECK(caff::mod_truncated(x, y) == truncated_expected);
    CHECK(caff::mod_floored(x, y) == floored_expected);
    CHECK(caff::mod_euclidean(x, y) == euclidean_expected);
    CHECK(caff::mod_native(x, y) == caff::mod_truncated(x, y));
}

TEMPLATE_LIST_TEST_CASE("mod functions using unsigned types", "[cmath][mod]",
    caff::unsigned_standard_integer_types)
{
    using T = TestType;
    static_assert(std::is_unsigned_v<T>);

    auto [x, expected] = GENERATE(table<T,T>(
    {
        { T{ 0 }, T{ 0 } },
        { T{ 1 }, T{ 1 } },
        { T{ 2 }, T{ 2 } },
        { T{ 3 }, T{ 0 } },
        { T{ 4 }, T{ 1 } },
        { T{ 5 }, T{ 2 } },
        { T{ 6 }, T{ 0 } }
    }));

    const T y{ 3 };

    CHECK(caff::mod_native(x, y) == expected);
    CHECK(caff::mod_truncated(x, y) == expected);
    CHECK(caff::mod_floored(x, y) == expected);
    CHECK(caff::mod_euclidean(x, y) == expected);
    CHECK(caff::mod_native(x, y) == caff::mod_truncated(x, y));
}

TEMPLATE_LIST_TEST_CASE("is_even using signed types", "[cmath][is_even]",
    caff::signed_standard_integer_types)
{
    using T = TestType;
    static_assert(std::is_signed_v<T>);

    auto [x, expected] = GENERATE(table<T,bool>(
    {
        { T{ 0 }, true },
        { T{ 1 }, false },
        { T{ 2 }, true },
        { T{ 3 }, false },
        { T{ 4 }, true },
        { T{ -1 }, false },
        { T{ -2 }, true },
        { T{ -3 }, false },
        { T{ -4 }, true }
    }));

    CHECK(caff::is_even(x) == expected);
}

TEMPLATE_LIST_TEST_CASE("is_even using unsigned types", "[cmath][is_even]",
    caff::unsigned_standard_integer_types)
{
    using T = TestType;
    static_assert(std::is_unsigned_v<T>);

    auto [x, expected] = GENERATE(table<T,bool>(
    {
        { T{ 0 }, true },
        { T{ 1 }, false },
        { T{ 2 }, true },
        { T{ 3 }, false },
        { T{ 4 }, true }
    }));

    CHECK(caff::is_even(x) == expected);
}

TEMPLATE_LIST_TEST_CASE("is_odd using signed types", "[cmath][is_odd]",
    caff::signed_standard_integer_types)
{
    using T = TestType;
    static_assert(std::is_signed_v<T>);

    auto [x, expected] = GENERATE(table<T,bool>(
    {
        { T{ 0 }, false },
        { T{ 1 }, true },
        { T{ 2 }, false },
        { T{ 3 }, true },
        { T{ 4 }, false },
        { T{ -1 }, true },
        { T{ -2 }, false },
        { T{ -3 }, true },
        { T{ -4 }, false }
    }));

    CHECK(caff::is_odd(x) == expected);
}

TEMPLATE_LIST_TEST_CASE("is_odd using unsigned types", "[cmath][is_odd]",
    caff::unsigned_standard_integer_types)
{
    using T = TestType;
    static_assert(std::is_unsigned_v<T>);

    auto [x, expected] = GENERATE(table<T,bool>(
    {
        { T{ 0 }, false },
        { T{ 1 }, true },
        { T{ 2 }, false },
        { T{ 3 }, true },
        { T{ 4 }, false }
    }));

    CHECK(caff::is_odd(x) == expected);
}

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

TEMPLATE_LIST_TEST_CASE("evenly_divisible", "[cmath][even_divisible]",
    caff::standard_integer_types)
{
    using test_types_t = evenly_divisible_test_types<TestType>;
    using caff::evenly_divisible;

    caff::tuple_for_each_type<test_types_t>([]<typename T>
    {
        using x_type = std::tuple_element_t<0,T>;
        using y_type = std::tuple_element_t<1,T>;

        const y_type rhs{ 3 };
        CHECK(evenly_divisible(x_type{0}, rhs));
        CHECK_FALSE(evenly_divisible(x_type{1}, rhs));
        CHECK_FALSE(evenly_divisible(x_type{2}, rhs));
        CHECK(evenly_divisible(x_type{3}, rhs));
        CHECK_FALSE(evenly_divisible(x_type{4}, rhs));
        CHECK_FALSE(evenly_divisible(x_type{5}, rhs));
        CHECK(evenly_divisible(x_type{6}, rhs));

        if constexpr (std::is_signed_v<TestType>)
        {
            CHECK_FALSE(evenly_divisible(x_type{-1}, rhs));
            CHECK_FALSE(evenly_divisible(x_type{-2}, rhs));
            CHECK(evenly_divisible(x_type{-3}, rhs));
            CHECK_FALSE(evenly_divisible(x_type{-4}, rhs));
            CHECK_FALSE(evenly_divisible(x_type{-5}, rhs));
            CHECK(evenly_divisible(x_type{-6}, rhs));
        }
    });   
}

#if 0
// Helper macro to generate test cases for each combination of dividend and divisor types
#define TEST_EVENLY_DIVISIBLE_FOR_TYPE_COMBINATIONS(dividend_type, divisor_type) \
    TEMPLATE_TEST_CASE_SIG( \
        "evenly_divisible - Test each dividend as a standard integer type vs divisor of all standard integer types", \
        "[evenly_divisible]", \
        ((typename T, typename U), T, U), \
        dividend_type, \
        divisor_type \
    ) \
    { \
        T dividend = static_cast<T>(20); \
        U divisor_even = static_cast<U>(5); \
        U divisor_odd = static_cast<U>(3); \
        U divisor_zero = static_cast<U>(0); \
        \
        SECTION("Evenly divisible test") \
        { \
            REQUIRE(evenly_divisible(dividend, divisor_even) == true); \
        } \
        \
        SECTION("Not evenly divisible test") \
        { \
            REQUIRE(evenly_divisible(dividend, divisor_odd) == false); \
        } \
        \
        SECTION("Division by zero") \
        { \
            REQUIRE_THROWS_AS(evenly_divisible(dividend, divisor_zero), std::domain_error); \
        } \
    }

// Generate test cases for signed and unsigned combinations
TEST_EVENLY_DIVISIBLE_FOR_TYPE_COMBINATIONS(signed char, signed char)
TEST_EVENLY_DIVISIBLE_FOR_TYPE_COMBINATIONS(signed char, unsigned char)
TEST_EVENLY_DIVISIBLE_FOR_TYPE_COMBINATIONS(short, unsigned char)
TEST_EVENLY_DIVISIBLE_FOR_TYPE_COMBINATIONS(int, unsigned short)
TEST_EVENLY_DIVISIBLE_FOR_TYPE_COMBINATIONS(long, unsigned int)
TEST_EVENLY_DIVISIBLE_FOR_TYPE_COMBINATIONS(long long, unsigned long)
TEST_EVENLY_DIVISIBLE_FOR_TYPE_COMBINATIONS(unsigned char, signed char)
TEST_EVENLY_DIVISIBLE_FOR_TYPE_COMBINATIONS(unsigned short, int)
TEST_EVENLY_DIVISIBLE_FOR_TYPE_COMBINATIONS(unsigned int, long)
TEST_EVENLY_DIVISIBLE_FOR_TYPE_COMBINATIONS(unsigned long, long long)
TEST_EVENLY_DIVISIBLE_FOR_TYPE_COMBINATIONS(unsigned long long, unsigned short)
#endif