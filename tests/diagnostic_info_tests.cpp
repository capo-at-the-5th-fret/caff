#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include "caff/diagnostic_info.h"

#include <type_traits>
#include <vector>
#include <array>
#include <tuple>
#include <caff/format.h>
#include <caff/test/type_list.h>
#include <caff/test/dummy.h>

TEST_CASE("diagnostic_info constructors", "[diagnostic_info]")
{
    SECTION("explicit int")
    {
        caff::diagnostic_info<int> info{ "name", short{ 12 } };
        static_assert(std::is_same_v<decltype(info)::value_type, int>);
        CHECK(info.name() == "name");
        CHECK(info.value() == 12);
    }

    SECTION("deduction for object types")
    {
        SECTION("integral")
        {
            SECTION("bool")
            {
                caff::diagnostic_info info{ "name", true };
                static_assert(std::is_same_v<decltype(info)::value_type, bool>);
                CHECK(info.name() == "name");
                CHECK(info.value() == true);
            }

            SECTION("char")
            {
                caff::diagnostic_info info{ "name", 'b' };
                static_assert(std::is_same_v<decltype(info)::value_type, char>);
                CHECK(info.name() == "name");
                CHECK(info.value() == 'b');
            }

            SECTION("short")
            {
                short s = 12;
                caff::diagnostic_info info{ "name", s };
                static_assert(std::is_same_v<decltype(info)::value_type, short>);
                CHECK(info.name() == "name");
                CHECK(info.value() == s);
            }

            SECTION("unsigned long")
            {
                caff::diagnostic_info info{ "name", 1000ul };
                static_assert(std::is_same_v<decltype(info)::value_type, unsigned long>);
                CHECK(info.name() == "name");
                CHECK(info.value() == 1000ul);
            }
        }

        SECTION("floating point")
        {
            double d = 12.0;
            caff::diagnostic_info info{ "name", d };
            static_assert(std::is_same_v<decltype(info)::value_type, double>);
            CHECK(info.name() == "name");
            CHECK(info.value() == d);
        }

        SECTION("enum")
        {
            caff::test::dummy_enum e = caff::test::dummy_enum::one;
            caff::diagnostic_info info{ "name", e };
            static_assert(std::is_same_v<decltype(info)::value_type, caff::test::dummy_enum>);
            CHECK(info.name() == "name");
            CHECK(info.value() == caff::test::dummy_enum::one);
        }

        SECTION("pointer")
        {
            int x = 12;
            int* p = &x;
            caff::diagnostic_info info{ "name", p };
            static_assert(std::is_same_v<decltype(info)::value_type, int*>);
            CHECK(info.name() == "name");
            CHECK(info.value() == p);
        }

        SECTION("member pointer")
        {
            caff::test::dummy_class d{ 12 };

            SECTION("member variable pointer")
            {
                using mem_var_ptr_t = int caff::test::dummy_class::*;
            
                caff::diagnostic_info info{ "name", &caff::test::dummy_class::member_variable };
                static_assert(std::is_same_v<decltype(info)::value_type, mem_var_ptr_t>);
                CHECK(info.name() == "name");
                CHECK(info.value() == &caff::test::dummy_class::member_variable);
            }

            SECTION("member function pointer")
            {
                using mem_fun_ptr_t = void (caff::test::dummy_class::*)();
            
                caff::diagnostic_info info{ "name", &caff::test::dummy_class::member_function };
                static_assert(std::is_same_v<decltype(info)::value_type, mem_fun_ptr_t>);
                CHECK(info.name() == "name");
                CHECK(info.value() == &caff::test::dummy_class::member_function);
            }
        }

        SECTION("nullptr_t")
        {
            caff::diagnostic_info info{ "name", nullptr };
            static_assert(std::is_same_v<decltype(info)::value_type, std::nullptr_t>);
            CHECK(info.name() == "name");
            CHECK(info.value() == nullptr);
        }

        SECTION("array")
        {
            SECTION("unbounded array")
            {
                int a[] = { 1, 2, 3 };
                caff::diagnostic_info info{ "name", a };
                static_assert(std::is_same_v<decltype(info)::value_type, std::array<int,3>>);
                CHECK(info.name() == "name");
                CHECK(info.value() == std::array<int,3>{ 1, 2, 3 });
            }

            SECTION("bounded array")
            {
                int a[3] = { 1, 2, 3 };
                caff::diagnostic_info info{ "name", a };
                static_assert(std::is_same_v<decltype(info)::value_type, std::array<int,3>>);
                CHECK(info.name() == "name");
                CHECK(info.value() == std::array<int,3>{ 1, 2, 3 });
            }

            SECTION("std::array")
            {
                std::array a{ 1, 2, 3 };
                caff::diagnostic_info info{ "name", a };
                static_assert(std::is_same_v<decltype(info)::value_type, std::array<int, 3>>);
                CHECK(info.name() == "name");
                CHECK(info.value() == std::array<int,3>{ 1, 2, 3 });
            }
        }

        SECTION("union")
        {
            caff::test::dummy_union u{ 12 };
            caff::diagnostic_info info{ "name", u };
            static_assert(std::is_same_v<decltype(info)::value_type, caff::test::dummy_union>);
            CHECK(info.name() == "name");
            CHECK(info.value().i == 12);
        }

        SECTION("class")
        {
            caff::test::dummy_class d{ 12 };
            caff::diagnostic_info info{ "name", d };
            static_assert(std::is_same_v<decltype(info)::value_type, caff::test::dummy_class>);
            CHECK(info.name() == "name");
            CHECK(info.value() == d);
        }
    }

    SECTION("special deduction guide types")
    {
        SECTION("string literal -> std::string")
        {
            caff::diagnostic_info info{ "name", "value" };
            static_assert(std::is_same_v<decltype(info)::value_type, std::string>);
            CHECK(info.name() == "name");
            CHECK(info.value() == "value");
        }

        SECTION("std::string_view -> std::string")
        {
            caff::diagnostic_info info{ "name", std::string_view{ "value" } };
            static_assert(std::is_same_v<decltype(info)::value_type, std::string>);
            CHECK(info.name() == "name");
            CHECK(info.value() == "value");
        }

        SECTION("c-style array -> std::array<T, N>")
        {
            SECTION("mutable array")
            {
                int value[] = { 1, 2, 3 };
                caff::diagnostic_info info{ "name", value };
                static_assert(std::is_same_v<decltype(info)::value_type, std::array<int, 3>>);
                CHECK(info.name() == "name");
                CHECK(info.value() == std::array<int,3>{ 1, 2, 3 });
            }

            SECTION("const array")
            {
                const int value[] = { 1, 2, 3 };
                caff::diagnostic_info info{ "name", value };
                static_assert(std::is_same_v<decltype(info)::value_type, std::array<int, 3>>);
                CHECK(info.name() == "name");
                CHECK(info.value() == std::array<int,3>{ 1, 2, 3 });
            }
        }
    }
}

