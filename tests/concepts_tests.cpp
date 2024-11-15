#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include "caff/concepts.h"

#include "caff/test/type_list.h"

// TODO: Move and rename to cv_qualifiable_set_or_tuple_identity
template <typename T>
struct cv_qualifiable_set_or_tuple_identity
{
    using type = std::tuple<T>;
};

template <typename T>
requires (caff::is_cv_qualifiable_v<T>)
struct cv_qualifiable_set_or_tuple_identity<T>
{
    using type = caff::cv_qualified_set_t<T>;
};

template <typename T>
using cv_qualifiable_set_or_tuple_identity_t =
    typename cv_qualifiable_set_or_tuple_identity<T>::type;

TEMPLATE_LIST_TEST_CASE("boolean", "[concepts]", caff::test::primary_types)
{
    using test_types_t = cv_qualifiable_set_or_tuple_identity_t<TestType>;

    caff::tuple_enumerate_types<test_types_t>([]<auto I, typename T>()
    {
        CAPTURE(I);

        constexpr bool expected = caff::is_cv_qualifiable_v<T> ?
            std::is_same_v<std::remove_cv_t<T>, bool> : false;
        static_assert(caff::boolean<T> == expected);
    });

    CHECK(true);
}

TEMPLATE_LIST_TEST_CASE("standard_integer", "[concepts]",
    caff::test::primary_types)
{
    using test_types_t = cv_qualifiable_set_or_tuple_identity_t<TestType>;

    caff::tuple_enumerate_types<test_types_t>([]<auto I, typename T>()
    {
        CAPTURE(I);

        constexpr bool expected = caff::is_cv_qualifiable_v<T> ?
            caff::tuple_contains_type_v<std::remove_cv_t<T>,
            caff::standard_integer_types> : false;
       static_assert(caff::standard_integer<T> == expected);
    });

    CHECK(true);
}

TEMPLATE_LIST_TEST_CASE("signed_standard_integer", "[concepts]",
    caff::test::primary_types)
{
    using test_types_t = cv_qualifiable_set_or_tuple_identity_t<TestType>;

    caff::tuple_enumerate_types<test_types_t>([]<auto I, typename T>()
    {
        CAPTURE(I);

        constexpr bool expected = caff::is_cv_qualifiable_v<T> ?
            caff::tuple_contains_type_v<std::remove_cv_t<T>,
            caff::signed_standard_integer_types> : false;
        static_assert(caff::signed_standard_integer<T> == expected);
    });

    CHECK(true);
}

TEMPLATE_LIST_TEST_CASE("unsigned_standard_integer", "[concepts]",
    caff::test::primary_types)
{
    using test_types_t = cv_qualifiable_set_or_tuple_identity_t<TestType>;

    caff::tuple_enumerate_types<test_types_t>([]<auto I, typename T>()
    {
        CAPTURE(I);

        constexpr bool expected = caff::is_cv_qualifiable_v<T> ?
            caff::tuple_contains_type_v<std::remove_cv_t<T>,
            caff::unsigned_standard_integer_types> : false;
        static_assert(caff::unsigned_standard_integer<T> == expected);
    });

    CHECK(true);
}

TEMPLATE_LIST_TEST_CASE("all_of_same", "[concepts]", caff::test::primary_types)
{
    using test_types_t = cv_qualifiable_set_or_tuple_identity_t<TestType>;

    caff::tuple_enumerate_types<test_types_t>([]<auto I, typename T>()
    {
        using caff::test::other_dummy_class;

        CAPTURE(I);

        static_assert(caff::all_of_same<T>);
        static_assert(caff::all_of_same<T, T>);
        static_assert(caff::all_of_same<T, T, T>);

        static_assert(!caff::all_of_same<T, other_dummy_class>);
        static_assert(!caff::all_of_same<T, T, other_dummy_class>);
        static_assert(!caff::all_of_same<T, other_dummy_class, T>);
        static_assert(!caff::all_of_same<T, other_dummy_class,
            other_dummy_class>);
    });

    CHECK(true);
}

TEMPLATE_LIST_TEST_CASE("any_of_same", "[concepts]", caff::test::primary_types)
{
    using test_types_t = cv_qualifiable_set_or_tuple_identity_t<TestType>;
    
    caff::tuple_enumerate_types<test_types_t>([]<auto I, typename T>()
    {
        using caff::test::other_dummy_class;

        CAPTURE(I);

        static_assert(!caff::any_of_same<T>);
        static_assert(caff::any_of_same<T, T>);
        static_assert(caff::any_of_same<T, T, T>);

        static_assert(!caff::any_of_same<T, other_dummy_class>);
        static_assert(caff::any_of_same<T, T, other_dummy_class>);
        static_assert(caff::any_of_same<T, other_dummy_class, T>);
        static_assert(!caff::any_of_same<T, other_dummy_class,
            other_dummy_class>);
    });

    CHECK(true);
}

