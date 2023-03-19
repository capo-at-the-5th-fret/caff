#include <doctest/doctest.h>
#include "easy/tuple.h"

// NOTE: These four std includes are needed for tuple_like test
#include <ranges>
#include <list>
#include <iterator>
#include <array>
#include "easy/type_traits.h"
#include "easy/test/type_list.h"

namespace
{
#ifdef __cpp_lib_ranges
    using tuple_like_types = std::tuple
    <
        std::tuple<>,
        std::tuple<int>,
        std::array<int, 1>,
        std::tuple<int, float>,
        std::pair<int, float>,
        std::tuple<int, float, bool>,
        std::ranges::subrange<int*, int*>
    >;
    static_assert(std::tuple_size_v<tuple_like_types> == 7);
#else
    using tuple_like_types = std::tuple
    <
        std::tuple<>,
        std::tuple<int>,
        std::array<int, 1>,
        std::tuple<int, float>,
        std::pair<int, float>,
        std::tuple<int, float, bool>
    >;
    static_assert(std::tuple_size_v<tuple_like_types> == 6);
#endif

    inline constexpr std::array tuple_like_type_sizes
    {
        std::tuple_size_v<std::tuple_element_t<0, tuple_like_types>>,
        std::tuple_size_v<std::tuple_element_t<1, tuple_like_types>>,
        std::tuple_size_v<std::tuple_element_t<2, tuple_like_types>>,
        std::tuple_size_v<std::tuple_element_t<3, tuple_like_types>>,
        std::tuple_size_v<std::tuple_element_t<4, tuple_like_types>>,
#ifdef __cpp_lib_ranges
        std::tuple_size_v<std::tuple_element_t<5, tuple_like_types>>,
        std::tuple_size_v<std::tuple_element_t<6, tuple_like_types>>
#else
        std::tuple_size_v<std::tuple_element_t<5, tuple_like_types>>
#endif
    };
    static_assert(tuple_like_type_sizes.size() == std::tuple_size_v<tuple_like_types>);
}

namespace
{
    using tuple_cat_t_test_types = std::tuple
    <
        std::tuple<std::tuple<>, std::tuple<>, std::tuple<>>,
        std::tuple<std::tuple<>, std::tuple<int>, std::tuple<int>>,
        std::tuple<std::tuple<>, std::tuple<int,float>, std::tuple<int,float>>,
        std::tuple<std::tuple<int>, std::tuple<>, std::tuple<int>>,
        std::tuple<std::tuple<int>, std::tuple<int>, std::tuple<int,int>>,
        std::tuple<std::tuple<int>, std::tuple<int,float>, std::tuple<int,int,float>>,
        std::tuple<std::tuple<int,float>, std::tuple<>, std::tuple<int,float>>,
        std::tuple<std::tuple<int,float>, std::tuple<int>, std::tuple<int,float,int>>,
        std::tuple<std::tuple<int,float>, std::tuple<int,float>, std::tuple<int,float,int,float>>
    >;
}

TEST_CASE_TEMPLATE_DEFINE("tuple_cat_t", TestTypes, tuple_cat_t_test_id)
{
    using tuple_t_1 = std::tuple_element_t<0, TestTypes>;
    using tuple_t_2 = std::tuple_element_t<1, TestTypes>;
    using expected_type = std::tuple_element_t<2, TestTypes>;
    
    static_assert(std::is_same_v
    <
        easy::tuple_cat_t<tuple_t_1, tuple_t_2>,
        expected_type
    >);
}
TEST_CASE_TEMPLATE_APPLY(tuple_cat_t_test_id, tuple_cat_t_test_types);

namespace
{
    using tuple_append_t_test_types = std::tuple
    <
        std::tuple<std::tuple<>, std::tuple<>, std::tuple<>>,
        std::tuple<std::tuple<>, std::tuple<int>, std::tuple<int>>,
        std::tuple<std::tuple<>, std::tuple<int,float,bool>, std::tuple<int,float,bool>>,