TEST_CASE("diagnostic_info name", "[diagnostic_info]")
{
    caff::diagnostic_info<int> diagnostic_info{ "name", 12 };
    CHECK(diagnostic_info.name() == "name");
}

TEST_CASE("diagnostic_info value", "[diagnostic_info]")
{
    caff::diagnostic_info<int> diagnostic_info{ "name", 12 };
    CHECK(diagnostic_info.value() == 12);
}

TEST_CASE("diagnostic_info to_string", "[diagnostic_info]")
{
    caff::diagnostic_info<int> diagnostic_info{ "name", 12 };
    CHECK(to_string(diagnostic_info) == "name: 12");
}

TEST_CASE("make_diagnostic_info", "[diagnostic_info]")
{
    auto diagnostic_info = caff::make_diagnostic_info("name", 12);
    CHECK(diagnostic_info.name() == "name");
    CHECK(diagnostic_info.value() == 12);
}

TEST_CASE("format_diagnostic_info", "[diagnostic_info]")
{
    SECTION("no arguments")
    {
        auto diagnostic_info = caff::format_diagnostic_info("name",
            ">= 12");
        CHECK(diagnostic_info.name() == "name");
        CHECK(diagnostic_info.value() == ">= 12");
    }

    SECTION("single argument")
    {
        auto diagnostic_info = caff::format_diagnostic_info("name", ">= {}",
            12);
        CHECK(diagnostic_info.name() == "name");
        CHECK(diagnostic_info.value() == ">= 12");
    }

    SECTION("multiple arguments")
    {
        auto diagnostic_info = caff::format_diagnostic_info("name",
            "[ {}, {}, {} ]", "red", "green", "blue");
        CHECK(diagnostic_info.name() == "name");
        CHECK(diagnostic_info.value() == "[ red, green, blue ]");
    }

    SECTION("illegal format")
    {
        auto diagnostic_info = caff::format_diagnostic_info("name",
            "{", 10);
        CHECK(diagnostic_info.name() == "name");
        CHECK(diagnostic_info.value() == "unknown");
    }
}

