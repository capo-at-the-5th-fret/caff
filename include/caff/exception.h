#pragma once

#include <exception>
#include <cstdint>
#include <optional>
#include <concepts>
#include "caff/source_location.h"
#include "caff/diagnostic_info.h"

namespace caff
{
    class exception : public std::exception
    {
    public:
        exception() = default;
        explicit exception(std::string_view message) : message_(message)
        {
        }

        std::string const& message() const
        {
            return message_;
        }

        std::string const& diagnostics_text() const
        {
            return diagnostics_text_;
        }

        std::optional<caff::source_location> const& location() const
        {
            return location_;
        }

        char const* what() const noexcept override
        {
            static constexpr std::string_view default_message
            {
                "unknown exception"
            };

            static constexpr std::string_view diagnostic_header
            {
                "[ diagnostic info ]"
            };

            try
            {
                std::string what_msg;

                // Append the location to the end of the diagnostics text
                const std::string location_text =
                    (location_)
                        ? caff::to_string(caff::make_location_info(*location_))
                        : "";

                // Estimate space needed for the full error message
                // This is the size of message/default message +
                // size of diagnostics + location + line feeds
                const std::size_t what_msg_size =
                    (!message_.empty() ? message_.size()
                                       : default_message.size()) +
                    (!diagnostics_text_.empty() ? (diagnostic_header.size() +
                                                   diagnostics_text_.size() + 2)
                                                : 0) +
                    (!location_text.empty() ? (location_text.size() + 1) : 0);

                what_msg.reserve(what_msg_size);

                if (const auto message = this->message(); !message.empty())
                {
                    what_msg = message;
                }
                else
                {
                    what_msg = default_message;
                }

                if (!diagnostics_text_.empty() || !location_text.empty())
                {
                    if (!what_msg.ends_with('\n'))
                    {
                        what_msg += '\n';
                    }

                    what_msg += diagnostic_header;
                    what_msg += '\n';

                    if (!diagnostics_text_.empty())
                    {
                        what_msg += diagnostics_text_;
                    }

                    if (!location_text.empty())
                    {
                        if (!diagnostics_text_.empty())
                        {
                            what_msg += '\n';
                        }
                        what_msg += location_text;
                    }
                }

                what_.swap(what_msg);
            }
            catch (...)
            {
                what_ = default_message;
            }

            return what_.c_str();
        }

    private:
        mutable std::string message_;
        mutable std::string diagnostics_text_;
        mutable std::optional<caff::source_location> location_;
        mutable std::string what_;

        template <std::derived_from<caff::exception> ExceptionType, typename T>
        friend ExceptionType const& operator<<(
            ExceptionType const& e, caff::diagnostic_info<T> const& info);

        template <std::derived_from<caff::exception> ExceptionType>
        friend ExceptionType const& operator<<(
            ExceptionType const& e, caff::source_location const& location);
    };

    template <std::derived_from<caff::exception> ExceptionType, typename T>
    ExceptionType const& operator<<(ExceptionType const& e,
                                    caff::diagnostic_info<T> const& info)
    {
        try
        {
            if (!e.diagnostics_text_.empty())
            {
                e.diagnostics_text_ += '\n';
            }

            e.diagnostics_text_ += caff::to_string(info);
        }
        catch (...)
        {
        }

        return e;
    }

    template <std::derived_from<caff::exception> ExceptionType, typename T>
    ExceptionType const& operator<<(ExceptionType const& e,
                                    caff::variable<T> const& variable)
    {
        e << caff::make_variable_info(variable.name)
          << caff::make_value_info(variable.value);
        return e;
    }

    template <std::derived_from<caff::exception> ExceptionType>
    ExceptionType const& operator<<(ExceptionType const& e,
                                    caff::source_location const& location)
    {
        e.location_ = location;
        return e;
    }

    template <std::derived_from<caff::exception> ExceptionType>
    [[noreturn]] void throw_ex(ExceptionType const& e,
                               caff::source_location const& location =
                                   caff::source_location::current())
    {
        e << location;
        throw e;
    }
}
