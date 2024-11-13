#pragma once

#include "caff/exception.h"

namespace caff
{
    // logic_error
    inline constexpr std::string_view logic_error_message{"logic error"};

    class logic_error : public caff::exception
    {
    public:
        using caff::exception::exception;

        logic_error() : logic_error(logic_error_message)
        {
        }
    };

    template <typename... Args>
    caff::logic_error make_logic_error(Args&&... args)
    {
        return caff::logic_error{std::forward<Args>(args)...};
    }

    // domain_error
    inline constexpr std::string_view domain_error_message{"domain error"};

    class domain_error : public caff::logic_error
    {
    public:
        using caff::logic_error::logic_error;

        domain_error() : domain_error(domain_error_message)
        {
        }
    };

    template <typename... Args>
    caff::domain_error make_domain_error(Args... args)
    {
        return caff::domain_error{std::forward<Args>(args)...};
    }

    template <typename T, typename U>
    caff::domain_error make_domain_error_ex(
        std::string_view message,
        caff::variable<T> const& variable,
        caff::diagnostic_info<U> const& condition_info)
    {
        return caff::domain_error{message} << variable << condition_info;
    }

    template <typename T, typename U>
    caff::domain_error make_domain_error_ex(
        caff::variable<T> const& variable,
        caff::diagnostic_info<U> const& condition_info)
    {
        return caff::domain_error{} << variable << condition_info;
    }

    // invalid_argument
    inline constexpr std::string_view invalid_argument_message{
        "invalid argument"};

    class invalid_argument : public caff::logic_error
    {
    public:
        using caff::logic_error::logic_error;

        invalid_argument() : invalid_argument(invalid_argument_message)
        {
        }
    };

    template <typename... Args>
    caff::invalid_argument make_invalid_argument(Args... args)
    {
        return caff::invalid_argument{std::forward<Args>(args)...};
    }

    template <typename T, typename U>
    caff::invalid_argument make_invalid_argument_ex(
        std::string_view message,
        caff::variable<T> const& variable,
        caff::diagnostic_info<U> const& condition_info)
    {
        return caff::invalid_argument{message} << variable << condition_info;
    }

    template <typename T, typename U>
    caff::invalid_argument make_invalid_argument_ex(
        caff::variable<T> const& variable,
        caff::diagnostic_info<U> const& condition_info)
    {
        return caff::invalid_argument{} << variable << condition_info;
    }

    // invalid_operation
    inline constexpr std::string_view invalid_operation_message{
        "invalid operation"};

    class invalid_operation : public caff::logic_error
    {
    public:
        using caff::logic_error::logic_error;

        invalid_operation() : invalid_operation(caff::invalid_operation_message)
        {
        }
    };

    template <typename... Args>
    caff::invalid_operation make_invalid_operation(Args&&... args)
    {
        return caff::invalid_operation{std::forward<Args>(args)...};
    }

    // length_error
    inline constexpr std::string_view length_error_message{"length error"};

    class length_error : public caff::logic_error
    {
    public:
        using caff::logic_error::logic_error;

        length_error() : length_error(caff::length_error_message)
        {
        }
    };

    template <typename... Args>
    caff::length_error make_length_error(Args&&... args)
    {
        return caff::length_error{std::forward<Args>(args)...};
    }

    // null_pointer
    inline constexpr std::string_view null_pointer_message{
        "an attempt was made to dereference a null pointer"};

    class null_pointer : public caff::logic_error
    {
    public:
        using caff::logic_error::logic_error;

        null_pointer() : null_pointer(caff::null_pointer_message)
        {
        }
    };

    template <typename... Args>
    null_pointer make_null_pointer(Args&&... args)
    {
        return null_pointer{std::forward<Args>(args)...};
    }

    inline null_pointer make_null_pointer_ex(std::string_view message,
                                             std::string_view variable_name)
    {
        return null_pointer{message} << caff::make_variable_info(variable_name)
                                     << caff::make_null_value_info();
    }