        std::tuple<std::tuple<int>, std::tuple<>, std::tuple<int>>,
        std::tuple<std::tuple<int>, std::tuple<int>, std::tuple<int,int>>,
        std::tuple<std::tuple<int>, std::tuple<float>, std::tuple<int,float>>,
        std::tuple<std::tuple<int>, std::tuple<float,bool,char>, std::tuple<int,float,bool,char>>,

        std::tuple<std::tuple<int,float,bool>, std::tuple<>, std::tuple<int,float,bool>>,
        std::tuple<std::tuple<int,float,bool>, std::tuple<char>, std::tuple<int,float,bool,char>>,
        std::tuple<std::tuple<int,float,bool>, std::tuple<char,double,long>, std::tuple<int,float,bool,char,double,long>>
    >;

    template <typename Tuple, typename Types>
    using tuple_append_t_applier = decltype(std::apply([](auto... args)
    {
        return easy::tuple_append_t<Tuple, decltype(args)...>{};
    }, std::declval<Types>()));
}

TEST_CASE_TEMPLATE_DEFINE("tuple_append_t", TestTypes, tuple_append_t_test_id)
{
    using tuple_t = std::tuple_element_t<0, TestTypes>;
    using types_t = std::tuple_element_t<1, TestTypes>;
    using expected_type = std::tuple_element_t<2, TestTypes>;

    using t = tuple_append_t_applier<tuple_t, types_t>;
    static_assert(std::is_same_v<t, expected_type>);
}
TEST_CASE_TEMPLATE_APPLY(tuple_append_t_test_id, tuple_append_t_test_types);

namespace
{
    using tuple_prepend_t_test_types = std::tuple
    <
        std::tuple<std::tuple<>, std::tuple<>, std::tuple<>>,
        std::tuple<std::tuple<>, std::tuple<int>, std::tuple<int>>,
        std::tuple<std::tuple<>, std::tuple<int,float,bool>, std::tuple<int,float,bool>>,

        std::tuple<std::tuple<int>, std::tuple<>, std::tuple<int>>,
        std::tuple<std::tuple<int>, std::tuple<int>, std::tuple<int,int>>,
        std::tuple<std::tuple<int>, std::tuple<float>, std::tuple<float,int>>,
        std::tuple<std::tuple<int>, std::tuple<float,bool,char>, std::tuple<float,bool,char,int>>,

        std::tuple<std::tuple<int,float,bool>, std::tuple<>, std::tuple<int,float,bool>>,
        std::tuple<std::tuple<int,float,bool>, std::tuple<char>, std::tuple<char,int,float,bool>>,
        std::tuple<std::tuple<int,float,bool>, std::tuple<char,double,long>, std::tuple<char,double,long,int,float,bool>>
    >;

    template <typename Tuple, typename Types>
    using tuple_prepend_t_applier = decltype(std::apply([](auto... args)
    {
        return easy::tuple_prepend_t<Tuple, decltype(args)...>{};
    }, std::declval<Types>()));
}

TEST_CASE_TEMPLATE_DEFINE("tuple_prepend_t", TestTypes, tuple_prepend_t_test_id)
{
    using tuple_t = std::tuple_element_t<0, TestTypes>;
    using types_t = std::tuple_element_t<1, TestTypes>;
    using expected_type = std::tuple_element_t<2, TestTypes>;

    using t = tuple_prepend_t_applier<tuple_t, types_t>;
    static_assert(std::is_same_v<t, expected_type>);
}
TEST_CASE_TEMPLATE_APPLY(tuple_prepend_t_test_id, tuple_prepend_t_test_types);

TEST_CASE("is_cv_qualifiable_type")
{
    easy::tuple_for_each_type<easy::test::primary_types>([]<typename T>
    {
        constexpr bool expected_value = (!std::is_reference_v<T> &&
            !std::is_function_v<T>);
        static_assert(easy::is_cv_qualifiable_type<T>::value == expected_value);
        static_assert(easy::is_cv_qualifiable_type_v<T> == expected_value);

        if constexpr (expected_value)
        {
            static_assert(std::is_const_v<std::add_const_t<T>>);
            static_assert(std::is_volatile_v<std::add_volatile_t<T>>);
            using cv_t = std::add_cv_t<T>;
            static_assert(std::is_const_v<cv_t> && std::is_volatile_v<cv_t>);
        }
    });
}

