#include <doctest/doctest.h>
#include "caff/variable.h"
#include "caff/stdexcept.h"
#include "caff/format.h"

TEST_SUITE("caff::variable")
{
    using caff::variable;
    using caff::invalid_operation;

    TEST_CASE("constructors")
    {
        SUBCASE("copy constructor")
        {
            SUBCASE("explicit int")
            {
                int value = 12;
                variable<int> var{ "name", value };
                REQUIRE(var.name == "name");
                REQUIRE(var.value == value);
                static_assert(std::is_same_v<decltype(var)::type, int>);
            }

            SUBCASE("deduced value reference")
            {
                int value = 12;
                variable var{ "name", value };
                REQUIRE(var.name == "name");
                REQUIRE(var.value == value);
                static_assert(std::is_same_v<decltype(var)::type, int&>);
            }

            SUBCASE("deduced const value reference")
            {
                const int value = 12;
                variable var{ "name", value };
                REQUIRE(var.name == "name");
                REQUIRE(var.value == value);
                static_assert(std::is_same_v<decltype(var)::type,
                    const int&>);
            }

            SUBCASE("deduced value via temporary")
            {
                variable var{ "name", 12 };
                REQUIRE(var.name == "name");
                REQUIRE(var.value == 12);
                static_assert(std::is_same_v<decltype(var)::type, int>);
            }

            SUBCASE("deduced char const* -> std::string")
            {
                char const* value = "value";
                variable var{ "name", value };
                REQUIRE(var.name == "name");
                REQUIRE(var.value == value);
                static_assert(std::is_same_v<decltype(var)::type, std::string>);
            }
        }

        SUBCASE("move constructor")
        {
            variable variable{ "name", 12 };
            REQUIRE(variable.name == "name");
            REQUIRE(variable.value == 12);
            static_assert(std::is_same_v<decltype(variable)::type, int>);
        }
    }

    TEST_CASE("name")
    {
        variable var{ "name", 12 };
        REQUIRE(var.name == "name");
    }

    TEST_CASE("value")
    {
        variable var{ "name", 12 };
        REQUIRE(var.value == 12);
    }

    TEST_CASE("bitwise operator")
    {
        variable var{ "name" , 12 };

        invalid_operation e{ "Custom Message" };
        e << var;

        const auto exception_message = fmt::format(
            "Custom Message\n[ diagnostic info ]\nvariable: {}\nvalue: {}",
            var.name, var.value);
        REQUIRE(e.what() == exception_message);
    }

    TEST_CASE("make_variable")
    {
        const auto variable = caff::detail::make_variable("name", 12);
        REQUIRE(variable.name == "name");
        REQUIRE(variable.value == 12);
    }

    TEST_CASE("make_varname")
    {
        const auto varname = caff::detail::make_varname("name", "");
        REQUIRE(std::string{ varname } == "name");
    }

    TEST_CASE("caff_variable")
    {
        const int name = 12;
        const auto variable = caff_variable(name);
        REQUIRE(variable.name == "name");
        REQUIRE(variable.value == 12);
    }

    TEST_CASE("caff_variable_ex")
    {
        const int name = 12;
        const int value = 13;
        const auto variable = caff_variable_ex(name, value);
        REQUIRE(variable.name == "name");
        REQUIRE(variable.value == value);
    }

    TEST_CASE("caff_varname")
    {
        const int name = 12;
        const auto varname = caff_varname(name);
        REQUIRE(std::string{ varname } == "name");
    }
}