TEMPLATE_LIST_TEST_CASE("none_of_same", "[concepts]", caff::test::primary_types)
{
    using test_types_t = cv_qualifiable_set_or_tuple_identity_t<TestType>;

    caff::tuple_enumerate_types<test_types_t>([]<auto I, typename T>()
    {
        using caff::test::other_dummy_class;

        CAPTURE(I);

        static_assert(caff::none_of_same<T>);
        static_assert(!caff::none_of_same<T, T>);
        static_assert(!caff::none_of_same<T, T, T>);

        static_assert(caff::none_of_same<T, other_dummy_class>);
        static_assert(!caff::none_of_same<T, T, other_dummy_class>);
        static_assert(!caff::none_of_same<T, other_dummy_class, T>);
        static_assert(caff::none_of_same<T, other_dummy_class,
            other_dummy_class>);
    });

    CHECK(true);
}

TEMPLATE_LIST_TEST_CASE("n_of_same", "[concepts]", caff::test::primary_types)
{
    using test_types_t = cv_qualifiable_set_or_tuple_identity_t<TestType>;

    caff::tuple_enumerate_types<test_types_t>([]<auto I, typename T>()
    {
        using caff::test::other_dummy_class;

        CAPTURE(I);

        static_assert(caff::n_of_same<T,0>);
        static_assert(!caff::n_of_same<T,1>);

        static_assert(!caff::n_of_same<T,0,T>);
        static_assert(caff::n_of_same<T,1,T>);
        static_assert(!caff::n_of_same<T,2,T>);

        static_assert(!caff::n_of_same<T,1,T,T>);
        static_assert(caff::n_of_same<T,2,T,T>);
        static_assert(!caff::n_of_same<T,3,T,T>);

        static_assert(!caff::n_of_same<T,0, other_dummy_class>);
        static_assert(!caff::n_of_same<T,1, other_dummy_class>);
        static_assert(!caff::n_of_same<T,2, other_dummy_class>);

        static_assert(!caff::n_of_same<T, 1, T, other_dummy_class>);
        static_assert(!caff::n_of_same<T, 2, T, other_dummy_class>);
        static_assert(!caff::n_of_same<T, 3, T, other_dummy_class>);

        static_assert(!caff::n_of_same<T, 1, other_dummy_class, T>);
        static_assert(!caff::n_of_same<T, 2, other_dummy_class, T>);
        static_assert(!caff::n_of_same<T, 3, other_dummy_class, T>);

        static_assert(!caff::n_of_same<T, 1, other_dummy_class,
            other_dummy_class>);
        static_assert(!caff::n_of_same<T, 2, other_dummy_class,
            other_dummy_class>);
        static_assert(!caff::n_of_same<T, 3, other_dummy_class,
            other_dummy_class>);
    });

    CHECK(true);
}

