#pragma once

#include <cstdint>
#include <functional>

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

    class dummy_class
    {
    public:
        void member_function() { }

        int member_variable = 0;
        using member_type = int;
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
