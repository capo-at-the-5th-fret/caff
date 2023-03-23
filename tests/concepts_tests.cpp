#include <doctest/doctest.h>
#include "easy/concepts.h"

#include "easy/test/type_list.h"

TEST_CASE_TEMPLATE_DEFINE("boolean", TestType, boolean_test_id)
{
    if constexpr (easy::is_cv_qualifiable_v<TestType>)
    {
        using qts_t = easy::cv_qualified_set_t<TestType>;

        easy::tuple_enumerate_types<qts_t>([]<auto I, typename T>()
        {
            CAPTURE(I);

            constexpr bool expected = std::is_same_v<std::remove_cv_t<T>, bool>;
            static_assert(easy::boolean<T> == expected);
        });
    }
    else
    {
        static_assert(!easy::boolean<TestType>);
    }
}
TEST_CASE_TEMPLATE_APPLY(boolean_test_id, easy::test::primary_types);

TEST_CASE_TEMPLATE_DEFINE("standard_integer", TestType,
    standard_integer_test_id)
{
    if constexpr (easy::is_cv_qualifiable_v<TestType>)
    {
        using qts_t = easy::cv_qualified_set_t<TestType>;
    
        easy::tuple_enumerate_types<qts_t>([]<auto I, typename T>()
        {
            CAPTURE(I);
    
            constexpr bool expected = easy::tuple_contains_type_v<
                std::remove_cv_t<T>, easy::standard_integer_types>;
        
            static_assert(easy::standard_integer<T> == expected);
        });
    }
    else
    {
        static_assert(!easy::standard_integer<TestType>);
    }
}
TEST_CASE_TEMPLATE_APPLY(standard_integer_test_id, easy::test::primary_types);

TEST_CASE_TEMPLATE_DEFINE("signed_standard_integer", TestType,
    signed_standard_integer_test_id)
{
    if constexpr (easy::is_cv_qualifiable_v<TestType>)
    {
        using qts_t = easy::make_cv_qualified_type_set<TestType>;
    
        easy::tuple_enumerate_types<qts_t>([]<auto I, typename T>()
        {
            CAPTURE(I);
    
            constexpr bool expected = easy::tuple_contains_type_v<
                std::remove_cv_t<T>, easy::signed_standard_integer_types>;
        
            static_assert(easy::signed_standard_integer<T> == expected);
        });
    }
    else
    {
        static_assert(!easy::signed_standard_integer<TestType>);
    }
}
TEST_CASE_TEMPLATE_APPLY(signed_standard_integer_test_id,
    easy::test::primary_types);

TEST_CASE_TEMPLATE_DEFINE("unsigned_standard_integer", TestType,
    unsigned_standard_integer_test_id)
{
    if constexpr (easy::is_cv_qualifiable_v<TestType>)
    {
        using qts_t = easy::make_cv_qualified_type_set<TestType>;
    
        easy::tuple_enumerate_types<qts_t>([]<auto I, typename T>()
        {
            CAPTURE(I);
    
            constexpr bool expected = easy::tuple_contains_type_v<
                std::remove_cv_t<T>, easy::unsigned_standard_integer_types>;
        
            static_assert(easy::unsigned_standard_integer<T> == expected);
        });
    }
    else
    {
        static_assert(!easy::unsigned_standard_integer<TestType>);
    }
}
TEST_CASE_TEMPLATE_APPLY(unsigned_standard_integer_test_id,
    easy::test::primary_types);

template <typename T>
struct tuple_cv_qualifiable_set_or_identity
{
    using type = std::tuple<T>;
};

template <typename T>
requires (easy::is_cv_qualifiable_v<T>)
struct tuple_cv_qualifiable_set_or_identity<T>
{
    using type = easy::make_cv_qualified_type_set<T>;
};

template <typename T>
using tuple_cv_qualifiable_or_identity_t =
    typename tuple_cv_qualifiable_set_or_identity<T>::type;

TEST_CASE_TEMPLATE_DEFINE("all_of_same", TestType, all_of_same_test_id)
{
    using test_types_t = tuple_cv_qualifiable_or_identity_t<TestType>;
    /*using test_types_t = std::conditional_t
    <
        easy::is_cv_qualifiable_v<TestType>,
        easy::make_cv_qualified_type_set<TestType>,
        std::tuple<TestType>
    >;*/

#if 0
    easy::tuple_enumerate_types<test_types_t>([]<auto I, typename T>()
    {
        CAPTURE(I);

        static_assert(easy::all_of_same<T>);
        static_assert(easy::all_of_same<T, T>);
        static_assert(easy::all_of_same<T, T, T>);

        static_assert(!easy::all_of_same<T,
            easy::test::other_dummy_class>);
        static_assert(!easy::all_of_same<T, T,
            easy::test::other_dummy_class>);
        static_assert(!easy::all_of_same<T,
            easy::test::other_dummy_class, T>);
        static_assert(!easy::all_of_same<T,
            easy::test::other_dummy_class,
            easy::test::other_dummy_class>);
    });
#endif
}
TEST_CASE_TEMPLATE_APPLY(all_of_same_test_id, easy::test::primary_types);