TEMPLATE_LIST_TEST_CASE("n_range_of_same", "[concepts]",
    caff::test::primary_types)
{
    using test_types_t = cv_qualifiable_set_or_tuple_identity_t<TestType>;

    caff::tuple_enumerate_types<test_types_t>([]<auto I, typename T>()
    {
        using caff::test::other_dummy_class;

        CAPTURE(I);

        // NOTE: This won't (and shouldn't) compile due to MAX < MIN
        //static_assert(caff::n_range_of_same<T, 1, 0>);

        static_assert(caff::n_range_of_same<T,0,0>);
        static_assert(caff::n_range_of_same<T,0,1>);
        static_assert(caff::n_range_of_same<T,0,2>);
        static_assert(!caff::n_range_of_same<T,1,1>);
        static_assert(!caff::n_range_of_same<T,1,2>);

        static_assert(!caff::n_range_of_same<T,0,0,T>);
        static_assert(caff::n_range_of_same<T,0,1,T>);
        static_assert(caff::n_range_of_same<T,0,2,T>);
        static_assert(caff::n_range_of_same<T,1,1,T>);
        static_assert(caff::n_range_of_same<T,1,2,T>);
        static_assert(!caff::n_range_of_same<T,2,2,T>);
        static_assert(!caff::n_range_of_same<T,2,3,T>);

        static_assert(!caff::n_range_of_same<T,0,0,other_dummy_class>);
        static_assert(!caff::n_range_of_same<T,0,1,other_dummy_class>);
        static_assert(!caff::n_range_of_same<T,0,2,other_dummy_class>);
        static_assert(!caff::n_range_of_same<T,1,1,other_dummy_class>);
        static_assert(!caff::n_range_of_same<T,1,2,other_dummy_class>);
        static_assert(!caff::n_range_of_same<T,1,3,other_dummy_class>);
        static_assert(!caff::n_range_of_same<T,2,2,other_dummy_class>);
        static_assert(!caff::n_range_of_same<T,2,3,other_dummy_class>);
        static_assert(!caff::n_range_of_same<T,2,4,other_dummy_class>);

        static_assert(!caff::n_range_of_same<T,0,0,T,T>);
        static_assert(!caff::n_range_of_same<T,0,1,T,T>);
        static_assert(caff::n_range_of_same<T,0,2,T,T>);
        static_assert(caff::n_range_of_same<T,0,3,T,T>);
        static_assert(!caff::n_range_of_same<T,1,1,T,T>);
        static_assert(caff::n_range_of_same<T,1,2,T,T>);
        static_assert(caff::n_range_of_same<T,1,3,T,T>);
        static_assert(caff::n_range_of_same<T,2,2,T,T>);
        static_assert(caff::n_range_of_same<T,2,3,T,T>);
        static_assert(caff::n_range_of_same<T,2,4,T,T>);
        static_assert(!caff::n_range_of_same<T,3,3,T,T>);
        static_assert(!caff::n_range_of_same<T,3,4,T,T>);
        static_assert(!caff::n_range_of_same<T,3,5,T,T>);

        static_assert(!caff::n_range_of_same<T,0,0,T,other_dummy_class>);
        static_assert(!caff::n_range_of_same<T,0,1,T,other_dummy_class>);
        static_assert(!caff::n_range_of_same<T,0,2,T,other_dummy_class>);
        static_assert(!caff::n_range_of_same<T,0,3,T,other_dummy_class>);
        static_assert(!caff::n_range_of_same<T,1,1,T,other_dummy_class>);
        static_assert(!caff::n_range_of_same<T,1,2,T,other_dummy_class>);
        static_assert(!caff::n_range_of_same<T,1,3,T,other_dummy_class>);
        static_assert(!caff::n_range_of_same<T,2,2,T,other_dummy_class>);
        static_assert(!caff::n_range_of_same<T,2,3,T,other_dummy_class>);
        static_assert(!caff::n_range_of_same<T,2,4,T,other_dummy_class>);
        static_assert(!caff::n_range_of_same<T,3,3,T,other_dummy_class>);
        static_assert(!caff::n_range_of_same<T,3,4,T,other_dummy_class>);
        static_assert(!caff::n_range_of_same<T,3,5,T,other_dummy_class>);

        static_assert(!caff::n_range_of_same<T,0,0,other_dummy_class,T>);
        static_assert(!caff::n_range_of_same<T,0,1,other_dummy_class,T>);
        static_assert(!caff::n_range_of_same<T,0,2,other_dummy_class,T>);
        static_assert(!caff::n_range_of_same<T,0,3,other_dummy_class,T>);
        static_assert(!caff::n_range_of_same<T,1,1,other_dummy_class,T>);
        static_assert(!caff::n_range_of_same<T,1,2,other_dummy_class,T>);
        static_assert(!caff::n_range_of_same<T,1,3,other_dummy_class,T>);
        static_assert(!caff::n_range_of_same<T,2,2,other_dummy_class,T>);
        static_assert(!caff::n_range_of_same<T,2,3,other_dummy_class,T>);
        static_assert(!caff::n_range_of_same<T,2,4,other_dummy_class,T>);
        static_assert(!caff::n_range_of_same<T,3,3,other_dummy_class,T>);
        static_assert(!caff::n_range_of_same<T,3,4,other_dummy_class,T>);
        static_assert(!caff::n_range_of_same<T,3,5,other_dummy_class,T>);

        static_assert(!caff::n_range_of_same<T,0,0,other_dummy_class,other_dummy_class>);
        static_assert(!caff::n_range_of_same<T,0,1,other_dummy_class,other_dummy_class>);
        static_assert(!caff::n_range_of_same<T,0,2,other_dummy_class,other_dummy_class>);
        static_assert(!caff::n_range_of_same<T,0,3,other_dummy_class,other_dummy_class>);
        static_assert(!caff::n_range_of_same<T,1,1,other_dummy_class,other_dummy_class>);
        static_assert(!caff::n_range_of_same<T,1,2,other_dummy_class,other_dummy_class>);
        static_assert(!caff::n_range_of_same<T,1,3,other_dummy_class,other_dummy_class>);
        static_assert(!caff::n_range_of_same<T,2,2,other_dummy_class,other_dummy_class>);
        static_assert(!caff::n_range_of_same<T,2,3,other_dummy_class,other_dummy_class>);
        static_assert(!caff::n_range_of_same<T,2,4,other_dummy_class,other_dummy_class>);
        static_assert(!caff::n_range_of_same<T,3,3,other_dummy_class,other_dummy_class>);
        static_assert(!caff::n_range_of_same<T,3,4,other_dummy_class,other_dummy_class>);
        static_assert(!caff::n_range_of_same<T,3,5,other_dummy_class,other_dummy_class>);
    });

    CHECK(true);
}

TEMPLATE_LIST_TEST_CASE("cv_qualifiable", "[concepts]",
    caff::test::primary_types)
{
    using T = TestType;

    constexpr bool expected = (!std::is_function_v<T> &&
        !std::is_reference_v<T>);
    static_assert(caff::cv_qualifiable<T> == expected);

    CHECK(true);
}
