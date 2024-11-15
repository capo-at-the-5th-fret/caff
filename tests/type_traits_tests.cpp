#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include "caff/type_traits.h"

#include "caff/test/type_list.h"

TEMPLATE_LIST_TEST_CASE("always_false", "[type_traits]",
    caff::test::primary_types)
{
    if constexpr (caff::is_cv_qualifiable_v<TestType>)
    {
        using qts_t = caff::cv_qualified_set_t<TestType>;

        caff::tuple_enumerate_types<qts_t>([]<auto I, typename T>()
        {
            CAPTURE(I);

            static_assert(!caff::always_false<TestType>::value);
            static_assert(!caff::always_false_v<TestType>);

            static_assert(!caff::always_false<TestType, double>::value);
            static_assert(!caff::always_false_v<TestType, double>);
        });
    }
    else
    {
        static_assert(!caff::always_false<TestType>::value);
        static_assert(!caff::always_false_v<TestType>);

        static_assert(!caff::always_false<TestType, double>::value);
        static_assert(!caff::always_false_v<TestType, double>);
    }

    CHECK(true);
}

TEST_CASE("is_all_of", "[type_traits]")
{
    using caff::is_all_of;
    using caff::is_all_of_v;

    SECTION("no types")
    {
        static_assert(is_all_of<int, std::is_same>::value);
        static_assert(is_all_of_v<int, std::is_same>);
    }

    SECTION("single type")
    {
        static_assert(is_all_of<int, std::is_same, int>::value);
        static_assert(!is_all_of<int, std::is_same, double>::value);

        static_assert(is_all_of_v<int, std::is_same, int>);
        static_assert(!is_all_of_v<int, std::is_same, double>);
    }

    SECTION("multiple types")
    {
        static_assert(is_all_of<int, std::is_same, int, int>::value);
        static_assert(!is_all_of<int, std::is_same, int, double>::value);
        static_assert(!is_all_of<int, std::is_same, double, int>::value);

        static_assert(is_all_of_v<int, std::is_same, int, int>);
        static_assert(!is_all_of_v<int, std::is_same, int, double>);
        static_assert(!is_all_of_v<int, std::is_same, double, int>);
    }

    CHECK(true);
}

TEST_CASE("is_any_of", "[type_traits]")
{
    using caff::is_any_of;
    using caff::is_any_of_v;

    SECTION("no types")
    {
        static_assert(!is_any_of<int, std::is_same>::value);
        static_assert(!is_any_of_v<int, std::is_same>);
    }

    SECTION("single type")
    {
        static_assert(is_any_of<int, std::is_same, int>::value);
        static_assert(!is_any_of<int, std::is_same, double>::value);

        static_assert(is_any_of_v<int, std::is_same, int>);
        static_assert(!is_any_of_v<int, std::is_same, double>);
    }

    SECTION("multiple types")
    {
        static_assert(is_any_of<int, std::is_same, int, int>::value);
        static_assert(is_any_of<int, std::is_same, int, double>::value);
        static_assert(is_any_of<int, std::is_same, double, int>::value);
        static_assert(!is_any_of<int, std::is_same, double, short>::value);

        static_assert(is_any_of_v<int, std::is_same, int, int>);
        static_assert(is_any_of_v<int, std::is_same, int, double>);
        static_assert(is_any_of_v<int, std::is_same, double, int>);
        static_assert(!is_any_of_v<int, std::is_same, double, short>);
    }

    CHECK(true);
}

TEST_CASE("is_none_of", "[type_traits]")
{
    using caff::is_none_of;
    using caff::is_none_of_v;

    SECTION("no types")
    {
        static_assert(is_none_of<int, std::is_same>::value);
        static_assert(is_none_of_v<int, std::is_same>);
    }

    SECTION("single type")
    {
        static_assert(!is_none_of<int, std::is_same, int>::value);
        static_assert(is_none_of<int, std::is_same, double>::value);

        static_assert(!is_none_of_v<int, std::is_same, int>);
        static_assert(is_none_of_v<int, std::is_same, double>);
    }

    SECTION("multiple types")
    {
        static_assert(!is_none_of<int, std::is_same, int, int>::value);
        static_assert(!is_none_of<int, std::is_same, int, double>::value);
        static_assert(!is_none_of<int, std::is_same, double, int>::value);
        static_assert(is_none_of<int, std::is_same, double, short>::value);

        static_assert(!is_none_of_v<int, std::is_same, int, int>);
        static_assert(!is_none_of_v<int, std::is_same, int, double>);
        static_assert(!is_none_of_v<int, std::is_same, double, int>);
        static_assert(is_none_of_v<int, std::is_same, double, short>);
    }

    CHECK(true);
}

