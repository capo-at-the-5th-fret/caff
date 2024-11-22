#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include "caff/tuple.h"

// NOTE: These four std includes are needed for tuple_like test
#include <ranges>
#include <list>
#include <iterator>
#include <array>
#include <utility>
#include "caff/type_traits.h"
#include "caff/test/type_list.h"

#if 0
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

TEMPLATE_LIST_TEST_CASE("has_tuple_element", "[tuple]", tuple_like_types)
{
    using TupleLike = TestType;

    std::size_t i{ 0 };
    caff::tuple_for_each_index<TupleLike>([&i]<std::size_t I>
    {
        CAPTURE(I);
        CAPTURE(i);
        CHECK(I == i);
        static_assert(caff::detail::has_tuple_element<TupleLike, I>);
        ++i;
    });

    // Non tuple-like types
    static_assert(!caff::detail::has_tuple_element<int, 0>);

    // NOTE: Won't compile
    // Out of range index
    //static_assert(!caff::has_tuple_element<TestType,
      //  std::tuple_size_v<TestType>>);
}

TEMPLATE_LIST_TEST_CASE("tuple_like std types", "[tuple]", tuple_like_types)
{
    using qts_t = caff::cv_qualified_set_t<TestType>;

    caff::tuple_for_each_type<qts_t>([]<typename T>
    {
        static_assert(caff::tuple_like<T>);
        static_assert(!caff::tuple_like<T&>);
    });

    CHECK(true);
}

TEMPLATE_LIST_TEST_CASE("tuple_like other types", "[tuple]",
    caff::test::cv_qualifiable_types)
{
    using qts_t = caff::cv_qualified_set_t<TestType>;

    caff::tuple_for_each_type<qts_t>([]<typename T>
    {
        static_assert(!caff::tuple_like<T>);
    });

    CHECK(true);
}

TEMPLATE_LIST_TEST_CASE("tuple_contains_type", "[tuple]", tuple_like_types)
{
    using tuple_t = TestType;
    using success_list = tuple_t;
    using failure_list = caff::tuple_append_t<
        caff::cv_qualified_set_t<void>, void*>;

    SECTION("successes")
    {
        caff::tuple_for_each_type<success_list>([]<typename T>
        {
            static_assert(caff::tuple_contains_type<T, tuple_t>::value);
            static_assert(caff::tuple_contains_type_v<T, tuple_t>);
        });
    }

    SECTION("failures")
    {
        caff::tuple_for_each_type<failure_list>([]<typename T>
        {
            static_assert(!caff::tuple_contains_type<T, tuple_t>::value);
            static_assert(!caff::tuple_contains_type_v<T, tuple_t>);
        });
    }

    CHECK(true);
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

TEMPLATE_LIST_TEST_CASE("tuple_cat_t", "[tuple]", tuple_cat_t_test_types)
{
    using TestTypes = TestType;
    using tuple_t_1 = std::tuple_element_t<0, TestTypes>;
    using tuple_t_2 = std::tuple_element_t<1, TestTypes>;
    using expected_type = std::tuple_element_t<2, TestTypes>;
    
    static_assert(std::is_same_v
    <
        caff::tuple_cat_t<tuple_t_1, tuple_t_2>,
        expected_type
    >);

    CHECK(true);
}

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
        return caff::tuple_append_t<Tuple, decltype(args)...>{};
    }, std::declval<Types>()));
}

TEMPLATE_LIST_TEST_CASE("tuple_append_t", "[tuple]", tuple_append_t_test_types)
{
    using TestTypes = TestType;
    using tuple_t = std::tuple_element_t<0, TestTypes>;
    using types_t = std::tuple_element_t<1, TestTypes>;
    using expected_type = std::tuple_element_t<2, TestTypes>;

    using t = tuple_append_t_applier<tuple_t, types_t>;
    static_assert(std::is_same_v<t, expected_type>);

    CHECK(true);
}

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
        return caff::tuple_prepend_t<Tuple, decltype(args)...>{};
    }, std::declval<Types>()));
}

TEMPLATE_LIST_TEST_CASE("tuple_prepend_t", "[tuple]", tuple_prepend_t_test_types)
{
    using TestTypes = TestType;
    using tuple_t = std::tuple_element_t<0, TestTypes>;
    using types_t = std::tuple_element_t<1, TestTypes>;
    using expected_type = std::tuple_element_t<2, TestTypes>;

    using t = tuple_prepend_t_applier<tuple_t, types_t>;
    static_assert(std::is_same_v<t, expected_type>);

    CHECK(true);
}