#if 0
TEST_CASE_TEMPLATE_DEFINE("tuple_has_element_type", TestType,
    tuple_has_element_type_test_id)
{
    using tuple_t = TestType;
    using success_list = tuple_t;
    using failure_list = easy::tuple_cat_t<
        easy::qualifier_type_list<void>, std::tuple<void*>>;

    SECTION("failures")
    {
        easy::for_each<failure_list>([]<typename T>
        {
            STATIC_REQUIRE_FALSE(easy::tuple_has_type<T, tuple_t>::value);
            STATIC_REQUIRE_FALSE(easy::tuple_has_type_v<T, tuple_t>);
        });
    }

    SECTION("successes")
    {
        easy::for_each<success_list>([]<typename T>()
        {
            STATIC_REQUIRE(easy::tuple_has_type<T, tuple_t>::value);
            STATIC_REQUIRE(easy::tuple_has_type_v<T, tuple_t>);
        });
    }
    
}

TEST_CASE_TEMPLATE_APPLY(tuple_has_element_type_test_id, tuple_like_types);


TEMPLATE_LIST_TEST_CASE("tuple_has_type", "[tuple][has_type]",
    tuple_like_type_list)
{
    using tuple_t = TestType;
    using success_list = easy::to_type_list<tuple_t>;
    using failure_list = easy::append_type<
        easy::qualifier_type_list<void>, void*>;

    SECTION("failures")
    {
        easy::for_each<failure_list>([]<typename T>
        {
            STATIC_REQUIRE_FALSE(easy::tuple_has_type<T, tuple_t>::value);
            STATIC_REQUIRE_FALSE(easy::tuple_has_type_v<T, tuple_t>);
        });
    }

    SECTION("successes")
    {
        easy::for_each<success_list>([]<typename T>()
        {
            STATIC_REQUIRE(easy::tuple_has_type<T, tuple_t>::value);
            STATIC_REQUIRE(easy::tuple_has_type_v<T, tuple_t>);
        });
    }
}

TEST_CASE_TEMPLATE_DEFINE("has_tuple_element", TupleLike,
    has_tuple_element_test_id)
{
    std::size_t i{ 0 };
    easy::tuple_for_each_index<TupleLike>([&i]<std::size_t I>
    {
        CAPTURE(I);
        CAPTURE(i);
        REQUIRE(I == i);
        static_assert(easy::detail::has_tuple_element<TupleLike, I>);
        ++i;
    });

    // Non tuple-like types
    static_assert(!easy::detail::has_tuple_element<int, 0>);

    // NOTE: Won't compile
    // Out of range index
    //static_assert(!easy::has_tuple_element<TestType,
      //  std::tuple_size_v<TestType>>);
}

TEST_CASE_TEMPLATE_APPLY(has_tuple_element_test_id, tuple_like_types);

TEST_CASE("tuple_for_each_type")
{
    // NOTE: The REQUIREs here need to use a literal type as
    // std::tuple_element_t requires a compile time index which isn't available
    // in for_each_type

    static_assert(std::tuple_size_v<tuple_like_types> == 7);

    easy::tuple_enumerate_types<tuple_like_types>(
        []<std::size_t I, typename TupleLike>()
    {
        using tuple_t = TupleLike;
        static_assert(std::tuple_size_v<tuple_t> == tuple_like_type_sizes[I]);

        std::size_t i{ 0 };
        easy::tuple_for_each_type<tuple_t>([&i]<typename T>
        {
            CAPTURE(I);
            CAPTURE(i);

            REQUIRE(i < tuple_like_type_sizes[I]);

            if constexpr (I == 6)
            {
                REQUIRE(std::is_same_v<T,int*>);
            }
            else
            {
                if (i == 0)
                {
                    REQUIRE(std::is_same_v<T,int>);
                }
                else if (i == 1)
                {
                    REQUIRE(std::is_same_v<T,float>);
                }
                else if (i == 2)
                {
                    REQUIRE(std::is_same_v<T,bool>);
                }
            }

            ++i;
        });

        REQUIRE(i == tuple_like_type_sizes[I]);
    });
}

