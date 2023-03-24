#include <doctest/doctest.h>
#include "easy/functional.h"
#include <utility>
#include <unordered_set>
#include "easy/type_traits.h"
#include "easy/type_list.h"

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
            else
            {
                static_assert(easy::always_false_v<CharT>,
                    "The type used for template parameter CharT is unsupported");
            }
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
            else
            {
                static_assert(easy::always_false_v<CharT>,
                    "The type used for template parameter CharT is unsupported");
            }
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
            else
            {
                static_assert(easy::always_false_v<CharT>,
                    "The type used for template parameter CharT is unsupported");
            }
        }();
    };

}

TEST_CASE_TEMPLATE_DEFINE("basic_string_hash", CharT, basic_string_hash_test_id)
{
    using namespace std::string_literals;
    using namespace std::string_view_literals;
    using test_set = std::unordered_set<
        std::basic_string<CharT>,
        easy::basic_string_hash<CharT>,
        std::equal_to<>
    >;

    basic_string_hash_fixture<CharT> fixture;

    SUBCASE("empty set")
    {
        test_set ts;

        SUBCASE("const CharT*")
        {
            auto pos = ts.find(fixture.c_str);
            REQUIRE((pos == ts.end()));
        }

        SUBCASE("basic_string")
        {
            auto pos = ts.find(fixture.str);
            REQUIRE((pos == ts.end()));
        }

        SUBCASE("basic_string_view")
        {
            auto pos = ts.find(fixture.sv);
            REQUIRE((pos == ts.end()));
        }
    }

    SUBCASE("populated set")
    {
        test_set ts;
        ts.insert(fixture.c_str);
        ts.insert(fixture.str);
        ts.insert(std::basic_string<CharT>{ fixture.sv });

        SUBCASE("const CharT*")
        {
            auto pos = ts.find(fixture.c_str);
            REQUIRE(pos != ts.end());
            REQUIRE(*pos == fixture.c_str);
        }

        SUBCASE("basic_string")
        {
            auto pos = ts.find(fixture.str);
            REQUIRE(pos != ts.end());
            REQUIRE(*pos == fixture.str);
        }

        SUBCASE("basic_string_view")
        {
            auto pos = ts.find(fixture.sv);
            REQUIRE(pos != ts.end());
            REQUIRE(*pos == std::basic_string<CharT>{ fixture.sv });
        }
    }
}
TEST_CASE_TEMPLATE_APPLY(basic_string_hash_test_id,
    easy::standard_character_types);

TEST_CASE("basic_string_hash aliases")
{
    // clang-format off
    static_assert(std::is_same_v
    <
        easy::string_hash::hash_type,
        std::hash<std::string_view>
    >);

    static_assert(std::is_same_v
    <
        easy::wstring_hash::hash_type,
        std::hash<std::wstring_view>
    >);

    static_assert(std::is_same_v
    <
        easy::u8string_hash::hash_type,
        std::hash<std::u8string_view>
    >);

    static_assert(std::is_same_v
    <
        easy::u16string_hash::hash_type,
        std::hash<std::u16string_view>
    >);

    static_assert(std::is_same_v
    <
        easy::u32string_hash::hash_type,
        std::hash<std::u32string_view>
    >);
    // clang-format on
}

TEST_CASE("hash_combine")
{
    auto hash_value = std::hash<int>{}(12);

    SUBCASE("single value")
    {
        easy::hash_combine(hash_value, 2.5);
        REQUIRE(hash_value == easy::hash(12, 2.5));
    }
    
    SUBCASE("multi-value")
    {
        easy::hash_combine(hash_value, 2.5);
        easy::hash_combine(hash_value, true);
        REQUIRE(hash_value == easy::hash(12, 2.5, true));
    }
}

TEST_CASE("hash")
{
    auto hash_value = std::hash<int>{}(12);

    // single value should be the same as using std::hash
    SUBCASE("single value")
    {
        REQUIRE(easy::hash(12) == hash_value);
    }

    // multi-value should be the same as using hash_combine on types
    SUBCASE("multi-value")
    {
        easy::hash_combine(hash_value, 2.5);
        easy::hash_combine(hash_value, true);
        REQUIRE(easy::hash(12, 2.5, true) == hash_value);
    }
}