TEST_CASE("make_location_info", "[diagnostic_info]")
{
    const auto location = caff::source_location::current();
    const auto expected_value = fmt::format("{}({})",
        location.file_name(),
        location.line());
    auto diagnostic_info = caff::make_location_info(location);
    CHECK(diagnostic_info.name() == "location");
    CHECK(diagnostic_info.value() == expected_value);
}

TEST_CASE("make_variable_info", "[diagnostic_info]")
{
    auto diagnostic_info = caff::make_variable_info("var");
    CHECK(diagnostic_info.name() == "variable");
    CHECK(diagnostic_info.value() == "var");
}

TEST_CASE("make_value_info", "[diagnostic_info]")
{
    auto diagnostic_info = caff::make_value_info(12);
    CHECK(diagnostic_info.name() == "value");
    CHECK(diagnostic_info.value() == 12);
}

TEST_CASE("make_index_info", "[diagnostic_info]")
{
    auto diagnostic_info = caff::make_index_info(12);
    CHECK(diagnostic_info.name() == "index");
    CHECK(diagnostic_info.value() == 12);
}

TEST_CASE("make_null_value_info", "[diagnostic_info]")
{
    auto diagnostic_info = caff::make_null_value_info();
    CHECK(diagnostic_info.name() == "value");
    CHECK(diagnostic_info.value() == "null");
}

TEST_CASE("make_message_info", "[diagnostic_info]")
{
    auto diagnostic_info = caff::make_message_info("the message");
    CHECK(diagnostic_info.name() == "message");
    CHECK(diagnostic_info.value() == "the message");
}

TEST_CASE("make_file_path_info", "[diagnostic_info]")
{
    auto diagnostic_info = caff::make_file_path_info("filename.txt");
    CHECK(diagnostic_info.name() == "file_path");
    CHECK(diagnostic_info.value() == "filename.txt");
}

TEST_CASE("make_condition_info", "[diagnostic_info]")
{
    const auto diagnostic_info = caff::make_condition_info(">= 12");
    CHECK(diagnostic_info.name() == "condition");
    CHECK(diagnostic_info.value() == ">= 12");
}

TEST_CASE("format_condition_info", "[diagnostic_info]")
{
    SECTION("no arguments")
    {
        const auto diagnostic_info = caff::format_condition_info(">= 10");
        CHECK(diagnostic_info.name() == "condition");
        CHECK(diagnostic_info.value() == ">= 10");
    }

    SECTION("single argument")
    {
        const auto diagnostic_info =
            caff::format_condition_info(">= {}", 12);
        CHECK(diagnostic_info.name() == "condition");
        CHECK(diagnostic_info.value() == ">= 12");
    }

    SECTION("multiple arguments")
    {
        const auto diagnostic_info =
            caff::format_condition_info("[ {}, {}, {} ]", "red", "green",
                "blue");
        CHECK(diagnostic_info.name() == "condition");
        CHECK(diagnostic_info.value() == "[ red, green, blue ]");
    }

    SECTION("illegal format")
    {
        auto diagnostic_info = caff::format_diagnostic_info("name",
            "{", 10);
        CHECK(diagnostic_info.name() == "name");
        CHECK(diagnostic_info.value() == "unknown");
    }

    SECTION("illegal format")
    {
        const auto diagnostic_info = caff::format_condition_info("{", 10);
        CHECK(diagnostic_info.name() == "condition");
        CHECK(diagnostic_info.value() == "unknown");
    }
}

