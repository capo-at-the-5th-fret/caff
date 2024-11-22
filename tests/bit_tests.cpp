#include <catch2/catch_test_macros.hpp>
#include "caff/bit.h"

#if 0
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

TEST_CASE("enum bitmask", "[bit]")
{
    using namespace caff;
    using enum options;

    options o{ none };

    SECTION("set single bit at a time")
    {
        set(o, write);
        CHECK(test(o, write));
        CHECK_FALSE(test(o, read));
        CHECK_FALSE(test(o, append));
        CHECK_FALSE(test(o, trunc));

        set(o, append);
        CHECK(test(o, write));
        CHECK_FALSE(test(o, read));
        CHECK(test(o, append));
        CHECK_FALSE(test(o, trunc));
    }

    SECTION("set multiple bits at a time")
    {
        set(o, write, append);
        CHECK(test(o, write));
        CHECK_FALSE(test(o, read));
        CHECK(test(o, append));
        CHECK_FALSE(test(o, trunc));
    }

    SECTION("reset single bit at a time")
    {
        set(o, write, append);
        reset(o, write);
        CHECK_FALSE(test(o, write));
        CHECK_FALSE(test(o, read));
        CHECK(test(o, append));
        CHECK_FALSE(test(o, trunc));

        reset(o, append);
        CHECK(o == none);
    }

    SECTION("reset multiple bits at a time")
    {
        set(o, write, append);
        reset(o, write, append);
        CHECK(o == none);
    }

    SECTION("reset all bits")
    {
        set(o, write, append);
        reset(o);
        CHECK(o == none);
    }

    SECTION("test single bit at a time")
    {
        set(o, write, append);
        CHECK(test(o, write));
        CHECK_FALSE(test(o, read));
        CHECK(test(o, append));
        CHECK_FALSE(test(o, trunc));
    }

    SECTION("test multiple bits at a time")
    {
        set(o, write, append);
        CHECK(test(o, write, append));
        CHECK_FALSE(test(o, write, read));
        CHECK_FALSE(test(o, write, trunc));
        CHECK_FALSE(test(o, read, write));
        CHECK_FALSE(test(o, trunc, write));
        CHECK_FALSE(test(o, append, read));
        CHECK_FALSE(test(o, append, trunc));
        CHECK_FALSE(test(o, read, append));
        CHECK_FALSE(test(o, trunc, append));
        CHECK_FALSE(test(o, read, trunc));
    }

    SECTION("flip single bit at a time")
    {
        flip(o, write);
        CHECK(test(o, write));
        CHECK_FALSE(test(o, read));
        CHECK_FALSE(test(o, append));
        CHECK_FALSE(test(o, trunc));

        flip(o, append);
        CHECK(test(o, write));
        CHECK_FALSE(test(o, read));
        CHECK(test(o, append));
        CHECK_FALSE(test(o, trunc));

        flip(o, write);
        CHECK_FALSE(test(o, write));
        CHECK_FALSE(test(o, read));
        CHECK(test(o, append));
        CHECK_FALSE(test(o, trunc));

        flip(o, append);
        CHECK(o == none);
    }

    SECTION("flip multiple bits at a time")
    {
        flip(o, write, append);
        CHECK(test(o, write));
        CHECK_FALSE(test(o, read));
        CHECK(test(o, append));
        CHECK_FALSE(test(o, trunc));

        flip(o, write, append);
        CHECK(o == none);
    }

    SECTION("test_xxx")
    {
        set(o, write, append);

        CHECK(test_all_of(o, write));
        CHECK(test_all_of(o, append));
        CHECK(test_all_of(o, write, append));
        CHECK_FALSE(
            test_all_of(o, write, append, read));

        CHECK(test_any_of(o, write));
        CHECK(test_any_of(o, append));
        CHECK_FALSE(test_any_of(o, read));

        CHECK(test_none_of(o, read));
        CHECK_FALSE(test_none_of(o, write));
        CHECK_FALSE(test_none_of(o, append));
    }
}
#endif