TEMPLATE_LIST_TEST_CASE("tuple_for_each_index", "[tuple]", tuple_like_types)
{
    using TupleLike = TestType;

    constexpr std::size_t expected_size = std::tuple_size_v<TupleLike>;

    std::size_t i{ 0 };
    caff::tuple_for_each_index<TupleLike>([&i]<std::size_t I>
    {
        CHECK(I == i);
        static_assert(I < expected_size);
        ++i;
    });

    CHECK(i == expected_size);
}

TEST_CASE("tuple_for_each_type")
{
    // NOTE: The REQUIREs here need to use a literal type as
    // std::tuple_element_t requires a compile time index which isn't available
    // in for_each_type

#ifdef __cpp_lib_ranges
    static_assert(std::tuple_size_v<tuple_like_types> == 7);
#else
    static_assert(std::tuple_size_v<tuple_like_types> == 6);
#endif

    caff::tuple_enumerate_types<tuple_like_types>(
        []<std::size_t I, typename TupleLike>()
    {
        using tuple_t = TupleLike;
        static_assert(std::tuple_size_v<tuple_t> == tuple_like_type_sizes[I]);

        std::size_t i{ 0 };
        caff::tuple_for_each_type<tuple_t>([&i]<typename T>
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

TEMPLATE_LIST_TEST_CASE("tuple_enumerate_types", "[tuple]", tuple_like_types)
{
    using TupleLike = TestType;

    std::size_t i{ 0 };
    caff::tuple_enumerate_types<TupleLike>([&i]<std::size_t I, typename T>
    {
        CAPTURE(I);
        CAPTURE(i);
        REQUIRE(i == I);
        static_assert(std::is_same_v<T, std::tuple_element_t<I,TupleLike>>);
        ++i;
    });

    REQUIRE(i == std::tuple_size_v<TupleLike>);
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

TEMPLATE_TEST_CASE("tuple_for_each", "[tuple]", tuple_test_type, pair_test_type,
    array_test_type)
{
    using fixture_t = tuple_fixture<TestType>;
    using tuple_t = typename fixture_t::tuple_type;

    fixture_t fixture;

    tuple_t t{ fixture.test_value };
    auto ct = std::as_const(fixture.test_value);

    SECTION("verify the element types")
    {
        int i = 0;
        caff::tuple_for_each(t, [&]<typename T>(T element)
        {
            CAPTURE(i);
            tuple_for_each_validator v;
            REQUIRE(v.validate_element(i, element));
            ++i;
        });
    }

    SECTION("modify the elements")
    {
        caff::tuple_for_each(t, [](auto& element)
        {
            ++element;
        });

        REQUIRE(t == fixture.modified_expected);
    }

    SECTION("verify const correctness")
    {
        int i = 0;
        caff::tuple_for_each(std::as_const(t), [&](const auto& element)
        {
            CAPTURE(i);
            tuple_for_each_validator v;
            REQUIRE(v.validate_const_correctness(i, element));
            ++i;
        });
    }
}


TEMPLATE_TEST_CASE("tuple_enumerate", "[tuple]", tuple_test_type, pair_test_type,
    array_test_type)
{
    using fixture_t = tuple_fixture<TestType>;
    using tuple_t = typename fixture_t::tuple_type;

    fixture_t fixture;

    tuple_t t{ fixture.test_value };

    SECTION("verify the indices and element types")
    {
        caff::tuple_enumerate(t, [&t = t]<auto I>(auto element)
        {
            CAPTURE(I);
            static_assert(std::is_same_v<std::tuple_element_t<I, tuple_t>,
                decltype(element)>);
            REQUIRE(std::get<I>(t) == element);
        });
    }

    SECTION("modify the elements")
    {
        caff::tuple_enumerate(t, []<auto I>(auto& element)
        {
            ++element;
        });

        REQUIRE(t == fixture.modified_expected );
    }

    SECTION("verify const correctness")
    {
        caff::tuple_enumerate(std::as_const(t), []<auto I>(const auto& element)
        {
            CAPTURE(I);
            static_assert(
                std::is_const_v<std::remove_reference_t<decltype(element)>>);
        });
    }
}
#endif