TEST_CASE("make_equal_to_condition", "[diagnostic_info]")
{
    const auto diagnostic_info = caff::make_equal_to_condition(12);
    CHECK(diagnostic_info.name() == "condition");
    CHECK(diagnostic_info.value() == "== 12");
}

TEST_CASE("make_not_equal_to_condition", "[diagnostic_info]")
{
    const auto diagnostic_info = caff::make_not_equal_to_condition(12);
    CHECK(diagnostic_info.name() == "condition");
    CHECK(diagnostic_info.value() == "!= 12");
}

TEST_CASE("make_less_condition", "[diagnostic_info]")
{
    const auto diagnostic_info = caff::make_less_condition(12);
    CHECK(diagnostic_info.name() == "condition");
    CHECK(diagnostic_info.value() == "< 12");
}

TEST_CASE("make_greater_condition", "[diagnostic_info]")
{
    const auto diagnostic_info = caff::make_greater_condition(12);
    CHECK(diagnostic_info.name() == "condition");
    CHECK(diagnostic_info.value() == "> 12");
}

TEST_CASE("make_less_equal_condition", "[diagnostic_info]")
{
    const auto diagnostic_info = caff::make_less_equal_condition(12);
    CHECK(diagnostic_info.name() == "condition");
    CHECK(diagnostic_info.value() == "<= 12");
}

TEST_CASE("make_greater_equal_condition", "[diagnostic_info]")
{
    const auto diagnostic_info = caff::make_greater_equal_condition(12);
    CHECK(diagnostic_info.name() == "condition");
    CHECK(diagnostic_info.value() == ">= 12");
}

TEST_CASE("make_range_info", "[diagnostic_info]")
{
    const auto [rangeType, expectedText] = GENERATE(
        table<caff::range_type, std::string>(
    {
        { caff::range_type::closed, ">= 0 && <= 4" },
        { caff::range_type::left_open, "> 0 && <= 4" },
        { caff::range_type::right_open, ">= 0 && < 4" },
        { caff::range_type::open, "> 0 && < 4" }
    }));

    CAPTURE(rangeType, expectedText);

    SECTION("variable range")
    {
        const auto diagnostic_info = caff::make_range_info(0, 4, rangeType);

        CHECK(diagnostic_info.name() == "condition");
        CHECK(diagnostic_info.value() == expectedText);
    }

    SECTION("sized range")
    {
        const std::vector<int> testArray = { 1, 2, 3, 4 };

        const auto diagnostic_info = caff::make_sized_range_info(testArray,
            rangeType);
        CHECK(diagnostic_info.name() == "condition");
        CHECK(diagnostic_info.value() == expectedText);
    }
}

TEST_CASE("make_closed_range_info", "[diagnostic_info]")
{
    const auto diagnostic_info =
        caff::make_closed_range_info(-12,12);
    CHECK(diagnostic_info.name() == "condition");
    CHECK(diagnostic_info.value() == ">= -12 && <= 12");
}

TEST_CASE("make_half_open_range_info", "[diagnostic_info]")
{
    const auto diagnostic_info =
        caff::make_half_open_range_info(-12,12);
    CHECK(diagnostic_info.name() == "condition");
    CHECK(diagnostic_info.value() == ">= -12 && < 12");
}

