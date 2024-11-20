#pragma once

#include <cstdint>
#include <functional>
#include "caff/format.h"

namespace caff::test
{
    enum class dummy_enum : std::uint8_t
    {
        one,
        two,
        three,
        four,
        five
    };

    constexpr std::string_view to_string(dummy_enum e) noexcept
    {
        using enum dummy_enum;

        if (e == one)
        {
            return "one";
        }
        else if (e == two)
        {
            return "two";
        }
        else if (e == three)
        {
            return "three";
        }
        else if (e == four)
        {
            return "four";
        }
        else if (e == five)
        {
            return "five";
        }
        else
        {
            return "unknown";
        }
    }
    
    class dummy_class
    {
    public:
        using member_type = int;

        auto operator<=>(dummy_class const&) const = default;

        void member_function() { }

        int member_variable = 0;
    };

    class other_dummy_class
    {
    };

    union dummy_union
    {
        int i;
        double d;
    };

    constexpr int dummy_function()
    {
        return 1;
    }

    struct projection_type
    {
        using value_type = int;

        auto operator<=>(projection_type const&) const = default; // NOLINT(modernize-use-nullptr)

        value_type value{0};
    };
}

template <>
struct std::hash<caff::test::projection_type>
{
    std::size_t operator()(
        caff::test::projection_type const& key) const noexcept
    {
        return std::hash<std::size_t>{}(key.value);
    }
};

template <>
struct fmt::formatter<caff::test::dummy_enum> : formatter<string_view>
{
    auto format(caff::test::dummy_enum e, format_context& ctx) const
        -> format_context::iterator
    {
        return formatter<string_view>::format(to_string(e), ctx);
    }
};

template <>
struct fmt::formatter<caff::test::dummy_class> : formatter<int>
{
    auto format(caff::test::dummy_class const& dc, format_context& ctx) const
        -> format_context::iterator
    {
        return formatter<int>::format(dc.member_variable, ctx);
    }
};