#include <doctest/doctest.h>
#include "caff/diagnostic_info.h"

#include <type_traits>
#include <vector>
#include <array>
#include <tuple>
#include <caff/format.h>

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