    inline null_pointer make_null_pointer_ex(std::string_view variable_name)
    {
        return make_null_pointer_ex(caff::null_pointer_message, variable_name);
    }

    // out_of_range
    inline constexpr std::string_view out_of_range_message{"out of range"};

    class out_of_range : public caff::logic_error
    {
    public:
        using caff::logic_error::logic_error;

        out_of_range() : out_of_range(out_of_range_message)
        {
        }
    };

    template <typename... Args>
    caff::out_of_range make_out_of_range(Args&&... args)
    {
        return caff::out_of_range{std::forward<Args>(args)...};
    }

    // argument_out_of_range
    inline constexpr std::string_view argument_out_of_range_message{
        "argument is out of range"};

    class argument_out_of_range : public caff::invalid_argument
    {
    public:
        using caff::invalid_argument::invalid_argument;

        argument_out_of_range()
            : argument_out_of_range(caff::argument_out_of_range_message)
        {
        }
    };

    template <typename... Args>
    caff::argument_out_of_range make_argument_out_of_range(Args&&... args)
    {
        return caff::argument_out_of_range{std::forward<Args>(args)...};
    }

    template <typename T, typename U>
    caff::argument_out_of_range make_argument_out_of_range_ex(
        std::string_view message,
        caff::variable<T> const& variable,
        caff::diagnostic_info<U> const& condition_info)
    {
        return caff::argument_out_of_range{message} << variable
                                                    << condition_info;
    }

    template <typename T, typename U>
    caff::argument_out_of_range make_argument_out_of_range_ex(
        caff::variable<T> const& variable,
        caff::diagnostic_info<U> const& condition_info)
    {
        return caff::argument_out_of_range{} << variable << condition_info;
    }

    template <typename T1, typename T2, typename U>
    caff::argument_out_of_range make_argument_out_of_range_ex(
        std::string_view message,
        caff::variable<T1> const& variable1,
        caff::variable<T2> const& variable2,
        caff::diagnostic_info<U> const& condition_info)
    {
        return caff::argument_out_of_range{message} << variable1 << variable2
                                                    << condition_info;
    }

    template <typename T1, typename T2, typename U>
    caff::argument_out_of_range make_argument_out_of_range_ex(
        caff::variable<T1> const& variable1,
        caff::variable<T2> const& variable2,
        caff::diagnostic_info<U> const& condition_info)
    {
        return caff::argument_out_of_range{} << variable1 << variable2
                                             << condition_info;
    }

    // index_out_of_range
    inline constexpr std::string_view index_out_of_range_message{
        "index is out of range"};

    class index_out_of_range : public caff::out_of_range
    {
    public:
        using caff::out_of_range::out_of_range;

        index_out_of_range()
            : index_out_of_range(caff::index_out_of_range_message)
        {
        }
    };

    template <typename... Args>
    caff::index_out_of_range make_index_out_of_range(Args&&... args)
    {
        return caff::index_out_of_range{std::forward<Args>(args)...};
    }

    template <typename T, typename U1, typename U2>
    caff::index_out_of_range make_index_out_of_range_ex(
        std::string_view message,
        caff::variable<T> const& variable,
        U1 const& low,
        U2 const& high,
        caff::range_type range_type = caff::range_type::closed)
    {
        return caff::index_out_of_range{message}
               << variable << caff::make_range_info(low, high, range_type);
    }

    template <typename T, typename U1, typename U2>
    caff::index_out_of_range make_index_out_of_range_ex(
        caff::variable<T> const& variable,
        U1 const& low,
        U2 const& high,
        caff::range_type range_type = caff::range_type::closed)
    {
        return caff::index_out_of_range()
               << variable << caff::make_range_info(low, high, range_type);
    }