TEST_CASE("make_set_condition", "[diagnostic_info]")
{
    // default string formatter
    {
        std::vector<int> v;
        const auto diagnostic_info = caff::make_set_condition(v);
        CHECK(diagnostic_info.name() == "condition");
        CHECK(diagnostic_info.value() == "{ }");
    }

    {
        std::vector<int> v{ 21 };
        const auto diagnostic_info = caff::make_set_condition(v);
        CHECK(diagnostic_info.name() == "condition");
        CHECK(diagnostic_info.value() == "{ 21 }");
    }

    {
        std::vector<int> v{ 1, 2, 4, 3 };
        const auto diagnostic_info = caff::make_set_condition(v);
        CHECK(diagnostic_info.name() == "condition");
        CHECK(diagnostic_info.value() == "{ 1, 2, 4, 3 }");
    }

    // custom string formatter
    caff::string_formatter custom{ "[{}]" };
    {
        std::vector<int> v;
        const auto diagnostic_info = caff::make_set_condition(v, custom);
        CHECK(diagnostic_info.name() == "condition");
        CHECK(diagnostic_info.value() == "{ }");
    }

    {
        std::vector<int> v{ 21 };
        const auto diagnostic_info = caff::make_set_condition(v, custom);
        CHECK(diagnostic_info.name() == "condition");
        CHECK(diagnostic_info.value() == "{ [21] }");
    }

    {
        std::vector<int> v{ 1, 2, 4, 3 };
        const auto diagnostic_info = caff::make_set_condition(v, custom);
        CHECK(diagnostic_info.name() == "condition");
        CHECK(diagnostic_info.value() == "{ [1], [2], [4], [3] }");
    }
}

