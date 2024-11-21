#pragma once

#include <fmt/core.h>
#include <fmt/ranges.h>
#include <fmt/std.h>

namespace caff
{
    class string_formatter
    {
    public:
        string_formatter() = default;
        explicit string_formatter(std::string_view fmt) : fmt_(fmt)
        {
        }

        template <typename... Args>
        requires(sizeof...(Args) > 0)
        std::string operator()(Args const&... args) const
        {
            return fmt::format(fmt::runtime(fmt_), args...);
        }

    private:
        const std::string fmt_{"{}"};
    };
}