TEST_CASE_TEMPLATE_DEFINE("any_of_same", TestType, any_of_same_test_id)
{
    if constexpr (easy::is_cv_qualifiable_v<TestType>)
    {
        using qts_t = easy::make_cv_qualified_type_set<TestType>;
    
        easy::tuple_enumerate_types<qts_t>([]<auto I, typename T>()
        {
            CAPTURE(I);
    
            static_assert(!easy::any_of_same<T>);
            static_assert(easy::any_of_same<T, T>);
            static_assert(easy::any_of_same<T, T, T>);

            static_assert(!easy::any_of_same<T,
                easy::test::other_dummy_class>);
            static_assert(easy::any_of_same<T, T,
                easy::test::other_dummy_class>);
            static_assert(easy::any_of_same<T,
                easy::test::other_dummy_class, T>);
            static_assert(!easy::any_of_same<T,
                easy::test::other_dummy_class,
                easy::test::other_dummy_class>);
        });
    }
    else
    {
        static_assert(!easy::any_of_same<TestType>);
        static_assert(easy::any_of_same<TestType, TestType>);
        static_assert(easy::any_of_same<TestType, TestType, TestType>);

        static_assert(!easy::any_of_same<TestType,
            easy::test::other_dummy_class>);
        static_assert(easy::any_of_same<TestType, TestType,
            easy::test::other_dummy_class>);
        static_assert(easy::any_of_same<TestType,
            easy::test::other_dummy_class, TestType>);
        static_assert(!easy::any_of_same<TestType,
            easy::test::other_dummy_class,
            easy::test::other_dummy_class>);
    }
}
TEST_CASE_TEMPLATE_APPLY(any_of_same_test_id, easy::test::primary_types);

TEST_CASE_TEMPLATE_DEFINE("none_of_same", TestType, none_of_same_test_id)
{
    if constexpr (easy::is_cv_qualifiable_v<TestType>)
    {
        using qts_t = easy::make_cv_qualified_type_set<TestType>;
    
        easy::tuple_enumerate_types<qts_t>([]<auto I, typename T>()
        {
            CAPTURE(I);
    
            static_assert(easy::none_of_same<T>);
            static_assert(!easy::none_of_same<T, T>);
            static_assert(!easy::none_of_same<T, T, T>);

            static_assert(easy::none_of_same<T,
                easy::test::other_dummy_class>);
            static_assert(!easy::none_of_same<T, T,
                easy::test::other_dummy_class>);
            static_assert(!easy::none_of_same<T,
                easy::test::other_dummy_class, T>);
            static_assert(easy::none_of_same<T,
                easy::test::other_dummy_class,
                easy::test::other_dummy_class>);
        });
    }
    else
    {
        static_assert(easy::none_of_same<TestType>);
        static_assert(!easy::none_of_same<TestType, TestType>);
        static_assert(!easy::none_of_same<TestType, TestType, TestType>);

        static_assert(easy::none_of_same<TestType,
            easy::test::other_dummy_class>);
        static_assert(!easy::none_of_same<TestType, TestType,
            easy::test::other_dummy_class>);
        static_assert(!easy::none_of_same<TestType,
            easy::test::other_dummy_class, TestType>);
        static_assert(easy::none_of_same<TestType,
            easy::test::other_dummy_class,
            easy::test::other_dummy_class>);
    }
}
TEST_CASE_TEMPLATE_APPLY(none_of_same_test_id, easy::test::primary_types);