// doctest code
#if 0
TEST_SUITE("caff::diagnostic_info")
{
    TEST_CASE("constructors")
    {
        SUBCASE("explicit int")
        {
            caff::diagnostic_info<int> info{ "name", 12 };
            REQUIRE(info.name == "name");
            REQUIRE(info.value == 12);
            REQUIRE(std::is_same_v<decltype(info.value), int>);
        }

        SUBCASE("deduced int")
        {
            caff::diagnostic_info info{ "name", 12 };
            REQUIRE(info.name == "name");
            REQUIRE(info.value == 12);
            REQUIRE(std::is_same_v<decltype(info.value), int>);
        }

        SUBCASE("deduced const char* -> std::string")
        {
            caff::diagnostic_info info{ "name", "value" };
            REQUIRE(info.name == "name");
            REQUIRE(info.value == "value");
            REQUIRE(std::is_same_v<decltype(info.value), std::string>);
        }
    }

    TEST_CASE("name")
    {
        caff::diagnostic_info<int> diagnostic_info{ "name", 12 };
        REQUIRE(diagnostic_info.name == "name");
    }

    TEST_CASE("value")
    {
        caff::diagnostic_info<int> diagnostic_info{ "name", 12 };
        REQUIRE(diagnostic_info.value == 12);
    }

    TEST_CASE("make_diagnostic_info")
    {
        auto diagnostic_info = caff::make_diagnostic_info("name", 12);
        REQUIRE(diagnostic_info.name == "name");
        REQUIRE(diagnostic_info.value == 12);
    }

    TEST_CASE("format_diagnostic_info")
    {
        SUBCASE("no arguments")
        {
            auto diagnostic_info = caff::format_diagnostic_info("name",
                ">= 12");
            REQUIRE(diagnostic_info.name == "name");
            REQUIRE(diagnostic_info.value == ">= 12");
        }

        SUBCASE("single argument")
        {
            auto diagnostic_info = caff::format_diagnostic_info("name", ">= {}",
                12);
            REQUIRE(diagnostic_info.name == "name");
            REQUIRE(diagnostic_info.value == ">= 12");
        }

        SUBCASE("multiple arguments")
        {
            auto diagnostic_info = caff::format_diagnostic_info("name",
                "[ {}, {}, {} ]", "red", "green", "blue");
            REQUIRE(diagnostic_info.name == "name");
            REQUIRE(diagnostic_info.value == "[ red, green, blue ]");
        }

        SUBCASE("illegal format")
        {
            auto diagnostic_info = caff::format_diagnostic_info("name",
                "{", 10);
            REQUIRE(diagnostic_info.name == "name");
            REQUIRE(diagnostic_info.value == "unknown");
        }
    }

    TEST_CASE("make_location_info")
    {
        const auto location = caff::source_location::current();
        const auto expected_value = fmt::format("{}({})",
            location.file_name(),
            location.line());
        auto diagnostic_info = caff::make_location_info(location);
        REQUIRE(diagnostic_info.name == "location");
        REQUIRE(diagnostic_info.value == expected_value);
    }

    TEST_CASE("make_variable_info")
    {
        auto diagnostic_info = caff::make_variable_info("var");
        REQUIRE(diagnostic_info.name == "variable");
        REQUIRE(diagnostic_info.value == "var");
    }

    TEST_CASE("make_value_info")
    {
        auto diagnostic_info = caff::make_value_info(12);
        REQUIRE(diagnostic_info.name == "value");
        REQUIRE(diagnostic_info.value == 12);
    }

    TEST_CASE("make_index_info")
    {
        auto diagnostic_info = caff::make_index_info(12);
        REQUIRE(diagnostic_info.name == "index");
        REQUIRE(diagnostic_info.value == 12);
    }

    TEST_CASE("make_null_value_info")
    {
        auto diagnostic_info = caff::make_null_value_info();
        REQUIRE(diagnostic_info.name == "value");
        REQUIRE(diagnostic_info.value == "null");
    }

    TEST_CASE("make_message_info")
    {
        auto diagnostic_info = caff::make_message_info("the message");
        REQUIRE(diagnostic_info.name == "message");
        REQUIRE(diagnostic_info.value == "the message");
    }

    TEST_CASE("make_file_path_info")
    {
        auto diagnostic_info = caff::make_file_path_info("filename.txt");
        REQUIRE(diagnostic_info.name == "file_path");
        REQUIRE(diagnostic_info.value == "filename.txt");
    }

    TEST_CASE("make_condition_info")
    {
        const auto diagnostic_info = caff::make_condition_info(">= 12");
        REQUIRE(diagnostic_info.name == "condition");
        REQUIRE(diagnostic_info.value == ">= 12");
    }

    TEST_CASE("format_condition_info")
    {
        SUBCASE("no arguments")
        {
            const auto diagnostic_info = caff::format_condition_info(">= 10");
            REQUIRE(diagnostic_info.name == "condition");
            REQUIRE(diagnostic_info.value == ">= 10");
        }

        SUBCASE("single argument")
        {
            const auto diagnostic_info =
                caff::format_condition_info(">= {}", 12);
            REQUIRE(diagnostic_info.name == "condition");
            REQUIRE(diagnostic_info.value == ">= 12");
        }

        SUBCASE("multiple arguments")
        {
            const auto diagnostic_info =
                caff::format_condition_info("[ {}, {}, {} ]", "red", "green",
                    "blue");
            REQUIRE(diagnostic_info.name == "condition");
            REQUIRE(diagnostic_info.value == "[ red, green, blue ]");
        }

        SUBCASE("illegal format")
        {
            auto diagnostic_info = caff::format_diagnostic_info("name",
                "{", 10);
            REQUIRE(diagnostic_info.name == "name");
            REQUIRE(diagnostic_info.value == "unknown");
        }

        SUBCASE("illegal format")
        {
            const auto diagnostic_info = caff::format_condition_info("{", 10);
            REQUIRE(diagnostic_info.name == "condition");
            REQUIRE(diagnostic_info.value == "unknown");
        }
    }

    TEST_CASE("make_equal_to_condition")
    {
        const auto diagnostic_info = caff::make_equal_to_condition(12);
        REQUIRE(diagnostic_info.name == "condition");
        REQUIRE(diagnostic_info.value == "== 12");
    }

    TEST_CASE("make_not_equal_to_condition")
    {
        const auto diagnostic_info = caff::make_not_equal_to_condition(12);
        REQUIRE(diagnostic_info.name == "condition");
        REQUIRE(diagnostic_info.value == "!= 12");
    }

    TEST_CASE("make_less_condition")
    {
        const auto diagnostic_info = caff::make_less_condition(12);
        REQUIRE(diagnostic_info.name == "condition");
        REQUIRE(diagnostic_info.value == "< 12");
    }

    TEST_CASE("make_greater_condition")
    {
        const auto diagnostic_info = caff::make_greater_condition(12);
        REQUIRE(diagnostic_info.name == "condition");
        REQUIRE(diagnostic_info.value == "> 12");
    }

    TEST_CASE("make_less_equal_condition")
    {
        const auto diagnostic_info = caff::make_less_equal_condition(12);
        REQUIRE(diagnostic_info.name == "condition");
        REQUIRE(diagnostic_info.value == "<= 12");
    }

    TEST_CASE("make_greater_equal_condition")
    {
        const auto diagnostic_info = caff::make_greater_equal_condition(12);
        REQUIRE(diagnostic_info.name == "condition");
        REQUIRE(diagnostic_info.value == ">= 12");
    }

    TEST_CASE("make_range_info")
    {
        using namespace std::string_literals;

        const std::array test_values =
        {
            std::tuple{ caff::range_type::closed, ">= 0 && <= 4"s },
            std::tuple{ caff::range_type::left_open, "> 0 && <= 4"s },
            std::tuple{ caff::range_type::right_open, ">= 0 && < 4"s },
            std::tuple{ caff::range_type::open, "> 0 && < 4"s }
        };

        for (int i = 0; const auto [rangeType, expectedText] : test_values)
        {
            CAPTURE(i);

            SUBCASE("variable range")
            {
                const auto diagnostic_info = caff::make_range_info(0, 4, rangeType);

                REQUIRE(diagnostic_info.name == "condition");
                REQUIRE(diagnostic_info.value == expectedText);
            }

            SUBCASE("sized range")
            {
                const std::vector<int> testArray = { 1, 2, 3, 4 };

                const auto diagnostic_info = caff::make_sized_range_info(testArray,
                    rangeType);
                REQUIRE(diagnostic_info.name == "condition");
                REQUIRE(diagnostic_info.value == expectedText);
            }

            ++i;
        }
    }

    TEST_CASE("make_closed_range_info")
    {
        const auto diagnostic_info =
            caff::make_closed_range_info(-12,12);
        REQUIRE(diagnostic_info.name == "condition");
        REQUIRE(diagnostic_info.value == ">= -12 && <= 12");
    }

    TEST_CASE("make_half_open_range_info")
    {
        const auto diagnostic_info =
            caff::make_half_open_range_info(-12,12);
        REQUIRE(diagnostic_info.name == "condition");
        REQUIRE(diagnostic_info.value == ">= -12 && < 12");
    }

    TEST_CASE("make_set_condition")
    {
        // default string formatter
        {
            std::vector<int> v;
            const auto diagnostic_info = caff::make_set_condition(v);
            REQUIRE(diagnostic_info.name == "condition");
            REQUIRE(diagnostic_info.value == "{ }");
        }

        {
            std::vector<int> v{ 21 };
            const auto diagnostic_info = caff::make_set_condition(v);
            REQUIRE(diagnostic_info.name == "condition");
            REQUIRE(diagnostic_info.value == "{ 21 }");
        }

        {
            std::vector<int> v{ 1, 2, 4, 3 };
            const auto diagnostic_info = caff::make_set_condition(v);
            REQUIRE(diagnostic_info.name == "condition");
            REQUIRE(diagnostic_info.value == "{ 1, 2, 4, 3 }");
        }

        // custom string formatter
        caff::string_formatter custom{ "[{}]" };
        {
            std::vector<int> v;
            const auto diagnostic_info = caff::make_set_condition(v, custom);
            REQUIRE(diagnostic_info.name == "condition");
            REQUIRE(diagnostic_info.value == "{ }");
        }

        {
            std::vector<int> v{ 21 };
            const auto diagnostic_info = caff::make_set_condition(v, custom);
            REQUIRE(diagnostic_info.name == "condition");
            REQUIRE(diagnostic_info.value == "{ [21] }");
        }

        {
            std::vector<int> v{ 1, 2, 4, 3 };
            const auto diagnostic_info = caff::make_set_condition(v, custom);
            REQUIRE(diagnostic_info.name == "condition");
            REQUIRE(diagnostic_info.value == "{ [1], [2], [4], [3] }");
        }
    }
}

#endif