TEST_CASE_TEMPLATE_DEFINE("tuple_enumerate_types", TupleLike,
    tuple_enumerate_types_test_id)
{
    std::size_t i{ 0 };
    easy::tuple_enumerate_types<TupleLike>([&i]<std::size_t I, typename T>
    {
        CAPTURE(I);
        CAPTURE(i);
        REQUIRE(i == I);
        static_assert(std::is_same_v<T, std::tuple_element_t<I,TupleLike>>);
        ++i;
    });

    REQUIRE(i == std::tuple_size_v<TupleLike>);
}

TEST_CASE_TEMPLATE_APPLY(tuple_enumerate_types_test_id, tuple_like_types);
#endif

#if 0
TEST_CASE("enumerate_types", "[type_list][enumerate_types]")
{
    SECTION("no types")
    {
        std::size_t i{ 0 };
        easy::enumerate_types<>([&i]<auto I, typename T>
        {
            ++i;
        });

        REQUIRE(i == 0);
    }

    SECTION("one type")
    {
        std::size_t i{ 0 };
        easy::enumerate_types<int>([&i]<auto I, typename T>
        {
            REQUIRE(I == i);
            if (i == 0)
            {
                REQUIRE(std::is_same_v<T,int>);
            }
            ++i;
        });

        REQUIRE(i == 1);
    }

    SECTION("multiple types")
    {
        std::size_t i{ 0 };
        easy::enumerate_types<int,float,bool>([&i]<auto I, typename T>
        {
            REQUIRE(I == i);

            if (i == 0)
            {
                REQUIRE(std::is_same_v<T,int>);
            }
            else if (i == 1)
            {
                REQUIRE(std::is_same_v<T,float>);
            }
            else if (i == 2)
            {
                REQUIRE(std::is_same_v<T,bool>);
            }
            ++i;
        });

        REQUIRE(i == 3);
    }
}
#endif

#if 0
TEMPLATE_LIST_TEST_CASE("tuple_like - std tuple-likes",
    "[tuple][tuple_like][concepts]", tuple_like_type_list)
{
    // NOTE: std::tuple doesn't work with volatile qualifiers; volatile
    // specializations for tuple_element and tuple_size have been deprecated in
    // C++20
    // Reference:
    // https://stackoverflow.com/questions/26854320/volatile-and-const-volatile-stdtuple-and-stdget

    using qtl_t = easy::make_const_qualifier_type_list<TestType>;

    easy::enumerate<qtl_t>([]<auto I, typename T>
    {
        CAPTURE(I);
        STATIC_REQUIRE(easy::tuple_like<T>);
        STATIC_REQUIRE_FALSE(easy::tuple_like<T&>);
    });
}

TEMPLATE_LIST_TEST_CASE("tuple_like - other types",
    "[tuple][tuple_like][concepts]",
    easy::primary_type_list)
{
    using qtl_t = easy::make_qualifier_type_list<TestType>;

    easy::enumerate<qtl_t>([]<auto I, typename T>
    {
        CAPTURE(I);
        STATIC_REQUIRE_FALSE(easy::tuple_like<T>);
    });
}

TEMPLATE_LIST_TEST_CASE("tuple_has_type", "[tuple][has_type]",
    tuple_like_type_list)
{
    using tuple_t = TestType;
    using success_list = easy::to_type_list<tuple_t>;
    using failure_list = easy::append_type<
        easy::qualifier_type_list<void>, void*>;

    SECTION("failures")
    {
        easy::for_each<failure_list>([]<typename T>
        {
            STATIC_REQUIRE_FALSE(easy::tuple_has_type<T, tuple_t>::value);
            STATIC_REQUIRE_FALSE(easy::tuple_has_type_v<T, tuple_t>);
        });
    }

    SECTION("successes")
    {
        easy::for_each<success_list>([]<typename T>()
        {
            STATIC_REQUIRE(easy::tuple_has_type<T, tuple_t>::value);
            STATIC_REQUIRE(easy::tuple_has_type_v<T, tuple_t>);
        });
    }
}

