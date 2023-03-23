#include <doctest/doctest.h>
#include "easy/concepts.h"

#include "easy/test/type_list.h"

// TODO: Move and rename to cv_qualifiable_set_or_tuple_identity
template <typename T>
struct cv_qualifiable_set_or_tuple_identity
{
    using type = std::tuple<T>;
};

template <typename T>
requires (easy::is_cv_qualifiable_v<T>)
struct cv_qualifiable_set_or_tuple_identity<T>
{
    using type = easy::cv_qualified_set_t<T>;
};

template <typename T>
using cv_qualifiable_set_or_tuple_identity_t =
    typename cv_qualifiable_set_or_tuple_identity<T>::type;

TEST_CASE_TEMPLATE_DEFINE("boolean", TestType, boolean_test_id)
{
    using test_types_t = cv_qualifiable_set_or_tuple_identity_t<TestType>;

    easy::tuple_enumerate_types<test_types_t>([]<auto I, typename T>()
    {
        CAPTURE(I);

        constexpr bool expected = easy::is_cv_qualifiable_v<T> ?
            std::is_same_v<std::remove_cv_t<T>, bool> : false;
        static_assert(easy::boolean<T> == expected);
    });
}
TEST_CASE_TEMPLATE_APPLY(boolean_test_id, easy::test::primary_types);

TEST_CASE_TEMPLATE_DEFINE("standard_integer", TestType,
    standard_integer_test_id)
{
    using test_types_t = cv_qualifiable_set_or_tuple_identity_t<TestType>;

    easy::tuple_enumerate_types<test_types_t>([]<auto I, typename T>()
    {
        CAPTURE(I);

        constexpr bool expected = easy::is_cv_qualifiable_v<T> ?
            easy::tuple_contains_type_v<std::remove_cv_t<T>,
            easy::standard_integer_types> : false;
       static_assert(easy::standard_integer<T> == expected);
    });
}
TEST_CASE_TEMPLATE_APPLY(standard_integer_test_id, easy::test::primary_types);

TEST_CASE_TEMPLATE_DEFINE("signed_standard_integer", TestType,
    signed_standard_integer_test_id)
{
    using test_types_t = cv_qualifiable_set_or_tuple_identity_t<TestType>;

    easy::tuple_enumerate_types<test_types_t>([]<auto I, typename T>()
    {
        CAPTURE(I);

        constexpr bool expected = easy::is_cv_qualifiable_v<T> ?
            easy::tuple_contains_type_v<std::remove_cv_t<T>,
            easy::signed_standard_integer_types> : false;
        static_assert(easy::signed_standard_integer<T> == expected);
    });
}
TEST_CASE_TEMPLATE_APPLY(signed_standard_integer_test_id,
    easy::test::primary_types);

TEST_CASE_TEMPLATE_DEFINE("unsigned_standard_integer", TestType,
    unsigned_standard_integer_test_id)
{
    using test_types_t = cv_qualifiable_set_or_tuple_identity_t<TestType>;

    easy::tuple_enumerate_types<test_types_t>([]<auto I, typename T>()
    {
        CAPTURE(I);

        constexpr bool expected = easy::is_cv_qualifiable_v<T> ?
            easy::tuple_contains_type_v<std::remove_cv_t<T>,
            easy::unsigned_standard_integer_types> : false;
        static_assert(easy::unsigned_standard_integer<T> == expected);
    });
}
TEST_CASE_TEMPLATE_APPLY(unsigned_standard_integer_test_id,
    easy::test::primary_types);