TEST_CASE_TEMPLATE_DEFINE("n_of_same", TestType, n_of_same_test_id)
{
    if constexpr (easy::is_cv_qualifiable_v<TestType>)
    {
        using qts_t = easy::make_cv_qualified_type_set<TestType>;
    
        easy::tuple_enumerate_types<qts_t>([]<auto I, typename T>()
        {
            CAPTURE(I);
    
            static_assert(easy::n_of_same<T,0>);
            static_assert(!easy::n_of_same<T,1>);

            static_assert(!easy::n_of_same<T,0,T>);
            static_assert(easy::n_of_same<T,1,T>);
            static_assert(!easy::n_of_same<T,2,T>);

            static_assert(!easy::n_of_same<T,1,T,T>);
            static_assert(easy::n_of_same<T,2,T,T>);
            static_assert(!easy::n_of_same<T,3,T,T>);

            static_assert(!easy::n_of_same<T,0,
                easy::test::other_dummy_class>);
            static_assert(!easy::n_of_same<T,1,
                easy::test::other_dummy_class>);
            static_assert(!easy::n_of_same<T,2,
                easy::test::other_dummy_class>);

            static_assert(!easy::n_of_same<T, 1, T,
                easy::test::other_dummy_class>);
            static_assert(!easy::n_of_same<T, 2, T,
                easy::test::other_dummy_class>);
            static_assert(!easy::n_of_same<T, 3, T,
                easy::test::other_dummy_class>);

            static_assert(!easy::n_of_same<T, 1,
                easy::test::other_dummy_class, T>);
            static_assert(!easy::n_of_same<T, 2,
                easy::test::other_dummy_class, T>);
            static_assert(!easy::n_of_same<T, 3,
                easy::test::other_dummy_class, T>);

            static_assert(!easy::n_of_same<T, 1,
                easy::test::other_dummy_class,
                easy::test::other_dummy_class>);
            static_assert(!easy::n_of_same<T, 2,
                easy::test::other_dummy_class,
                easy::test::other_dummy_class>);
            static_assert(!easy::n_of_same<T, 3,
                easy::test::other_dummy_class,
                easy::test::other_dummy_class>);
        });
    }
    else
    {
        static_assert(easy::n_of_same<TestType, 0>);
        static_assert(!easy::n_of_same<TestType, 1>);

        static_assert(!easy::n_of_same<TestType, 0, TestType>);
        static_assert(easy::n_of_same<TestType, 1, TestType>);
        static_assert(!easy::n_of_same<TestType, 2, TestType>);

        static_assert(!easy::n_of_same<TestType, 1, TestType, TestType>);
        static_assert(easy::n_of_same<TestType, 2, TestType, TestType>);
        static_assert(!easy::n_of_same<TestType, 3, TestType, TestType>);

        static_assert(!easy::n_of_same<TestType, 0,
            easy::test::other_dummy_class>);
        static_assert(!easy::n_of_same<TestType, 1,
            easy::test::other_dummy_class>);
        static_assert(!easy::n_of_same<TestType, 2,
            easy::test::other_dummy_class>);

        static_assert(!easy::n_of_same<TestType, 1, TestType,
            easy::test::other_dummy_class>);
        static_assert(!easy::n_of_same<TestType, 2, TestType,
            easy::test::other_dummy_class>);
        static_assert(!easy::n_of_same<TestType, 3, TestType,
            easy::test::other_dummy_class>);

        static_assert(!easy::n_of_same<TestType, 1,
            easy::test::other_dummy_class, TestType>);
        static_assert(!easy::n_of_same<TestType, 2,
            easy::test::other_dummy_class, TestType>);
        static_assert(!easy::n_of_same<TestType, 3,
            easy::test::other_dummy_class, TestType>);

        static_assert(!easy::n_of_same<TestType, 1,
            easy::test::other_dummy_class,
            easy::test::other_dummy_class>);
        static_assert(!easy::n_of_same<TestType, 2,
            easy::test::other_dummy_class,
            easy::test::other_dummy_class>);
        static_assert(!easy::n_of_same<TestType, 3,
            easy::test::other_dummy_class,
            easy::test::other_dummy_class>);
    }
}
TEST_CASE_TEMPLATE_APPLY(n_of_same_test_id, easy::test::primary_types);

TEST_CASE_TEMPLATE_DEFINE("n_range_of_same", TestType, n_range_of_same_test_id)
{
    using test_types_t = tuple_cv_qualifiable_or_identity_t<TestType>;
    /*using test_types_t = std::conditional_t
    <
        easy::is_cv_qualifiable_v<TestType>,
        easy::make_cv_qualified_type_set<TestType>,
        std::tuple<TestType>
    >;*/

    easy::tuple_enumerate_types<test_types_t>([]<auto I, typename T>()
    {
        CAPTURE(I);

        static_assert(easy::n_range_of_same<T,0,0>);
        //static_assert(!easy::n_range_of_same<T,1,0>);

#if 0
        static_assert(!easy::n_range_of_same<T,0,T>);
        static_assert(easy::n_range_of_same<T,1,T>);
        static_assert(!easy::n_range_of_same<T,2,T>);

        static_assert(!easy::n_range_of_same<T,1,T,T>);
        static_assert(easy::n_range_of_same<T,2,T,T>);
        static_assert(!easy::n_range_of_same<T,3,T,T>);

        static_assert(!easy::n_range_of_same<T,0,
            easy::test::other_dummy_class>);
        static_assert(!easy::n_range_of_same<T,1,
            easy::test::other_dummy_class>);
        static_assert(!easy::n_range_of_same<T,2,
            easy::test::other_dummy_class>);

        static_assert(!easy::n_range_of_same<T, 1, T,
            easy::test::other_dummy_class>);
        static_assert(!easy::n_range_of_same<T, 2, T,
            easy::test::other_dummy_class>);
        static_assert(!easy::n_range_of_same<T, 3, T,
            easy::test::other_dummy_class>);

        static_assert(!easy::n_range_of_same<T, 1,
            easy::test::other_dummy_class, T>);
        static_assert(!easy::n_range_of_same<T, 2,
            easy::test::other_dummy_class, T>);
        static_assert(!easy::n_range_of_same<T, 3,
            easy::test::other_dummy_class, T>);

        static_assert(!easy::n_range_of_same<T, 1,
            easy::test::other_dummy_class,
            easy::test::other_dummy_class>);
        static_assert(!easy::n_range_of_same<T, 2,
            easy::test::other_dummy_class,
            easy::test::other_dummy_class>);
        static_assert(!easy::n_range_of_same<T, 3,
            easy::test::other_dummy_class,
            easy::test::other_dummy_class>);
    #endif
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