    template <typename T, typename CollectionType>
    caff::index_out_of_range make_index_out_of_range_ex(
        std::string_view message,
        caff::variable<T> const& variable,
        CollectionType const& c,
        caff::range_type range_type = caff::range_type::right_open)
    {
        return caff::make_index_out_of_range_ex(
            message, variable, 0, std::ranges::size(c), range_type);
    }

    template <typename T, typename CollectionType>
    caff::index_out_of_range make_index_out_of_range_ex(
        caff::variable<T> const& variable,
        CollectionType const& c,
        caff::range_type range_type = caff::range_type::right_open)
    {
        return caff::make_index_out_of_range_ex(
            variable, 0, std::ranges::size(c), range_type);
    }

    // null_argument
    inline constexpr std::string_view null_argument_message{
        "value cannot be null"};

    class null_argument : public caff::invalid_argument
    {
    public:
        using invalid_argument::invalid_argument;

        null_argument() : null_argument(caff::null_argument_message)
        {
        }
    };

    template <typename... Args>
    caff::null_argument make_null_argument(Args&&... args)
    {
        return caff::null_argument{std::forward<Args>(args)...};
    }

    inline null_argument make_null_argument_ex(std::string_view message,
                                               std::string_view variable_name)
    {
        return null_argument{message} << caff::make_variable_info(variable_name)
                                      << caff::make_null_value_info();
    }

    inline null_argument make_null_argument_ex(std::string_view variable_name)
    {
        return make_null_argument_ex(caff::null_argument_message,
                                     variable_name);
    }

    // value_out_of_range
    inline constexpr std::string_view value_out_of_range_message{
        "value is out of range"};

    class value_out_of_range : public caff::out_of_range
    {
    public:
        using caff::out_of_range::out_of_range;

        value_out_of_range() : value_out_of_range(value_out_of_range_message)
        {
        }
    };

    template <typename... Args>
    caff::value_out_of_range make_value_out_of_range(Args&&... args)
    {
        return caff::value_out_of_range{std::forward<Args>(args)...};
    }

    template <typename T, typename U>
    caff::value_out_of_range make_value_out_of_range_ex(
        std::string_view message,
        caff::variable<T> const& variable,
        caff::diagnostic_info<U> const& condition_info)
    {
        return caff::value_out_of_range{message} << variable << condition_info;
    }

    template <typename T, typename U>
    caff::value_out_of_range make_value_out_of_range_ex(
        caff::variable<T> const& variable,
        caff::diagnostic_info<U> const& condition_info)
    {
        return caff::value_out_of_range{} << variable << condition_info;
    }

    template <typename T1, typename T2, typename U>
    caff::value_out_of_range make_value_out_of_range_ex(
        std::string_view message,
        caff::variable<T1> const& variable1,
        caff::variable<T2> const& variable2,
        caff::diagnostic_info<U> const& condition_info)
    {
        return caff::value_out_of_range{message} << variable1 << variable2
                                                 << condition_info;
    }

    template <typename T1, typename T2, typename U>
    caff::value_out_of_range make_value_out_of_range_ex(
        caff::variable<T1> const& variable1,
        caff::variable<T2> const& variable2,
        caff::diagnostic_info<U> const& condition_info)
    {
        return caff::value_out_of_range{} << variable1 << variable2
                                          << condition_info;
    }

    // runtime_error
    inline constexpr std::string_view runtime_error_message{"runtime error"};

    class runtime_error : public caff::exception
    {
    public:
        using caff::exception::exception;

        runtime_error() : runtime_error(runtime_error_message)
        {
        }
    };

    template <typename... Args>
    caff::runtime_error make_runtime_error(Args&&... args)
    {
        return caff::runtime_error{std::forward<Args>(args)...};
    }

    // format_error
    inline constexpr std::string_view format_error_message{"format error"};

    class format_error : public caff::runtime_error
    {
    public:
        using caff::runtime_error::runtime_error;

        format_error() : format_error(format_error_message)
        {
        }
    };

