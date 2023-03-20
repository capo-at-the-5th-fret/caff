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

TEST_CASE("is_all_of")
{
    using easy::is_all_of;
    using easy::is_all_of_v;

    SUBCASE("no types")
    {
        static_assert(is_all_of<int, std::is_same>::value);
        static_assert(is_all_of_v<int, std::is_same>);
    }

    SUBCASE("single type")
    {
        static_assert(is_all_of<int, std::is_same, int>::value);
        static_assert(!is_all_of<int, std::is_same, double>::value);

        static_assert(is_all_of_v<int, std::is_same, int>);
        static_assert(!is_all_of_v<int, std::is_same, double>);
    }

    SUBCASE("multiple types")
    {
        static_assert(is_all_of<int, std::is_same, int, int>::value);
        static_assert(!is_all_of<int, std::is_same, int, double>::value);
        static_assert(!is_all_of<int, std::is_same, double, int>::value);

        static_assert(is_all_of_v<int, std::is_same, int, int>);
        static_assert(!is_all_of_v<int, std::is_same, int, double>);
        static_assert(!is_all_of_v<int, std::is_same, double, int>);
    }
}

TEST_CASE("is_any_of")
{
    using easy::is_any_of;
    using easy::is_any_of_v;

    SUBCASE("no types")
    {
        static_assert(!is_any_of<int, std::is_same>::value);
        static_assert(!is_any_of_v<int, std::is_same>);
    }

    SUBCASE("single type")
    {
        static_assert(is_any_of<int, std::is_same, int>::value);
        static_assert(!is_any_of<int, std::is_same, double>::value);

        static_assert(is_any_of_v<int, std::is_same, int>);
        static_assert(!is_any_of_v<int, std::is_same, double>);
    }

    SUBCASE("multiple types")
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
}

TEST_CASE("is_none_of")
{
    using easy::is_none_of;
    using easy::is_none_of_v;

    SUBCASE("no types")
    {
        static_assert(is_none_of<int, std::is_same>::value);
        static_assert(is_none_of_v<int, std::is_same>);
    }

    SUBCASE("single type")
    {
        static_assert(!is_none_of<int, std::is_same, int>::value);
        static_assert(is_none_of<int, std::is_same, double>::value);

        static_assert(!is_none_of_v<int, std::is_same, int>);
        static_assert(is_none_of_v<int, std::is_same, double>);
    }

    SUBCASE("multiple types")
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
}

TEST_CASE("is_all_of_same")
{
    using easy::is_all_of_same;
    using easy::is_all_of_same_v;

    SUBCASE("no types")
    {
        static_assert(is_all_of_same<int>::value);
        static_assert(is_all_of_same_v<int>);
    }

    SUBCASE("single type")
    {
        static_assert(is_all_of_same<int, int>::value);
        static_assert(!is_all_of_same<int, double>::value);

        static_assert(is_all_of_same_v<int, int>);
        static_assert(!is_all_of_same_v<int, double>);
    }

    SUBCASE("multiple types")
    {
        static_assert(is_all_of_same<int, int, int>::value);
        static_assert(!is_all_of_same<int, int, double>::value);
        static_assert(!is_all_of_same<int, double, int>::value);

        static_assert(is_all_of_same_v<int, int, int>);
        static_assert(!is_all_of_same_v<int, int, double>);
        static_assert(!is_all_of_same_v<int, double, int>);
    }
}

TEST_CASE("is_any_of_same")
{
    using easy::is_any_of_same;
    using easy::is_any_of_same_v;

    SUBCASE("no types")
    {
        static_assert(!is_any_of_same<int>::value);
        static_assert(!is_any_of_same_v<int>);
    }

    SUBCASE("single type")
    {
        static_assert(is_any_of_same<int, int>::value);
        static_assert(!is_any_of_same<int, double>::value);

        static_assert(is_any_of_same_v<int, int>);
        static_assert(!is_any_of_same_v<int, double>);
    }

    SUBCASE("multiple types")
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
}