TEMPLATE_PRODUCT_TEST_CASE("tuple_cat_t", "[tuple][tuple_cat][tuple_cat_type]",
    std::tuple,
    (
        (std::tuple<>, std::tuple<>, std::tuple<>, std::tuple<>),
        (std::tuple<int>, std::tuple<>, std::tuple<int>, std::tuple<int>),
        (std::tuple<int>, std::tuple<double>, std::tuple<int,double>, std::tuple<double,int>),
        (std::tuple<int,double>, std::tuple<char,float>, (std::tuple<int,double,char,float>), (std::tuple<char,float,int,double>))
    )
)
{
    using tuple1_type = std::tuple_element_t<0, TestType>;
    using tuple2_type = std::tuple_element_t<1, TestType>;
    using expected_type_1 = std::tuple_element_t<2, TestType>;
    using expected_type_2 = std::tuple_element_t<3, TestType>;

    STATIC_REQUIRE(std::is_same_v<easy::tuple_cat_t<tuple1_type, tuple2_type>,
        expected_type_1>);
    STATIC_REQUIRE(std::is_same_v<easy::tuple_cat_t<tuple2_type, tuple1_type>,
        expected_type_2>);
}

namespace
{
    struct tuple_for_each_validator
    {
        template<typename T>
        bool validate_element(int i, T&& t)
        {
            using type = std::remove_cvref_t<T>;

            if (i == 0)
            {
                return std::is_same_v<type, char> && t == 'a';
            }
            else if (i == 1)
            {
                return std::is_same_v<type, int> && t == -12;
            }
            else if (i == 2)
            {
                return std::is_same_v<type, unsigned int> && t == 13;
            }
            return false;
        }

        template<typename T>
        bool validate_const_correctness(int i, T&& t)
        {
            if (i == 0)
            {
                return std::is_same_v<const char&, decltype(t)>;
            }
            else if (i == 1)
            {
                return std::is_same_v<const int&, decltype(t)>;
            }
            else if (i == 2)
            {
                return std::is_same_v<const unsigned int&, decltype(t)>;
            }
            return false;
        }
    };

    template <typename T>
    struct tuple_fixture;

    using tuple_test_type = std::tuple<char, int, unsigned int>;

    template <>
    struct tuple_fixture<tuple_test_type> : tuple_test_type
    {
        using tuple_type = tuple_test_type;

        tuple_test_type test_value{ 'a', -12, 13 };
        tuple_test_type modified_expected{ 'b', -11, 14 };
    };

    using pair_test_type = std::pair<char, int>;

    template <>
    struct tuple_fixture<pair_test_type>
    {
        using tuple_type = pair_test_type;

        pair_test_type test_value{ 'a', -12 };
        pair_test_type modified_expected{ 'b', -11 };
    };

    using array_test_type = std::array<char,1>;

    template <>
    struct tuple_fixture<array_test_type>
    {
        using tuple_type = array_test_type;

        array_test_type test_value{ 'a' };
        array_test_type modified_expected{ 'b' };
    };
}

TEMPLATE_TEST_CASE_METHOD(tuple_fixture, "tuple_for_each",
    "[tuple][for_each]", tuple_test_type, pair_test_type, array_test_type)
{
    using fixture_t = tuple_fixture<TestType>;
    using tuple_t = typename fixture_t::tuple_type;

    tuple_t t{ fixture_t::test_value };
    auto ct = std::as_const(fixture_t::test_value);

    SECTION("verify the element types")
    {
        int i = 0;
        easy::for_each(t, [&]<typename T>(T element)
        {
            CAPTURE(i);
            tuple_for_each_validator v;
            REQUIRE(v.validate_element(i, element));
            ++i;
        });
    }

    SECTION("modify the elements")
    {
        easy::for_each(t, [](auto& element)
        {
            ++element;
        });

        REQUIRE(t == fixture_t::modified_expected);
    }

    SECTION("verify const correctness")
    {
        int i = 0;
        easy::for_each(std::as_const(t), [&](const auto& element)
        {
            CAPTURE(i);
            tuple_for_each_validator v;
            REQUIRE(v.validate_const_correctness(i, element));
            ++i;
        });
    }
}