TEST_CASE("is_all_of_same", "[type_traits]")
{
    using caff::is_all_of_same;
    using caff::is_all_of_same_v;

    SECTION("no types")
    {
        static_assert(is_all_of_same<int>::value);
        static_assert(is_all_of_same_v<int>);
    }

    SECTION("single type")
    {
        static_assert(is_all_of_same<int, int>::value);
        static_assert(!is_all_of_same<int, double>::value);

        static_assert(is_all_of_same_v<int, int>);
        static_assert(!is_all_of_same_v<int, double>);
    }

    SECTION("multiple types")
    {
        static_assert(is_all_of_same<int, int, int>::value);
        static_assert(!is_all_of_same<int, int, double>::value);
        static_assert(!is_all_of_same<int, double, int>::value);

        static_assert(is_all_of_same_v<int, int, int>);
        static_assert(!is_all_of_same_v<int, int, double>);
        static_assert(!is_all_of_same_v<int, double, int>);
    }

    CHECK(true);
}

TEST_CASE("is_any_of_same", "[type_traits]")
{
    using caff::is_any_of_same;
    using caff::is_any_of_same_v;

    SECTION("no types")
    {
        static_assert(!is_any_of_same<int>::value);
        static_assert(!is_any_of_same_v<int>);
    }

    SECTION("single type")
    {
        static_assert(is_any_of_same<int, int>::value);
        static_assert(!is_any_of_same<int, double>::value);

        static_assert(is_any_of_same_v<int, int>);
        static_assert(!is_any_of_same_v<int, double>);
    }

    SECTION("multiple types")
    {
        static_assert(is_any_of_same<int, int, int>::value);
        static_assert(is_any_of_same<int, int, double>::value);
        static_assert(is_any_of_same<int, double, int>::value);
        static_assert(!is_any_of_same<int, double, short>::value);

        static_assert(is_any_of_same_v<int, int, int>);
        static_assert(is_any_of_same_v<int, int, double>);
        static_assert(is_any_of_same_v<int, double, int>);
        static_assert(!is_any_of_same_v<int, double, short>);
    }

    CHECK(true);
}

TEST_CASE("is_none_of_same", "[type_traits]")
{
    using caff::is_none_of_same;
    using caff::is_none_of_same_v;

    SECTION("no types")
    {
        static_assert(is_none_of_same<int>::value);
        static_assert(is_none_of_same_v<int>);
    }

    SECTION("single type")
    {
        static_assert(!is_none_of_same<int, int>::value);
        static_assert(is_none_of_same<int, double>::value);

        static_assert(!is_none_of_same_v<int, int>);
        static_assert(is_none_of_same_v<int, double>);
    }

    SECTION("multiple types")
    {
        static_assert(!is_none_of_same<int, int, int>::value);
        static_assert(!is_none_of_same<int, int, double>::value);
        static_assert(!is_none_of_same<int, double, int>::value);
        static_assert(is_none_of_same<int, double, short>::value);

        static_assert(!is_none_of_same_v<int, int, int>);
        static_assert(!is_none_of_same_v<int, int, double>);
        static_assert(!is_none_of_same_v<int, double, int>);
        static_assert(is_none_of_same_v<int, double, short>);
    }

    CHECK(true);
}

TEMPLATE_LIST_TEST_CASE("is_boolean", "[type_traits]",
    caff::test::primary_types)
{
    if constexpr (caff::is_cv_qualifiable_v<TestType>)
    {
        using qts_t = caff::cv_qualified_set_t<TestType>;

        caff::tuple_enumerate_types<qts_t>([]<auto I, typename T>()
        {
            CAPTURE(I);

            constexpr bool expected = std::is_same_v<qts_t,
                caff::cv_qualified_set_t<bool>>;

            static_assert(caff::is_boolean<T>::value == expected);
            static_assert(caff::is_boolean_v<T> == expected);
        });
    }
    else
    {
        static_assert(!caff::is_boolean<TestType>::value);
        static_assert(!caff::is_boolean_v<TestType>);
    }

    CHECK(true);
}

TEMPLATE_LIST_TEST_CASE("is_standard_integer", "[type_traits]",
    caff::test::primary_types)
{
    if constexpr (caff::is_cv_qualifiable_v<TestType>)
    {
        using qts_t = caff::cv_qualified_set_t<TestType>;
    
        caff::tuple_enumerate_types<qts_t>([]<auto I, typename T>()
        {
            CAPTURE(I);
    
            constexpr bool expected = caff::tuple_contains_type_v<
                std::remove_cv_t<T>, caff::standard_integer_types>;
        
            static_assert(caff::is_standard_integer<T>::value == expected);
            static_assert(caff::is_standard_integer_v<T> == expected);
        });
    }
    else
    {
        static_assert(!caff::is_standard_integer<TestType>::value);
        static_assert(!caff::is_standard_integer_v<TestType>);
    }

    CHECK(true);
}

