#include <catch2/catch_test_macros.hpp>
#include "caff/exception.h"

TEST_CASE("caff_exception")
{
    SECTION("constructors")
    {
        SECTION("default constructor")
        {
            caff::exception ex;
            REQUIRE(ex.message().empty());
            REQUIRE(ex.diagnostics_text().empty());
            REQUIRE_FALSE(ex.location().has_value());
            REQUIRE(ex.what() == std::string_view{ "unknown exception" });
        }

        SECTION("message constructor")
        {
            caff::exception ex{ "message" };
            REQUIRE(ex.message() == "message");
            REQUIRE(ex.diagnostics_text().empty());
            REQUIRE_FALSE(ex.location().has_value());
            REQUIRE(ex.what() == std::string_view{ "message" });
        }
    }

    SECTION("message")
    {
        caff::exception ex{ "message" };
        REQUIRE(ex.message() == "message");
    }

    SECTION("diagnostics_text")
    {
        caff::exception ex{ "message" };
        caff::diagnostic_info<int> diagnostic_info{ "name", 12 };
        ex << diagnostic_info;
        REQUIRE(ex.diagnostics_text() == "name: 12");

        const caff::diagnostic_info<int> diagnostic_info2{ "name2", 13 };
        ex << diagnostic_info2;
        REQUIRE(ex.diagnostics_text() == "name: 12\nname2: 13");
    }

    SECTION("location")
    {
        caff::exception ex{ "message" };
        REQUIRE_FALSE(ex.location().has_value());

        const std::source_location location;
        ex << location;
        const auto ex_location = ex.location();
        REQUIRE(ex_location.has_value());
        REQUIRE(ex_location->line() == location.line());
        REQUIRE(ex_location->column() == location.column());
        REQUIRE(ex_location->file_name() == location.file_name());
        REQUIRE(ex_location->function_name() == location.function_name());
    }

    SECTION("what")
    {
        SECTION("no message")
        {
            caff::exception ex;
            REQUIRE(ex.what() == std::string_view{ "unknown exception" });
        }

        SECTION("message only")
        {
            caff::exception ex{ "message" };
            REQUIRE(ex.what() == std::string_view{ "message" });
        }

        SECTION("message w/diagnostics text")
        {
            SECTION("message wo/new line")
            {
                caff::exception ex{ "message" };
                caff::diagnostic_info<int> diagnostic_info{ "name", 12 };
                ex << diagnostic_info;
                REQUIRE(ex.what() == std::string_view{
                    "message"
                    "\n"
                    "[ diagnostic info ]"
                    "\n"
                    "name: 12"
                });

                const caff::diagnostic_info<int> diagnostic_info2{ "name2", 13 };
                ex << diagnostic_info2;
                REQUIRE(ex.what() == std::string_view{
                    "message"
                    "\n"
                    "[ diagnostic info ]"
                    "\n"
                    "name: 12"
                    "\n"
                    "name2: 13"
                });
            }

            SECTION("message w/new line")
            {
                caff::exception ex{ "message\n" };
                caff::diagnostic_info<int> diagnostic_info{ "name", 12 };
                ex << diagnostic_info;
                REQUIRE(ex.what() == std::string_view{
                    "message\n"
                    "[ diagnostic info ]"
                    "\n"
                    "name: 12"
                });
            }
        }

        constexpr std::source_location location;
        const auto location_text = fmt::format("location: {}",
            caff::make_location_info(location).value());

        SECTION("message w/location")
        {
            SECTION("message wo/new line")
            {
                caff::exception ex{ "message" };
                ex << location;
                REQUIRE(ex.what() == fmt::format(
                    "message"
                    "\n"
                    "[ diagnostic info ]"
                    "\n"
                    "{}",
                    location_text
                ));
            }

            SECTION("message w/new line")
            {
                caff::exception ex{ "message\n" };
                ex << location;
                REQUIRE(ex.what() == fmt::format(
                    "message\n"
                    "[ diagnostic info ]"
                    "\n"
                    "{}",
                    location_text
                ));
            }
        }

        SECTION("message w/diagnostics text, w/location")
        {
            SECTION("message wo/new line")
            {
                caff::exception ex{ "message" };
                caff::diagnostic_info<int> diagnostic_info{ "name", 12 };
                ex << diagnostic_info << location;
                REQUIRE(ex.what() == fmt::format(
                    "message"
                    "\n"
                    "[ diagnostic info ]"
                    "\n"
                    "name: 12"
                    "\n"
                    "{}",
                    location_text
                ));

                const caff::diagnostic_info<int> diagnostic_info2{ "name2", 13 };
                ex << diagnostic_info2;
                REQUIRE(ex.what() == fmt::format(
                    "message"
                    "\n"
                    "[ diagnostic info ]"
                    "\n"
                    "name: 12"
                    "\n"
                    "name2: 13"
                    "\n"
                    "{}",
                    location_text
                ));
            }

            SECTION("message w/new line")
            {
                caff::exception ex{ "message\n" };
                caff::diagnostic_info<int> diagnostic_info{ "name", 12 };
                ex << diagnostic_info << location;
                REQUIRE(ex.what() == fmt::format(
                    "message\n"
                    "[ diagnostic info ]"
                    "\n"
                    "name: 12"
                    "\n"
                    "{}",
                    location_text
                ));
            }
        }
    }

    SECTION("operator<< - diagnostic_info")
    {
        caff::exception ex{ "message" };
        caff::diagnostic_info<int> diagnostic_info{ "name", 12 };
        ex << diagnostic_info;
        REQUIRE(ex.diagnostics_text() == "name: 12");

        const caff::diagnostic_info<int> diagnostic_info2{ "name2", 13 };
        ex << diagnostic_info2;
        REQUIRE(ex.diagnostics_text() == "name: 12\nname2: 13");
    }

    SECTION("operator<< - variable")
    {
        caff::exception ex{ "message" };
        int x = 10;
        ex << caff_variable(x);
        REQUIRE(ex.diagnostics_text() == "variable: x\nvalue: 10");

        const int y = 11;
        ex << caff_variable(y);
        REQUIRE(ex.diagnostics_text() ==
            "variable: x\nvalue: 10\nvariable: y\nvalue: 11");
    }

    SECTION("operator<< - source_location")
    {
        caff::exception ex{ "message" };
        const std::source_location location;
        ex << location;
        REQUIRE(ex.message() == std::string_view{ "message" });
        REQUIRE(ex.diagnostics_text().empty());

        const auto ex_location = ex.location();
        REQUIRE(ex_location.has_value());
        REQUIRE(ex_location->line() == location.line());
        REQUIRE(ex_location->column() == location.column());
        REQUIRE(ex_location->file_name() == location.file_name());
        REQUIRE(ex_location->function_name() == location.function_name());
    }

    SECTION("throw_ex")
    {
        caff::exception ex{ "message" };
        
        bool exception_caught = false;

        try
        {
            caff::throw_ex(ex);
        }
        catch(const std::exception& e)
        {
            exception_caught = true;
        }
        
        REQUIRE(exception_caught);
        REQUIRE(ex.location().has_value());
        // TODO: Add source_location file name and line check
    }
}

