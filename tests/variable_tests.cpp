#include <catch2/catch_test_macros.hpp>
#include "caff/variable.h"
#include "caff/stdexcept.h"
#include "caff/format.h"

using caff::variable;
using caff::invalid_operation;

TEST_CASE("variable constructor and type deduction")
{
    SECTION("Construct with lvalue reference")
    {
        int x = 42;
        variable v{"x", x};
        REQUIRE(v.name == "x");
        REQUIRE(v.value == 42);
        STATIC_REQUIRE(std::is_same_v<decltype(v), variable<int&>>);
    }

    SECTION("Construct with rvalue reference")
    {
        variable v{"y", 100};
        REQUIRE(v.name == "y");
        REQUIRE(v.value == 100);
        STATIC_REQUIRE(std::is_same_v<decltype(v), variable<int>>);
    }

    SECTION("Construct with const char* (deduced as std::string)")
    {
        variable v{"z", "Hello"};
        REQUIRE(v.name == "z");
        REQUIRE(v.value == "Hello");
        STATIC_REQUIRE(std::is_same_v<decltype(v), variable<std::string>>);
    }

    SECTION("Construct with std::string (rvalue)")
    {
        variable v{"a", std::string{"world"}};
        REQUIRE(v.name == "a");
        REQUIRE(v.value == "world");
        STATIC_REQUIRE(std::is_same_v<decltype(v), variable<std::string>>);
    }
}

TEST_CASE("detail::make_variable")
{
    using namespace caff::detail;

    SECTION("make_variable with lvalue")
    {
        int x = 42;
        auto v = make_variable("x", x);
        REQUIRE(v.name == "x");
        REQUIRE(v.value == 42);
        STATIC_REQUIRE(std::is_same_v<decltype(v), variable<int&>>);
    }

    SECTION("make_variable with rvalue")
    {
        auto v = make_variable("y", 99);
        REQUIRE(v.name == "y");
        REQUIRE(v.value == 99);
        STATIC_REQUIRE(std::is_same_v<decltype(v), variable<int>>);
    }
}

TEST_CASE("detail::make_varname")
{
    using namespace caff::detail;

    SECTION("Returns the variable name")
    {
        int x = 5;
        REQUIRE(make_varname("x", x) == "x");
    }
}

TEST_CASE("caff_variable macro")
{
    SECTION("Creates a variable with lvalue")
    {
        int x = 7;
        auto v = caff_variable(x);
        REQUIRE(v.name == "x");
        REQUIRE(v.value == 7);
        STATIC_REQUIRE(std::is_same_v<decltype(v), variable<int&>>);
    }

    SECTION("Creates a variable with rvalue")
    {
        auto v = caff_variable_ex(temp, 123);
        REQUIRE(v.name == "temp");
        REQUIRE(v.value == 123);
        STATIC_REQUIRE(std::is_same_v<decltype(v), variable<int>>);
    }
}

TEST_CASE("caff_varname macro")
{
    SECTION("Returns the variable name")
    {
        int my_var = 10;
        REQUIRE(caff_varname(my_var) == "my_var");
    }
}

TEST_CASE("caff::variable constructors", "[caff::variable]")
{
    SECTION("copy based constructor")
    {
        SECTION("explicit int")
        {
            short value = 12;
            variable<int> var{ "name", value };
            CHECK(var.name == "name");
            CHECK(var.value == value);
            static_assert(std::is_same_v<decltype(var)::type, int>);
        }

        SECTION("deduced value reference")
        {
            int value = 12;
            variable var{ "name", value };
            CHECK(var.name == "name");
            CHECK(var.value == value);
            static_assert(std::is_same_v<decltype(var)::type, int&>);
        }

        SECTION("deduced const value reference")
        {
            const int value = 12;
            variable var{ "name", value };
            CHECK(var.name == "name");
            CHECK(var.value == value);
            static_assert(std::is_same_v<decltype(var)::type, const int&>);
        }

        SECTION("deduced value via temporary")
        {
            variable var{ "name", 12 };
            CHECK(var.name == "name");
            CHECK(var.value == 12);
            static_assert(std::is_same_v<decltype(var)::type, int>);
        }

        SECTION("deduced char const* -> std::string")
        {
            char const* value = "value";
            variable var{ "name", value };
            CHECK(var.name == "name");
            CHECK(var.value == value);
            static_assert(std::is_same_v<decltype(var)::type, std::string>);
        }
    }

    SECTION("move based constructor")
    {
        variable variable{ "name", 12 };
        CHECK(variable.name == "name");
        CHECK(variable.value == 12);
        static_assert(std::is_same_v<decltype(variable)::type, int>);
    }
}

TEST_CASE("caff::variable name", "[caff::variable]")
{
    variable var{ "name", 12 };
    CHECK(var.name == "name");
}

