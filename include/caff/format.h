#pragma once

#include <format>

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
            return std::vformat(fmt_, std::make_format_args(args...));
        }

    private:
        const std::string fmt_{"{}"};
    };
}