// doctest code
#if 0
TEST_SUITE("caff::exception")
{
    using namespace std::literals;

    TEST_CASE("constructors")
    {
        SUBCASE("default constructor")
        {
            caff::exception ex;
            REQUIRE(ex.message().empty());
            REQUIRE(ex.diagnostics_text().empty());
            REQUIRE_FALSE(ex.location().has_value());
            REQUIRE(ex.what() == "unknown exception"s);
        }

        SUBCASE("message constructor")
        {
            caff::exception ex{ "message" };
            REQUIRE(ex.message() == "message");
            REQUIRE(ex.diagnostics_text().empty());
            REQUIRE_FALSE(ex.location().has_value());
            REQUIRE(ex.what() == "message"s);
        }
    }

    TEST_CASE("message")
    {
        caff::exception ex{ "message" };
        REQUIRE(ex.message() == "message");
    }

    TEST_CASE("diagnostics_text")
    {
        caff::exception ex{ "message" };
        caff::diagnostic_info<int> diagnostic_info{ "name", 12 };
        ex << diagnostic_info;
        REQUIRE(ex.diagnostics_text() == "name: 12");

        const caff::diagnostic_info<int> diagnostic_info2{ "name2", 13 };
        ex << diagnostic_info2;
        REQUIRE(ex.diagnostics_text() == "name: 12\nname2: 13");
    }

    TEST_CASE("location")
    {
        caff::exception ex{ "message" };
        REQUIRE_FALSE(ex.location().has_value());

        const caff::source_location location;
        ex << location;
        const auto ex_location = ex.location();
        REQUIRE(ex_location.has_value());
        REQUIRE(ex_location->line() == location.line());
        REQUIRE(ex_location->column() == location.column());
        REQUIRE(ex_location->file_name() == location.file_name());
        REQUIRE(ex_location->function_name() == location.function_name());
    }

    TEST_CASE("what")
    {
        SUBCASE("no message")
        {
            caff::exception ex;
            REQUIRE(ex.what() == "unknown exception"s);
        }

        SUBCASE("message only")
        {
            caff::exception ex{ "message" };
            REQUIRE(ex.what() == "message"s);
        }

        SUBCASE("message w/diagnostics text")
        {
            SUBCASE("message wo/new line")
            {
                caff::exception ex{ "message" };
                caff::diagnostic_info<int> diagnostic_info{ "name", 12 };
                ex << diagnostic_info;
                REQUIRE(ex.what() == 
                    "message"
                    "\n"
                    "[ diagnostic info ]"
                    "\n"
                    "name: 12"s
                );

                const caff::diagnostic_info<int> diagnostic_info2{ "name2", 13 };
                ex << diagnostic_info2;
                REQUIRE(ex.what() == 
                    "message"
                    "\n"
                    "[ diagnostic info ]"
                    "\n"
                    "name: 12"
                    "\n"
                    "name2: 13"s
                );
            }

            SUBCASE("message w/new line")
            {
                caff::exception ex{ "message\n" };
                caff::diagnostic_info<int> diagnostic_info{ "name", 12 };
                ex << diagnostic_info;
                REQUIRE(ex.what() == 
                    "message\n"
                    "[ diagnostic info ]"
                    "\n"
                    "name: 12"s
                );
            }
        }

        constexpr caff::source_location location;
        const auto location_text = fmt::format("location: {}",
            caff::make_location_info(location).value);

        SUBCASE("message w/location")
        {
            SUBCASE("message wo/new line")
            {
                caff::exception ex{ "message" };
                ex << location;
                REQUIRE(ex.what() == fmt::format(
                    "message"
                    "\n"
                    "[ diagnostic info ]"
                    "\n"
                    "{}",
                    location_text
                ));
            }

            SUBCASE("message w/new line")
            {
                caff::exception ex{ "message\n" };
                ex << location;
                REQUIRE(ex.what() == fmt::format(
                    "message\n"
                    "[ diagnostic info ]"
                    "\n"
                    "{}",
                    location_text
                ));
            }
        }

        SUBCASE("message w/diagnostics text, w/location")
        {
            SUBCASE("message wo/new line")
            {
                caff::exception ex{ "message" };
                caff::diagnostic_info<int> diagnostic_info{ "name", 12 };
                ex << diagnostic_info << location;
                REQUIRE(ex.what() == fmt::format(
                    "message"
                    "\n"
                    "[ diagnostic info ]"
                    "\n"
                    "name: 12"
                    "\n"
                    "{}",
                    location_text
                ));

                const caff::diagnostic_info<int> diagnostic_info2{ "name2", 13 };
                ex << diagnostic_info2;
                REQUIRE(ex.what() == fmt::format(
                    "message"
                    "\n"
                    "[ diagnostic info ]"
                    "\n"
                    "name: 12"
                    "\n"
                    "name2: 13"
                    "\n"
                    "{}",
                    location_text
                ));
            }

            SUBCASE("message w/new line")
            {
                caff::exception ex{ "message\n" };
                caff::diagnostic_info<int> diagnostic_info{ "name", 12 };
                ex << diagnostic_info << location;
                REQUIRE(ex.what() == fmt::format(
                    "message\n"
                    "[ diagnostic info ]"
                    "\n"
                    "name: 12"
                    "\n"
                    "{}",
                    location_text
                ));
            }
        }

    }

    TEST_CASE("operator<< - diagnostic_info")
    {
        caff::exception ex{ "message" };
        caff::diagnostic_info<int> diagnostic_info{ "name", 12 };
        ex << diagnostic_info;
        REQUIRE(ex.diagnostics_text() == "name: 12");

        const caff::diagnostic_info<int> diagnostic_info2{ "name2", 13 };
        ex << diagnostic_info2;
        REQUIRE(ex.diagnostics_text() == "name: 12\nname2: 13");
    }

    TEST_CASE("operator<< - variable")
    {
        caff::exception ex{ "message" };
        int x = 10;
        ex << caff_variable(x);
        REQUIRE(ex.diagnostics_text() == "variable: x\nvalue: 10");

        const int y = 11;
        ex << caff_variable(y);
        REQUIRE(ex.diagnostics_text() ==
            "variable: x\nvalue: 10\nvariable: y\nvalue: 11");
    }

    TEST_CASE("operator<< - source_location")
    {
        caff::exception ex{ "message" };
        const caff::source_location location;
        ex << location;
        REQUIRE(ex.message() == "message"s);
        REQUIRE(ex.diagnostics_text().empty());

        const auto ex_location = ex.location();
        REQUIRE(ex_location.has_value());
        REQUIRE(ex_location->line() == location.line());
        REQUIRE(ex_location->column() == location.column());
        REQUIRE(ex_location->file_name() == location.file_name());
        REQUIRE(ex_location->function_name() == location.function_name());
    }

    TEST_CASE("throw_ex")
    {
        caff::exception ex{ "message" };
        
        bool exception_caught = false;

        try
        {
            caff::throw_ex(ex);
        }
        catch(const std::exception& e)
        {
            exception_caught = true;
        }
        
        REQUIRE(exception_caught);
        REQUIRE(ex.location().has_value());
        // TODO: Add source_location file name and line check
    }
}
#endif
