#include <doctest/doctest.h>
#include "easy/bit.h"

namespace
{
    enum class options : std::uint8_t
    {
        none = 0,
        read = 0x01,
        write = 0x02,
        append = 0x04,
        trunc = 0x08
    };

    constexpr options operator~(options rhs) noexcept
    {
        return static_cast<options>(~std::to_underlying(rhs));
    }
    
    constexpr options& operator&=(options& lhs, options rhs) noexcept
    {
        lhs = static_cast<options>(std::to_underlying(lhs) & std::to_underlying(rhs));
        return lhs;
    }
    
    constexpr options& operator|=(options& lhs, options rhs) noexcept
    {
        lhs = static_cast<options>(std::to_underlying(lhs) | std::to_underlying(rhs));
        return lhs;
    }
    
    constexpr options& operator^=(options& lhs, options rhs) noexcept
    {
        lhs = static_cast<options>(std::to_underlying(lhs) ^ std::to_underlying(rhs));
        return lhs;
    }
    
    constexpr options operator&(options lhs, options rhs) noexcept
    {
        lhs &= rhs;
        return lhs;
    }
    
    constexpr options operator|(options lhs, options rhs) noexcept
    {
        lhs |= rhs;
        return lhs;
    }
    
    constexpr options operator^(options lhs, options rhs) noexcept
    {
        lhs ^= rhs;
        return lhs;
    }
}

TEST_CASE("enum bitmask")
{
    using namespace easy;
    using enum options;

    options o{ none };

    SUBCASE("set single bit at a time")
    {
        set(o, write);
        REQUIRE(test(o, write));
        REQUIRE_FALSE(test(o, read));
        REQUIRE_FALSE(test(o, append));
        REQUIRE_FALSE(test(o, trunc));

        set(o, append);
        REQUIRE(test(o, write));
        REQUIRE_FALSE(test(o, read));
        REQUIRE(test(o, append));
        REQUIRE_FALSE(test(o, trunc));
    }

    SUBCASE("set multiple bits at a time")
    {
        set(o, write, append);
        REQUIRE(test(o, write));
        REQUIRE_FALSE(test(o, read));
        REQUIRE(test(o, append));
        REQUIRE_FALSE(test(o, trunc));
    }

    SUBCASE("reset single bit at a time")
    {
        set(o, write, append);
        reset(o, write);
        REQUIRE_FALSE(test(o, write));
        REQUIRE_FALSE(test(o, read));
        REQUIRE(test(o, append));
        REQUIRE_FALSE(test(o, trunc));

        reset(o, append);
        REQUIRE(o == none);
    }

    SUBCASE("reset multiple bits at a time")
    {
        set(o, write, append);
        reset(o, write, append);
        REQUIRE(o == none);
    }

    SUBCASE("reset all bits")
    {
        set(o, write, append);
        reset(o);
        REQUIRE(o == none);
    }

    SUBCASE("test single bit at a time")
    {
        set(o, write, append);
        REQUIRE(test(o, write));
        REQUIRE_FALSE(test(o, read));
        REQUIRE(test(o, append));
        REQUIRE_FALSE(test(o, trunc));
    }

    SUBCASE("test multiple bits at a time")
    {
        set(o, write, append);
        REQUIRE(test(o, write, append));
        REQUIRE_FALSE(test(o, write, read));
        REQUIRE_FALSE(test(o, write, trunc));
        REQUIRE_FALSE(test(o, read, write));
        REQUIRE_FALSE(test(o, trunc, write));
        REQUIRE_FALSE(test(o, append, read));
        REQUIRE_FALSE(test(o, append, trunc));
        REQUIRE_FALSE(test(o, read, append));
        REQUIRE_FALSE(test(o, trunc, append));
        REQUIRE_FALSE(test(o, read, trunc));
    }

    SUBCASE("flip single bit at a time")
    {
        flip(o, write);
        REQUIRE(test(o, write));
        REQUIRE_FALSE(test(o, read));
        REQUIRE_FALSE(test(o, append));
        REQUIRE_FALSE(test(o, trunc));

        flip(o, append);
        REQUIRE(test(o, write));
        REQUIRE_FALSE(test(o, read));
        REQUIRE(test(o, append));
        REQUIRE_FALSE(test(o, trunc));

        flip(o, write);
        REQUIRE_FALSE(test(o, write));
        REQUIRE_FALSE(test(o, read));
        REQUIRE(test(o, append));
        REQUIRE_FALSE(test(o, trunc));

        flip(o, append);
        REQUIRE(o == none);
    }

    SUBCASE("flip multiple bits at a time")
    {
        flip(o, write, append);
        REQUIRE(test(o, write));
        REQUIRE_FALSE(test(o, read));
        REQUIRE(test(o, append));
        REQUIRE_FALSE(test(o, trunc));

        flip(o, write, append);
        REQUIRE(o == none);
    }

    SUBCASE("test_xxx")
    {
        set(o, write, append);

        REQUIRE(test_all_of(o, write));
        REQUIRE(test_all_of(o, append));
        REQUIRE(test_all_of(o, write, append));
        REQUIRE_FALSE(
            test_all_of(o, write, append, read));

        REQUIRE(test_any_of(o, write));
        REQUIRE(test_any_of(o, append));
        REQUIRE_FALSE(test_any_of(o, read));

        REQUIRE(test_none_of(o, read));
        REQUIRE_FALSE(test_none_of(o, write));
        REQUIRE_FALSE(test_none_of(o, append));
    }
}