TEMPLATE_TEST_CASE_METHOD(tuple_fixture, "tuple_enumerate",
    "[tuple][enumerate]", tuple_test_type, pair_test_type, array_test_type)
{
    using fixture_t = tuple_fixture<TestType>;
    using tuple_t = typename fixture_t::tuple_type;

    tuple_t t{ fixture_t::test_value };

    SECTION("verify the indices and element types")
    {
        easy::enumerate(t, [&t = t]<auto I>(auto element)
        {
            DYNAMIC_SECTION("verifying index and type for element " << I)
            {
                STATIC_REQUIRE(std::is_same_v<std::tuple_element_t<I, tuple_t>,
                    decltype(element)>);
                REQUIRE(std::get<I>(t) == element);
            }
        });
    }

    SECTION("modify the elements")
    {
        easy::enumerate(t, []<auto I>(auto& element)
        {
            ++element;
        });

        REQUIRE(t == fixture_t::modified_expected );
    }

    SECTION("verify const correctness")
    {
        easy::enumerate(std::as_const(t), []<auto I>(const auto& element)
        {
            DYNAMIC_SECTION("verifying constness for element " << I)
            {
                STATIC_REQUIRE(
                    std::is_const_v<std::remove_reference_t<decltype(element)>>
                );
            }
        });
    }
}
#endif


#if 0
TEST_CASE("append_type")
{
    SUBCASE("append to empty list")
    {
        // append single type
        using t1 = std::tuple<>;
        using t2 = easy::tuple_append_type_t<t1, int>;

        static_assert(std::tuple_size_v<t2> == 1);
        static_assert(std::is_same_v<std::tuple_element_t<0, t2>, int>);

        // append multiple types
        using t3 = easy::tuple_append_type_t<t1, int, float, bool>;
        static_assert(std::tuple_size_v<t3> == 3);
        static_assert(std::is_same_v<
            std::tuple_element_t<0, t3>, int>);
        static_assert(std::is_same_v<
            std::tuple_element_t<1, t3>, float>);
        static_assert(std::is_same_v<
            std::tuple_element_t<2, t3>, bool>);
    }

    SECTION("append to list with 1 type")
    {
        // append single type
        using type_list_1 = easy::type_list<int>;
        using type_list_2 = easy::append_type<type_list_1, float>;

        STATIC_REQUIRE(easy::type_list_size_v<type_list_2> == 2);
        STATIC_REQUIRE(std::is_same_v<
            easy::type_list_element_t<0, type_list_2>, int>);
        STATIC_REQUIRE(std::is_same_v<
            easy::type_list_element_t<1, type_list_2>, float>);

        // append multiple types
        using type_list_3 = easy::append_type<type_list_1, float,
            bool>;
        STATIC_REQUIRE(easy::type_list_size_v<type_list_3> == 3);
        STATIC_REQUIRE(std::is_same_v<
            easy::type_list_element_t<0, type_list_3>, int>);
        STATIC_REQUIRE(std::is_same_v<
            easy::type_list_element_t<1, type_list_3>, float>);
        STATIC_REQUIRE(std::is_same_v<
            easy::type_list_element_t<2, type_list_3>, bool>);
    }

    SECTION("append to list with multiple types")
    {
        // append single type
        using type_list_1 = easy::type_list<int, float>;
        using type_list_2 = easy::append_type<type_list_1, bool>;

        STATIC_REQUIRE(easy::type_list_size_v<type_list_2> == 3);
        STATIC_REQUIRE(std::is_same_v<
            easy::type_list_element_t<0, type_list_2>, int>);
        STATIC_REQUIRE(std::is_same_v<
            easy::type_list_element_t<1, type_list_2>, float>);
        STATIC_REQUIRE(std::is_same_v<
            easy::type_list_element_t<2, type_list_2>, bool>);

        // append multiple types
        using type_list_3 = easy::append_type<type_list_1, bool,
            char>;
        STATIC_REQUIRE(easy::type_list_size_v<type_list_3> == 4);
        STATIC_REQUIRE(std::is_same_v<
            easy::type_list_element_t<0, type_list_3>, int>);
        STATIC_REQUIRE(std::is_same_v<
            easy::type_list_element_t<1, type_list_3>, float>);
        STATIC_REQUIRE(std::is_same_v<
            easy::type_list_element_t<2, type_list_3>, bool>);
        STATIC_REQUIRE(std::is_same_v<
            easy::type_list_element_t<3, type_list_3>, char>);
    }
}
#endif