TEST_CASE("caff::variable value", "[caff::variable]")
{
    variable var{ "name", 12 };
    CHECK(var.value == 12);
}

TEST_CASE("bitwise operator", "[caff::variable]")
{
    variable var{ "name" , 12 };

    invalid_operation e{ "Custom Message" };
    e << var;

    const auto exception_message = fmt::format(
        "Custom Message\n[ diagnostic info ]\nvariable: {}\nvalue: {}",
        var.name, var.value);
    CHECK(e.what() == exception_message);
}

TEST_CASE("make_variable", "[caff::variable]")
{
    const auto variable = caff::detail::make_variable("name", 12);
    CHECK(variable.name == "name");
    CHECK(variable.value == 12);
}

TEST_CASE("make_varname", "[caff::variable]")
{
    const auto varname = caff::detail::make_varname("name", "");
    CHECK(varname == "name");
}

TEST_CASE("caff_variable", "[caff::variable]")
{
    const int name = 12;
    const auto variable = caff_variable(name);
    CHECK(variable.name == "name");
    CHECK(variable.value == 12);
}

TEST_CASE("caff_variable_ex", "[caff::variable]")
{
    const int name = 12;
    const int value = 13;
    const auto variable = caff_variable_ex(name, value);
    CHECK(variable.name == "name");
    CHECK(variable.value == value);
}

TEST_CASE("caff_varname", "[caff::variable]")
{
    const int name = 12;
    auto varname = caff_varname(name);
    CHECK(varname == "name");
    STATIC_CHECK(std::is_same_v<decltype(varname), std::string_view>);
}

// doctest tests
#if 0
TEST_CASE("caff::variable constructors", "[variable]")
{
    SECTION("copy based constructors")
    {
        SECTION("explicit int")
        {
            int value = 12;
            variable<int> var{ "name", value };
            REQUIRE(var.name == "name");
            REQUIRE(var.value == value);
            static_assert(std::is_same_v<decltype(var)::type, int>);
        }

        SECTION("deduced value reference")
        {
            int value = 12;
            variable var{ "name", value };
            REQUIRE(var.name == "name");
            REQUIRE(var.value == value);
            static_assert(std::is_same_v<decltype(var)::type, int&>);
        }

        SECTION("deduced const value reference")
        {
            const int value = 12;
            variable var{ "name", value };
            REQUIRE(var.name == "name");
            REQUIRE(var.value == value);
            static_assert(std::is_same_v<decltype(var)::type,
                const int&>);
        }

        SECTION("deduced value via temporary")
        {
            variable var{ "name", 12 };
            REQUIRE(var.name == "name");
            REQUIRE(var.value == 12);
            static_assert(std::is_same_v<decltype(var)::type, int>);
        }

        SECTION("deduced char const* -> std::string")
        {
            char const* value = "value";
            variable var{ "name", value };
            REQUIRE(var.name == "name");
            REQUIRE(var.value == value);
            static_assert(std::is_same_v<decltype(var)::type, std::string>);
        }
    }

    SECTION("move based constructors")
    {
        variable variable{ "name", 12 };
        REQUIRE(variable.name == "name");
        REQUIRE(variable.value == 12);
        static_assert(std::is_same_v<decltype(variable)::type, int>);
    }
}

TEST_CASE("name", "[variable]")
{
    variable var{ "name", 12 };
    REQUIRE(var.name == "name");
}

TEST_CASE("value", "[variable]")
{
    variable var{ "name", 12 };
    REQUIRE(var.value == 12);
}

TEST_CASE("bitwise operator", "[variable]")
{
    variable var{ "name" , 12 };

    invalid_operation e{ "Custom Message" };
    e << var;

    const auto exception_message = fmt::format(
        "Custom Message\n[ diagnostic info ]\nvariable: {}\nvalue: {}",
        var.name, var.value);
    REQUIRE(e.what() == exception_message);
}

TEST_CASE("make_variable", "[variable]")
{
    const auto variable = caff::detail::make_variable("name", 12);
    CHECK(variable.name == "name");
    CHECK(variable.value == 12);
}

TEST_CASE("make_varname", "[variable]")
{
    const auto varname = caff::detail::make_varname("name", "");
    CHECK(std::string{ varname } == "name");
}

TEST_CASE("caff_variable", "[variable]")
{
    const int name = 12;
    const auto variable = caff_variable(name);
    CHECK(variable.name == "name");
    CHECK(variable.value == 12);
}

TEST_CASE("caff_variable_ex", "[variable]")
{
    const int name = 12;
    const int value = 13;
    const auto variable = caff_variable_ex(name, value);
    CHECK(variable.name == "name");
    CHECK(variable.value == value);
}

TEST_CASE("caff_varname", "[variable]")
{
    const int name = 12;
    const auto varname = caff_varname(name);
    CHECK(std::string{ varname } == "name");
}
#endif