TEST_CASE_TEMPLATE_DEFINE("all_of_same", TestType, all_of_same_test_id)
{
    using test_types_t = cv_qualifiable_set_or_tuple_identity_t<TestType>;

    easy::tuple_enumerate_types<test_types_t>([]<auto I, typename T>()
    {
        using easy::test::other_dummy_class;

        CAPTURE(I);

        static_assert(easy::all_of_same<T>);
        static_assert(easy::all_of_same<T, T>);
        static_assert(easy::all_of_same<T, T, T>);

        static_assert(!easy::all_of_same<T, other_dummy_class>);
        static_assert(!easy::all_of_same<T, T, other_dummy_class>);
        static_assert(!easy::all_of_same<T, other_dummy_class, T>);
        static_assert(!easy::all_of_same<T, other_dummy_class,
            other_dummy_class>);
    });
}
TEST_CASE_TEMPLATE_APPLY(all_of_same_test_id, easy::test::primary_types);

TEST_CASE_TEMPLATE_DEFINE("any_of_same", TestType, any_of_same_test_id)
{
    using test_types_t = cv_qualifiable_set_or_tuple_identity_t<TestType>;
    
    easy::tuple_enumerate_types<test_types_t>([]<auto I, typename T>()
    {
        using easy::test::other_dummy_class;

        CAPTURE(I);

        static_assert(!easy::any_of_same<T>);
        static_assert(easy::any_of_same<T, T>);
        static_assert(easy::any_of_same<T, T, T>);

        static_assert(!easy::any_of_same<T, other_dummy_class>);
        static_assert(easy::any_of_same<T, T, other_dummy_class>);
        static_assert(easy::any_of_same<T, other_dummy_class, T>);
        static_assert(!easy::any_of_same<T, other_dummy_class,
            other_dummy_class>);
    });
}
TEST_CASE_TEMPLATE_APPLY(any_of_same_test_id, easy::test::primary_types);

TEST_CASE_TEMPLATE_DEFINE("none_of_same", TestType, none_of_same_test_id)
{
    using test_types_t = cv_qualifiable_set_or_tuple_identity_t<TestType>;

    easy::tuple_enumerate_types<test_types_t>([]<auto I, typename T>()
    {
        using easy::test::other_dummy_class;

        CAPTURE(I);

        static_assert(easy::none_of_same<T>);
        static_assert(!easy::none_of_same<T, T>);
        static_assert(!easy::none_of_same<T, T, T>);

        static_assert(easy::none_of_same<T, other_dummy_class>);
        static_assert(!easy::none_of_same<T, T, other_dummy_class>);
        static_assert(!easy::none_of_same<T, other_dummy_class, T>);
        static_assert(easy::none_of_same<T, other_dummy_class,
            other_dummy_class>);
    });
}
TEST_CASE_TEMPLATE_APPLY(none_of_same_test_id, easy::test::primary_types);

TEST_CASE_TEMPLATE_DEFINE("n_of_same", TestType, n_of_same_test_id)
{
    using test_types_t = cv_qualifiable_set_or_tuple_identity_t<TestType>;

    easy::tuple_enumerate_types<test_types_t>([]<auto I, typename T>()
    {
        using easy::test::other_dummy_class;

        CAPTURE(I);

        static_assert(easy::n_of_same<T,0>);
        static_assert(!easy::n_of_same<T,1>);

        static_assert(!easy::n_of_same<T,0,T>);
        static_assert(easy::n_of_same<T,1,T>);
        static_assert(!easy::n_of_same<T,2,T>);

        static_assert(!easy::n_of_same<T,1,T,T>);
        static_assert(easy::n_of_same<T,2,T,T>);
        static_assert(!easy::n_of_same<T,3,T,T>);

        static_assert(!easy::n_of_same<T,0, other_dummy_class>);
        static_assert(!easy::n_of_same<T,1, other_dummy_class>);
        static_assert(!easy::n_of_same<T,2, other_dummy_class>);

        static_assert(!easy::n_of_same<T, 1, T, other_dummy_class>);
        static_assert(!easy::n_of_same<T, 2, T, other_dummy_class>);
        static_assert(!easy::n_of_same<T, 3, T, other_dummy_class>);

        static_assert(!easy::n_of_same<T, 1, other_dummy_class, T>);
        static_assert(!easy::n_of_same<T, 2, other_dummy_class, T>);
        static_assert(!easy::n_of_same<T, 3, other_dummy_class, T>);

        static_assert(!easy::n_of_same<T, 1, other_dummy_class,
            other_dummy_class>);
        static_assert(!easy::n_of_same<T, 2, other_dummy_class,
            other_dummy_class>);
        static_assert(!easy::n_of_same<T, 3, other_dummy_class,
            other_dummy_class>);
    });
}
TEST_CASE_TEMPLATE_APPLY(n_of_same_test_id, easy::test::primary_types);