TEST_CASE("is_none_of_same")
{
    using easy::is_none_of_same;
    using easy::is_none_of_same_v;

    SUBCASE("no types")
    {
        static_assert(is_none_of_same<int>::value);
        static_assert(is_none_of_same_v<int>);
    }

    SUBCASE("single type")
    {
        static_assert(!is_none_of_same<int, int>::value);
        static_assert(is_none_of_same<int, double>::value);

        static_assert(!is_none_of_same_v<int, int>);
        static_assert(is_none_of_same_v<int, double>);
    }

    SUBCASE("multiple types")
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
}

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

TEST_CASE_TEMPLATE_DEFINE("is_cv_qualified_type_set", T,
    is_cv_qualified_type_set_test_id)
{
    SUBCASE("successes")
    {
        using qst = valid_qst<T>;
        static_assert(easy::is_cv_qualified_type_set<qst>::value);
        static_assert(easy::is_cv_qualified_type_set_v<qst>);
    }

    SUBCASE("failures")
    {
        using testTypes = invalid_qsts<T>;

        easy::tuple_for_each_type<testTypes>([]<typename U>
        {
            static_assert(!easy::is_cv_qualified_type_set<U>::value);
            static_assert(!easy::is_cv_qualified_type_set_v<U>);
        });
   }
}
TEST_CASE_TEMPLATE_APPLY(is_cv_qualified_type_set_test_id,
    easy::test::cv_qualifiable_types);

TEST_CASE_TEMPLATE_DEFINE("cv_qualified_type_set", T,
    cv_qualified_type_set_test_id)
{
    SUBCASE("make using non-qualified type")
    {
        using qts_t = easy::make_cv_qualified_type_set<T>;
        static_assert(std::is_same_v<easy::non_qualified_type<qts_t>, T>);
        static_assert(std::is_same_v<easy::const_type<qts_t>,
            std::add_const_t<T>>);
        static_assert(std::is_same_v<easy::volatile_type<qts_t>,
            std::add_volatile_t<T>>);
        static_assert(std::is_same_v<easy::cv_type<qts_t>, std::add_cv_t<T>>);
    }

    SUBCASE("make using const type")
    {
        using qts_t = easy::make_cv_qualified_type_set<std::add_const_t<T>>;
        static_assert(std::is_same_v<easy::non_qualified_type<qts_t>, T>);
        static_assert(std::is_same_v<easy::const_type<qts_t>,
            std::add_const_t<T>>);
        static_assert(std::is_same_v<easy::volatile_type<qts_t>,
            std::add_volatile_t<T>>);
        static_assert(std::is_same_v<easy::cv_type<qts_t>, std::add_cv_t<T>>);
    }

    SUBCASE("make using volatile type")
    {
        using qts_t = easy::make_cv_qualified_type_set<std::add_volatile_t<T>>;
        static_assert(std::is_same_v<easy::non_qualified_type<qts_t>, T>);
        static_assert(std::is_same_v<easy::const_type<qts_t>,
            std::add_const_t<T>>);
        static_assert(std::is_same_v<easy::volatile_type<qts_t>,
            std::add_volatile_t<T>>);
        static_assert(std::is_same_v<easy::cv_type<qts_t>, std::add_cv_t<T>>);
    }

    SUBCASE("make using const volatile type")
    {
        using qts_t = easy::make_cv_qualified_type_set<std::add_cv_t<T>>;
        static_assert(std::is_same_v<easy::non_qualified_type<qts_t>, T>);
        static_assert(std::is_same_v<easy::const_type<qts_t>,
            std::add_const_t<T>>);
        static_assert(std::is_same_v<easy::volatile_type<qts_t>,
            std::add_volatile_t<T>>);
        static_assert(std::is_same_v<easy::cv_type<qts_t>, std::add_cv_t<T>>);
    }
}
TEST_CASE_TEMPLATE_APPLY(cv_qualified_type_set_test_id,
    easy::test::cv_qualifiable_types);

