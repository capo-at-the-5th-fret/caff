#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include "caff/functional.h"
#include <utility>
#include <unordered_set>
#include "caff/type_traits.h"
#include "caff/type_list.h"

namespace
{
    template <typename CharT>
    //requires char_type<CharT>
    struct basic_string_hash_fixture
    {
        const CharT* c_str = []()
        {
            if constexpr (std::is_same_v<CharT, char>)
            {
                return "c_str";
            }
            else if constexpr (std::is_same_v<CharT, wchar_t>)
            {
                return L"c_str";
            }
            else if constexpr (std::is_same_v<CharT, char8_t>)
            {
                return u8"c_str";
            }
            else if constexpr (std::is_same_v<CharT, char16_t>)
            {
                return u"c_str";
            }
            else if constexpr (std::is_same_v<CharT, char32_t>)
            {
                return U"c_str";
            }
            // else
            // {
            //     static_assert(caff::always_false_v<CharT>,
            //         "The type used for template parameter CharT is unsupported");
            // }
        }();

        const std::basic_string<CharT> str = []()
        {
            if constexpr (std::is_same_v<CharT, char>)
            {
                return "str";
            }
            else if constexpr (std::is_same_v<CharT, wchar_t>)
            {
                return L"str";
            }
            else if constexpr (std::is_same_v<CharT, char8_t>)
            {
                return u8"str";
            }
            else if constexpr (std::is_same_v<CharT, char16_t>)
            {
                return u"str";
            }
            else if constexpr (std::is_same_v<CharT, char32_t>)
            {
                return U"str";
            }
            // else
            // {
            //     static_assert(caff::always_false_v<CharT>,
            //         "The type used for template parameter CharT is unsupported");
            // }
        }();

        const std::basic_string_view<CharT> sv = []()
        {
            if constexpr (std::is_same_v<CharT, char>)
            {
                return "sv";
            }
            else if constexpr (std::is_same_v<CharT, wchar_t>)
            {
                return L"sv";
            }
            else if constexpr (std::is_same_v<CharT, char8_t>)
            {
                return u8"sv";
            }
            else if constexpr (std::is_same_v<CharT, char16_t>)
            {
                return u"sv";
            }
            else if constexpr (std::is_same_v<CharT, char32_t>)
            {
                return U"sv";
            }
            // else
            // {
            //     static_assert(caff::always_false_v<CharT>,
            //         "The type used for template parameter CharT is unsupported");
            // }
        }();
    };

}

TEMPLATE_LIST_TEST_CASE("basic_string_hash", "[functional]",
    caff::standard_character_types)
{
    using CharT = TestType;

    using namespace std::string_literals;
    using namespace std::string_view_literals;
    using test_set = std::unordered_set<
        std::basic_string<CharT>,
        caff::basic_string_hash<CharT>,
        std::equal_to<>
    >;

    basic_string_hash_fixture<CharT> fixture;

    SECTION("empty set")
    {
        test_set ts;

        SECTION("const CharT*")
        {
            auto pos = ts.find(fixture.c_str);
            CHECK((pos == ts.end()));
        }

        SECTION("basic_string")
        {
            auto pos = ts.find(fixture.str);
            CHECK((pos == ts.end()));
        }

        SECTION("basic_string_view")
        {
            auto pos = ts.find(fixture.sv);
            CHECK((pos == ts.end()));
        }
    }

    SECTION("populated set")
    {
        test_set ts;
        ts.insert(fixture.c_str);
        ts.insert(fixture.str);
        ts.insert(std::basic_string<CharT>{ fixture.sv });

        SECTION("const CharT*")
        {
            auto pos = ts.find(fixture.c_str);
            REQUIRE(pos != ts.end());
            CHECK(*pos == fixture.c_str);
        }

        SECTION("basic_string")
        {
            auto pos = ts.find(fixture.str);
            REQUIRE(pos != ts.end());
            CHECK(*pos == fixture.str);
        }

        SECTION("basic_string_view")
        {
            auto pos = ts.find(fixture.sv);
            REQUIRE(pos != ts.end());
            CHECK(*pos == std::basic_string<CharT>{ fixture.sv });
        }
    }
}

TEST_CASE("basic_string_hash aliases")
{
    // clang-format off
    static_assert(std::is_same_v
    <
        caff::string_hash::hash_type,
        std::hash<std::string_view>
    >);

    static_assert(std::is_same_v
    <
        caff::wstring_hash::hash_type,
        std::hash<std::wstring_view>
    >);

    static_assert(std::is_same_v
    <
        caff::u8string_hash::hash_type,
        std::hash<std::u8string_view>
    >);

    static_assert(std::is_same_v
    <
        caff::u16string_hash::hash_type,
        std::hash<std::u16string_view>
    >);

    static_assert(std::is_same_v
    <
        caff::u32string_hash::hash_type,
        std::hash<std::u32string_view>
    >);
    // clang-format on

    CHECK(true);
}

TEST_CASE("hash_combine")
{
    auto hash_value = std::hash<int>{}(12);

    SECTION("single value")
    {
        caff::hash_combine(hash_value, 2.5);
        CHECK(hash_value == caff::hash(12, 2.5));
    }
    
    SECTION("multi-value")
    {
        caff::hash_combine(hash_value, 2.5);
        caff::hash_combine(hash_value, true);
        CHECK(hash_value == caff::hash(12, 2.5, true));
    }
}

TEST_CASE("hash")
{
    auto hash_value = std::hash<int>{}(12);

    // single value should be the same as using std::hash
    SECTION("single value")
    {
        CHECK(caff::hash(12) == hash_value);
    }

    // multi-value should be the same as using hash_combine on types
    SECTION("multi-value")
    {
        caff::hash_combine(hash_value, 2.5);
        caff::hash_combine(hash_value, true);
        CHECK(caff::hash(12, 2.5, true) == hash_value);
    }
}
