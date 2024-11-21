#pragma once

#include <string>
#include <concepts>
#include <ranges>
#include <source_location>
#include "caff/format.h"
#include "caff/variable.h"

namespace caff
{
    template <typename T>
    requires std::copyable<T>
    class diagnostic_info
    {
    public:
        using value_type = T;

        template <typename U>
        requires std::constructible_from<value_type, U>
        explicit diagnostic_info(std::string_view name, U const& value)
            : name_(name), value_(value)
        {
        }

        explicit diagnostic_info(std::string_view name, const char* value) :
            name_(name), value_(value)
        {
        }

        template <typename U, std::size_t N>
        explicit diagnostic_info(std::string_view name, U (&a)[N])
            : name_(name), value_(std::to_array(a))
        {
        }

        std::string const& name() const
        {
            return name_;
        }

        value_type const& value() const
        {
            return value_;
        }

    private:
        std::string name_;
        value_type value_;
    };

    template <typename T>
    diagnostic_info(std::string_view, T) -> diagnostic_info<T>;
    diagnostic_info(std::string_view, const char*) -> diagnostic_info<std::string>;
    diagnostic_info(std::string_view, std::string_view) -> diagnostic_info<std::string>;
    template <typename T, std::size_t N>
    diagnostic_info(std::string_view, T (&a)[N]) -> diagnostic_info<decltype(std::to_array(a))>;

    template <typename T>
    std::string to_string(caff::diagnostic_info<T> const& info)
    {
        // format pointers as addresses
        if constexpr (std::is_pointer_v<T>)
        {
            return fmt::format("{}: {}", info.name(),
                static_cast<void*>(info.value()));
        }
        // member pointers are always 1 (that's what iostream does)
        else if constexpr (std::is_member_pointer_v<T>)
        {
            return fmt::format("{}: {}", info.name(), 1);
        }
        else
        {
            static_assert(fmt::formattable<T>,
                "T must be formattable by the format library");
            return fmt::format("{}: {}", info.name(), info.value());
        }
    }

    template <typename T>
    auto make_diagnostic_info(std::string_view name, T const& value)
    {
        return caff::diagnostic_info{name, value};
    }

    template <typename... Args>
    auto format_diagnostic_info(std::string_view name,  // NOLINT(bugprone-easily-swappable-parameters)
                                std::string_view fmt,
                                Args const&... args)
    {
        std::string text;

        try
        {
            text = fmt::format(fmt::runtime(fmt), args...);
        }
        catch (...)
        {
            text = "unknown";
        }

        return caff::diagnostic_info{name, text};
    }

    inline auto make_location_info(std::source_location const& location)
    {
        return caff::format_diagnostic_info(
            "location", "{}({})", location.file_name(), location.line());
    }

    inline auto make_variable_info(std::string_view variable_name)
    {
        return caff::diagnostic_info<std::string>{"variable", variable_name};
    }

    template <typename T>
    auto make_value_info(T const& value)
    {
        return caff::diagnostic_info{"value", value};
    }

    template <typename T>
    auto make_index_info(T const& index)
    {
        return caff::diagnostic_info{"index", index};
    }

    inline auto make_null_value_info()
    {
        return caff::make_value_info("null");
    }

    inline auto make_message_info(std::string_view message)
    {
        return caff::diagnostic_info<std::string>{"message", message};
    }

    // TODO: Make a path overload?
    inline auto make_file_path_info(std::string_view path)
    {
        return caff::diagnostic_info<std::string>{"file_path", path};
    }

    inline auto make_condition_info(std::string_view condition)
    {
        return caff::diagnostic_info<std::string>{"condition", condition};
    }

    template <typename... Args>
    auto format_condition_info(std::string_view fmt, Args const&... args)
    {
        std::string text;

        try
        {
            text = fmt::format(fmt::runtime(fmt), args...);
        }
        catch (...)
        {
            text = "unknown";
        }

        return caff::make_condition_info(text);
    }

    template <typename T>
    auto make_equal_to_condition(T const& value)
    {
        return caff::format_condition_info("== {}", value);
    }

    template <typename T>
    auto make_not_equal_to_condition(T const& value)
    {
        return caff::format_condition_info("!= {}", value);
    }

    template <typename T>
    auto make_less_condition(T const& value)
    {
        return caff::format_condition_info("< {}", value);
    }

    template <typename T>
    auto make_greater_condition(T const& value)
    {
        return caff::format_condition_info("> {}", value);
    }

    template <typename T>
    auto make_less_equal_condition(T const& value)
    {
        return caff::format_condition_info("<= {}", value);
    }

    template <typename T>
    auto make_greater_equal_condition(T const& value)
    {
        return caff::format_condition_info(">= {}", value);
    }

    enum class range_type
    {
        closed,     // >= && <=
        left_open,  // > && <=
        right_open, // >= && <
        open        // > && <
    };

    template <typename T1, typename T2>
    auto make_range_info(T1 const& t1,
                         T2 const& t2,
                         caff::range_type range_type)
    {
        if (range_type == range_type::left_open)
        {
            return caff::format_condition_info("> {} && <= {}", t1, t2);
        }

        if (range_type == range_type::right_open)
        {
            return caff::format_condition_info(">= {} && < {}", t1, t2);
        }

        if (range_type == range_type::open)
        {
            return caff::format_condition_info("> {} && < {}", t1, t2);
        }

        return caff::format_condition_info(">= {} && <= {}", t1, t2);
    }

    template <std::ranges::sized_range R>
    inline auto make_sized_range_info(
        R const& r, caff::range_type range_type = caff::range_type::right_open)
    {
        return caff::make_range_info(0, std::ranges::size(r), range_type);
    }

    template <typename T1, typename T2>
    inline auto make_closed_range_info(T1 const& t1, T2 const& t2)
    {
        return caff::make_range_info(t1, t2, caff::range_type::closed);
    }

    template <typename T1, typename T2>
    inline auto make_half_open_range_info(T1 const& t1, T2 const& t2)
    {
        return caff::make_range_info(t1, t2, caff::range_type::right_open);
    }

    template <typename S, typename UnaryFunction = caff::string_formatter>
    auto make_set_condition(S const& set, UnaryFunction f = UnaryFunction{})
    {
        std::string text = "{";

        using std::begin;
        using std::end;

        auto first = begin(set);
        auto last = end(set);

        if (first != last)
        {
            try
            {
                text += f(*first);
            }
            catch (...)
            {
                text += "unknown";
            }

            while (++first != last)
            {
                try
                {
                    text += ", ";
                    text += f(*first);
                }
                catch (...)
                {
                    text += "unknown";
                }
            }
        }

        text += "}";

        return caff::make_condition_info(text);
    }
}

template <typename T>
struct fmt::formatter<caff::diagnostic_info<T>> : fmt::formatter<std::string>
{
    auto format(caff::diagnostic_info<T> const& info,
        fmt::format_context& ctx) const -> fmt::format_context::iterator
    {
        return fmt::formatter<std::string>::format(caff::to_string(info), ctx);
    }
};
