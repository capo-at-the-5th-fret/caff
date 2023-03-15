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
        using ut = std::underlying_type_t<options>;
        return static_cast<options>(~static_cast<ut>(rhs));
    }
    
    constexpr options& operator&=(options& lhs, options rhs) noexcept
    {
        using ut = std::underlying_type_t<options>;
        lhs = static_cast<options>(static_cast<ut>(lhs) & static_cast<ut>(rhs));
        return lhs;
    }
    
    constexpr options& operator|=(options& lhs, options rhs) noexcept
    {
        using ut = std::underlying_type_t<options>;
        lhs = static_cast<options>(static_cast<ut>(lhs) | static_cast<ut>(rhs));
        return lhs;
    }
    
    constexpr options& operator^=(options& lhs, options rhs) noexcept
    {
        using ut = std::underlying_type_t<options>;
        lhs = static_cast<options>(static_cast<ut>(lhs) ^ static_cast<ut>(rhs));
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
    using namespace ez;
    
    options o = options::none;

    SUBCASE("set single bit at a time")
    {
        set(o, options::write);
        REQUIRE(test(o, options::write));
        REQUIRE_FALSE(test(o, options::read));
        REQUIRE_FALSE(test(o, options::append));
        REQUIRE_FALSE(test(o, options::trunc));

        set(o, options::append);
        REQUIRE(test(o, options::write));
        REQUIRE_FALSE(test(o, options::read));
        REQUIRE(test(o, options::append));
        REQUIRE_FALSE(test(o, options::trunc));
    }

    SUBCASE("set multiple bits at a time")
    {
        set(o, options::write, options::append);
        REQUIRE(test(o, options::write));
        REQUIRE_FALSE(test(o, options::read));
        REQUIRE(test(o, options::append));
        REQUIRE_FALSE(test(o, options::trunc));
    }

    SUBCASE("reset single bit at a time")
    {
        set(o, options::write, options::append);
        reset(o, options::write);
        REQUIRE_FALSE(test(o, options::write));
        REQUIRE_FALSE(test(o, options::read));
        REQUIRE(test(o, options::append));
        REQUIRE_FALSE(test(o, options::trunc));

        reset(o, options::append);
        REQUIRE(o == options::none);
    }

    SUBCASE("reset multiple bits at a time")
    {
        set(o, options::write, options::append);
        reset(o, options::write, options::append);
        REQUIRE(o == options::none);
    }

    SUBCASE("reset all bits")
    {
        set(o, options::write, options::append);
        reset(o);
        REQUIRE(o == options::none);
    }

    SUBCASE("test single bit at a time")
    {
        set(o, options::write, options::append);
        REQUIRE(test(o, options::write));
        REQUIRE_FALSE(test(o, options::read));
        REQUIRE(test(o, options::append));
        REQUIRE_FALSE(test(o, options::trunc));
    }

    SUBCASE("test multiple bits at a time")
    {
        set(o, options::write, options::append);
        REQUIRE(test(o, options::write, options::append));
        REQUIRE_FALSE(test(o, options::write, options::read));
        REQUIRE_FALSE(test(o, options::write, options::trunc));
        REQUIRE_FALSE(test(o, options::read, options::write));
        REQUIRE_FALSE(test(o, options::trunc, options::write));
        REQUIRE_FALSE(test(o, options::append, options::read));
        REQUIRE_FALSE(test(o, options::append, options::trunc));
        REQUIRE_FALSE(test(o, options::read, options::append));
        REQUIRE_FALSE(test(o, options::trunc, options::append));
        REQUIRE_FALSE(test(o, options::read, options::trunc));
    }

    SUBCASE("flip single bit at a time")
    {
        flip(o, options::write);
        REQUIRE(test(o, options::write));
        REQUIRE_FALSE(test(o, options::read));
        REQUIRE_FALSE(test(o, options::append));
        REQUIRE_FALSE(test(o, options::trunc));

        flip(o, options::append);
        REQUIRE(test(o, options::write));
        REQUIRE_FALSE(test(o, options::read));
        REQUIRE(test(o, options::append));
        REQUIRE_FALSE(test(o, options::trunc));

        flip(o, options::write);
        REQUIRE_FALSE(test(o, options::write));
        REQUIRE_FALSE(test(o, options::read));
        REQUIRE(test(o, options::append));
        REQUIRE_FALSE(test(o, options::trunc));

        flip(o, options::append);
        REQUIRE(o == options::none);
    }

    SUBCASE("flip multiple bits at a time")
    {
        flip(o, options::write, options::append);
        REQUIRE(test(o, options::write));
        REQUIRE_FALSE(test(o, options::read));
        REQUIRE(test(o, options::append));
        REQUIRE_FALSE(test(o, options::trunc));

        flip(o, options::write, options::append);
        REQUIRE(o == options::none);
    }

    SUBCASE("test_xxx")
    {
        set(o, options::write, options::append);

        REQUIRE(test_all_of(o, options::write));
        REQUIRE(test_all_of(o, options::append));
        REQUIRE(test_all_of(o, options::write, options::append));
        REQUIRE_FALSE(
            test_all_of(o, options::write, options::append, options::read));

        REQUIRE(test_any_of(o, options::write));
        REQUIRE(test_any_of(o, options::append));
        REQUIRE_FALSE(test_any_of(o, options::read));

        REQUIRE(test_none_of(o, options::read));
        REQUIRE_FALSE(test_none_of(o, options::write));
        REQUIRE_FALSE(test_none_of(o, options::append));
    }
}