TEST_CASE("is_cv_qualifiable")
{
    caff::tuple_for_each_type<caff::test::primary_types>([]<typename T>
    {
        constexpr bool expected_value = (!std::is_function_v<T> &&
            !std::is_reference_v<T>);
        static_assert(caff::is_cv_qualifiable<T>::value == expected_value);
        static_assert(caff::is_cv_qualifiable_v<T> == expected_value);

        if constexpr (expected_value)
        {
            static_assert(std::is_const_v<std::add_const_t<T>>);
            static_assert(std::is_volatile_v<std::add_volatile_t<T>>);
            using cv_t = std::add_cv_t<T>;
            static_assert(std::is_const_v<cv_t> && std::is_volatile_v<cv_t>);
        }
    });

    CHECK(true);
}

namespace
{
    template <typename T>
    using valid_qst = std::tuple<T, const T, volatile T, const volatile T>;

    template<typename T>
    using invalid_qsts = std::tuple
    <
        // wrong type
        T,
        const T,
        volatile T,
        const volatile T,

        // proper type, wrong size
        std::tuple<T>,
        std::tuple<T, const T>,
        std::tuple<T, const T, volatile T>,
        std::tuple<T, const T, volatile T, const volatile T, T>,

        // proper type and size, wrong qualifier order
        std::tuple<T, const T, volatile T, T>,
        std::tuple<T, const T, volatile T, const T>,
        std::tuple<T, const T, volatile T, volatile T>,

        std::tuple<T, const T, T, const volatile T>,
        std::tuple<T, const T, const T, const volatile T>,
        std::tuple<T, const T, const volatile T, const volatile T>,

        std::tuple<T, T, volatile T, const volatile T>,
        std::tuple<T, volatile T, volatile T, const volatile T>,
        std::tuple<T, const volatile T, volatile T, const volatile T>,

        std::tuple<const T, const T, volatile T, const volatile T>,
        std::tuple<volatile T, const T, volatile T, const volatile T>,
        std::tuple<const volatile T, const T, volatile T, const volatile T>
    >;
}

TEMPLATE_LIST_TEST_CASE("is_cv_qualified_set", "[type_traits]",
    caff::test::cv_qualifiable_types)
{
    using T = TestType;

    SECTION("successes")
    {
        using qst = valid_qst<T>;
        static_assert(caff::is_cv_qualified_set<qst>::value);
        static_assert(caff::is_cv_qualified_set_v<qst>);
    }

    SECTION("failures")
    {
        using testTypes = invalid_qsts<T>;

        caff::tuple_for_each_type<testTypes>([]<typename U>
        {
            static_assert(!caff::is_cv_qualified_set<U>::value);
            static_assert(!caff::is_cv_qualified_set_v<U>);
        });
   }

   CHECK(true);
}

TEMPLATE_LIST_TEST_CASE("cv_qualified_type_set", "[type_traits]",
    caff::test::cv_qualifiable_types)
{
    using T = TestType;

    SECTION("make using non-qualified type")
    {
        using qts_t = caff::cv_qualified_set_t<T>;
        static_assert(std::is_same_v<caff::non_qualified_type<qts_t>, T>);
        static_assert(std::is_same_v<caff::const_type<qts_t>,
            std::add_const_t<T>>);
        static_assert(std::is_same_v<caff::volatile_type<qts_t>,
            std::add_volatile_t<T>>);
        static_assert(std::is_same_v<caff::cv_type<qts_t>, std::add_cv_t<T>>);
    }

    SECTION("make using const type")
    {
        using qts_t = caff::cv_qualified_set_t<std::add_const_t<T>>;
        static_assert(std::is_same_v<caff::non_qualified_type<qts_t>, T>);
        static_assert(std::is_same_v<caff::const_type<qts_t>,
            std::add_const_t<T>>);
        static_assert(std::is_same_v<caff::volatile_type<qts_t>,
            std::add_volatile_t<T>>);
        static_assert(std::is_same_v<caff::cv_type<qts_t>, std::add_cv_t<T>>);
    }

    SECTION("make using volatile type")
    {
        using qts_t = caff::cv_qualified_set_t<std::add_volatile_t<T>>;
        static_assert(std::is_same_v<caff::non_qualified_type<qts_t>, T>);
        static_assert(std::is_same_v<caff::const_type<qts_t>,
            std::add_const_t<T>>);
        static_assert(std::is_same_v<caff::volatile_type<qts_t>,
            std::add_volatile_t<T>>);
        static_assert(std::is_same_v<caff::cv_type<qts_t>, std::add_cv_t<T>>);
    }

    SECTION("make using const volatile type")
    {
        using qts_t = caff::cv_qualified_set_t<std::add_cv_t<T>>;
        static_assert(std::is_same_v<caff::non_qualified_type<qts_t>, T>);
        static_assert(std::is_same_v<caff::const_type<qts_t>,
            std::add_const_t<T>>);
        static_assert(std::is_same_v<caff::volatile_type<qts_t>,
            std::add_volatile_t<T>>);
        static_assert(std::is_same_v<caff::cv_type<qts_t>, std::add_cv_t<T>>);
    }

    CHECK(true);
}