    template <typename... Args>
    caff::format_error make_format_error(Args&&... args)
    {
        return caff::format_error{std::forward<Args>(args)...};
    }

    template <typename T>
    caff::format_error make_format_error_ex(
        std::string_view message,
        caff::diagnostic_info<T> const& condition_info)
    {
        return caff::format_error{message} << condition_info;
    }

    template <typename T>
    caff::format_error make_format_error_ex(
        caff::diagnostic_info<T> const& condition_info)
    {
        return caff::format_error{} << condition_info;
    }

    // unsupported_operation
    inline constexpr std::string_view unsupported_operation_message{
        "unsupported operation"};

    class unsupported_operation : public caff::runtime_error
    {
    public:
        using caff::runtime_error::runtime_error;

        unsupported_operation()
            : unsupported_operation(unsupported_operation_message)
        {
        }
    };

    template <typename... Args>
    caff::unsupported_operation make_unsupported_operation(Args&&... args)
    {
        return caff::unsupported_operation{std::forward<Args>(args)...};
    }
}

// helper macros
#define caff_ensure_arg(a)                                                     \
    do                                                                         \
    {                                                                          \
        if (!(a))                                                              \
        {                                                                      \
            caff::throw_ex(caff::make_null_argument_ex(caff_varname(a)));      \
        }                                                                      \
    } while (false)

#define caff_ensure_ptr(p)                                                     \
    do                                                                         \
    {                                                                          \
        if (!(p))                                                              \
        {                                                                      \
            caff::throw_ex(caff::make_null_pointer_ex(caff_varname(p)));       \
        }                                                                      \
    } while (false)

#define caff_ensure_index(c, i)                                                \
    do                                                                         \
    {                                                                          \
        using std::size;                                                       \
        if (!(((i) >= 0) && ((i) < size(c))))                                  \
        {                                                                      \
            caff::throw_ex(                                                    \
                caff::make_index_out_of_range_ex(caff_variable(i), c));        \
        }                                                                      \
    } while (false)

#define caff_ensure_closed_index(c, i)                                         \
    do                                                                         \
    {                                                                          \
        using std::size;                                                       \
        if (!(((i) >= 0) && ((i) <= size(c))))                                 \
        {                                                                      \
            caff::throw_ex(caff::make_index_out_of_range_ex(                   \
                caff_variable(i), c, caff::range_type::closed));               \
        }                                                                      \
    } while (false)

#define caff_ensure_in_range_arg(a, l, h)                                      \
    do                                                                         \
    {                                                                          \
        if (!(((a) >= (l)) && ((a) < (h))))                                    \
        {                                                                      \
            caff::throw_ex(caff::make_argument_out_of_range_ex(                \
                caff_variable(a), caff::make_half_open_range_info((l), (h)))); \
        }                                                                      \
    } while (false)

#define caff_ensure_in_closed_range_arg(a, l, h)                               \
    do                                                                         \
    {                                                                          \
        if (!(((a) >= (l)) && ((a) <= (h))))                                   \
        {                                                                      \
            caff::throw_ex(caff::make_argument_out_of_range_ex(                \
                caff_variable(a), caff::make_closed_range_info((l), (h))));    \
        }                                                                      \
    } while (false)

#define caff_ensure_non_zero_arg(a)                                            \
    do                                                                         \
    {                                                                          \
        if ((a) == 0)                                                          \
        {                                                                      \
            caff::throw_ex(caff::make_argument_out_of_range_ex(                \
                caff_variable(a), caff::make_not_equal_to_condition(0)));      \
        }                                                                      \
    } while (false)

#define caff_ensure_greater_than_zero_arg(a)                                   \
    do                                                                         \
    {                                                                          \
        if (!((a) > 0))                                                        \
        {                                                                      \
            caff::throw_ex(caff::make_argument_out_of_range_ex(                \
                caff_variable(a), caff::make_greater_condition(0)));           \
        }                                                                      \
    } while (false)
