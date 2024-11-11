#include <doctest/doctest.h>
#include "caff/format.h"

TEST_SUITE("string_formatter")
{
    using caff::string_formatter;

    TEST_CASE("constructor")
    {
        string_formatter sf;
        //REQUIRE_THROWS(sf() == "");
        REQUIRE(sf(12) == "12");
    }

    TEST_CASE("operator")
    {
        {
            string_formatter sf{ "{}, {:.1f}" };
            REQUIRE(sf(12, 2.42) == "12, 2.4");
        }

        {
            string_formatter sf2{ "{}, {:.1f}" };
            REQUIRE_THROWS(sf2(12));
        }
    }
}