TEST_CASE_TEMPLATE_DEFINE("n_range_of_same", TestType, n_range_of_same_test_id)
{
    using test_types_t = cv_qualifiable_set_or_tuple_identity_t<TestType>;

    easy::tuple_enumerate_types<test_types_t>([]<auto I, typename T>()
    {
        using easy::test::other_dummy_class;

        CAPTURE(I);

        // NOTE: This won't (and shouldn't) compile due to MAX < MIN
        //static_assert(easy::n_range_of_same<T, 1, 0>);

        static_assert(easy::n_range_of_same<T,0,0>);
        static_assert(easy::n_range_of_same<T,0,1>);
        static_assert(easy::n_range_of_same<T,0,2>);
        static_assert(!easy::n_range_of_same<T,1,1>);
        static_assert(!easy::n_range_of_same<T,1,2>);

        static_assert(!easy::n_range_of_same<T,0,0,T>);
        static_assert(easy::n_range_of_same<T,0,1,T>);
        static_assert(easy::n_range_of_same<T,0,2,T>);
        static_assert(easy::n_range_of_same<T,1,1,T>);
        static_assert(easy::n_range_of_same<T,1,2,T>);
        static_assert(!easy::n_range_of_same<T,2,2,T>);
        static_assert(!easy::n_range_of_same<T,2,3,T>);

        static_assert(!easy::n_range_of_same<T,0,0,other_dummy_class>);
        static_assert(!easy::n_range_of_same<T,0,1,other_dummy_class>);
        static_assert(!easy::n_range_of_same<T,0,2,other_dummy_class>);
        static_assert(!easy::n_range_of_same<T,1,1,other_dummy_class>);
        static_assert(!easy::n_range_of_same<T,1,2,other_dummy_class>);
        static_assert(!easy::n_range_of_same<T,1,3,other_dummy_class>);
        static_assert(!easy::n_range_of_same<T,2,2,other_dummy_class>);
        static_assert(!easy::n_range_of_same<T,2,3,other_dummy_class>);
        static_assert(!easy::n_range_of_same<T,2,4,other_dummy_class>);

        static_assert(!easy::n_range_of_same<T,0,0,T,T>);
        static_assert(!easy::n_range_of_same<T,0,1,T,T>);
        static_assert(easy::n_range_of_same<T,0,2,T,T>);
        static_assert(easy::n_range_of_same<T,0,3,T,T>);
        static_assert(!easy::n_range_of_same<T,1,1,T,T>);
        static_assert(easy::n_range_of_same<T,1,2,T,T>);
        static_assert(easy::n_range_of_same<T,1,3,T,T>);
        static_assert(easy::n_range_of_same<T,2,2,T,T>);
        static_assert(easy::n_range_of_same<T,2,3,T,T>);
        static_assert(easy::n_range_of_same<T,2,4,T,T>);
        static_assert(!easy::n_range_of_same<T,3,3,T,T>);
        static_assert(!easy::n_range_of_same<T,3,4,T,T>);
        static_assert(!easy::n_range_of_same<T,3,5,T,T>);

        static_assert(!easy::n_range_of_same<T,0,0,T,other_dummy_class>);
        static_assert(!easy::n_range_of_same<T,0,1,T,other_dummy_class>);
        static_assert(!easy::n_range_of_same<T,0,2,T,other_dummy_class>);
        static_assert(!easy::n_range_of_same<T,0,3,T,other_dummy_class>);
        static_assert(!easy::n_range_of_same<T,1,1,T,other_dummy_class>);
        static_assert(!easy::n_range_of_same<T,1,2,T,other_dummy_class>);
        static_assert(!easy::n_range_of_same<T,1,3,T,other_dummy_class>);
        static_assert(!easy::n_range_of_same<T,2,2,T,other_dummy_class>);
        static_assert(!easy::n_range_of_same<T,2,3,T,other_dummy_class>);
        static_assert(!easy::n_range_of_same<T,2,4,T,other_dummy_class>);
        static_assert(!easy::n_range_of_same<T,3,3,T,other_dummy_class>);
        static_assert(!easy::n_range_of_same<T,3,4,T,other_dummy_class>);
        static_assert(!easy::n_range_of_same<T,3,5,T,other_dummy_class>);

        static_assert(!easy::n_range_of_same<T,0,0,other_dummy_class,T>);
        static_assert(!easy::n_range_of_same<T,0,1,other_dummy_class,T>);
        static_assert(!easy::n_range_of_same<T,0,2,other_dummy_class,T>);
        static_assert(!easy::n_range_of_same<T,0,3,other_dummy_class,T>);
        static_assert(!easy::n_range_of_same<T,1,1,other_dummy_class,T>);
        static_assert(!easy::n_range_of_same<T,1,2,other_dummy_class,T>);
        static_assert(!easy::n_range_of_same<T,1,3,other_dummy_class,T>);
        static_assert(!easy::n_range_of_same<T,2,2,other_dummy_class,T>);
        static_assert(!easy::n_range_of_same<T,2,3,other_dummy_class,T>);
        static_assert(!easy::n_range_of_same<T,2,4,other_dummy_class,T>);
        static_assert(!easy::n_range_of_same<T,3,3,other_dummy_class,T>);
        static_assert(!easy::n_range_of_same<T,3,4,other_dummy_class,T>);
        static_assert(!easy::n_range_of_same<T,3,5,other_dummy_class,T>);

        static_assert(!easy::n_range_of_same<T,0,0,other_dummy_class,other_dummy_class>);
        static_assert(!easy::n_range_of_same<T,0,1,other_dummy_class,other_dummy_class>);
        static_assert(!easy::n_range_of_same<T,0,2,other_dummy_class,other_dummy_class>);
        static_assert(!easy::n_range_of_same<T,0,3,other_dummy_class,other_dummy_class>);
        static_assert(!easy::n_range_of_same<T,1,1,other_dummy_class,other_dummy_class>);
        static_assert(!easy::n_range_of_same<T,1,2,other_dummy_class,other_dummy_class>);
        static_assert(!easy::n_range_of_same<T,1,3,other_dummy_class,other_dummy_class>);
        static_assert(!easy::n_range_of_same<T,2,2,other_dummy_class,other_dummy_class>);
        static_assert(!easy::n_range_of_same<T,2,3,other_dummy_class,other_dummy_class>);
        static_assert(!easy::n_range_of_same<T,2,4,other_dummy_class,other_dummy_class>);
        static_assert(!easy::n_range_of_same<T,3,3,other_dummy_class,other_dummy_class>);
        static_assert(!easy::n_range_of_same<T,3,4,other_dummy_class,other_dummy_class>);
        static_assert(!easy::n_range_of_same<T,3,5,other_dummy_class,other_dummy_class>);
    });
}
TEST_CASE_TEMPLATE_APPLY(n_range_of_same_test_id, easy::test::primary_types);

TEST_CASE_TEMPLATE_DEFINE("cv_qualifiable", TestType, cv_qualifiable_test_id)
{
    using T = TestType;

    constexpr bool expected = (!std::is_function_v<T> &&
        !std::is_reference_v<T>);
    static_assert(easy::cv_qualifiable<T> == expected);
}
TEST_CASE_TEMPLATE_APPLY(cv_qualifiable_test_id, easy::test::primary_types);