#if 0
TEST_CASE("prepend_type", "[type_list][prepend_type]")
{
    SECTION("prepend to empty list")
    {
        // prepend single type
        using type_list_1 = easy::type_list<>;
        using type_list_2 = easy::prepend_type<type_list_1, int>;

        STATIC_REQUIRE(easy::type_list_size_v<type_list_2> == 1);
        STATIC_REQUIRE(std::is_same_v<
            easy::type_list_element_t<0, type_list_2>, int>);

        // prepend multiple types
        using type_list_3 = easy::prepend_type<type_list_1, int, float,
            bool>;
        STATIC_REQUIRE(easy::type_list_size_v<type_list_3> == 3);
        STATIC_REQUIRE(std::is_same_v<
            easy::type_list_element_t<0, type_list_3>, int>);
        STATIC_REQUIRE(std::is_same_v<
            easy::type_list_element_t<1, type_list_3>, float>);
        STATIC_REQUIRE(std::is_same_v<
            easy::type_list_element_t<2, type_list_3>, bool>);
    }

    SECTION("prepend to list with 1 type")
    {
        // prepend single type
        using type_list_1 = easy::type_list<int>;
        using type_list_2 = easy::prepend_type<type_list_1, float>;

        STATIC_REQUIRE(easy::type_list_size_v<type_list_2> == 2);
        STATIC_REQUIRE(std::is_same_v<
            easy::type_list_element_t<0, type_list_2>, float>);
        STATIC_REQUIRE(std::is_same_v<
            easy::type_list_element_t<1, type_list_2>, int>);

        // prepend multiple types
        using type_list_3 = easy::prepend_type<type_list_1, bool, float>;

        STATIC_REQUIRE(easy::type_list_size_v<type_list_3> == 3);
        STATIC_REQUIRE(std::is_same_v<
            easy::type_list_element_t<0, type_list_3>, bool>);
        STATIC_REQUIRE(std::is_same_v<
            easy::type_list_element_t<1, type_list_3>, float>);
        STATIC_REQUIRE(std::is_same_v<
            easy::type_list_element_t<2, type_list_3>, int>);
    }

    SECTION("prepend to list with multiple types")
    {
        // prepend single type
        using type_list_1 = easy::type_list<float, int>;
        using type_list_2 = easy::prepend_type<type_list_1, bool>;

        STATIC_REQUIRE(easy::type_list_size_v<type_list_2> == 3);
        STATIC_REQUIRE(std::is_same_v<
            easy::type_list_element_t<0, type_list_2>, bool>);
        STATIC_REQUIRE(std::is_same_v<
            easy::type_list_element_t<1, type_list_2>, float>);
        STATIC_REQUIRE(std::is_same_v<
            easy::type_list_element_t<2, type_list_2>, int>);

        // prepend multiple types
        using type_list_3 = easy::prepend_type<type_list_1, char, bool>;
        
        STATIC_REQUIRE(easy::type_list_size_v<type_list_3> == 4);
        STATIC_REQUIRE(std::is_same_v<
            easy::type_list_element_t<0, type_list_3>, char>);
        STATIC_REQUIRE(std::is_same_v<
            easy::type_list_element_t<1, type_list_3>, bool>);
        STATIC_REQUIRE(std::is_same_v<
            easy::type_list_element_t<2, type_list_3>, float>);
        STATIC_REQUIRE(std::is_same_v<
            easy::type_list_element_t<3, type_list_3>, int>);
    }
}
#endif