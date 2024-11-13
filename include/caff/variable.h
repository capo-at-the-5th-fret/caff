#pragma once

#include <string>
#include <string_view>
#include <type_traits>

namespace caff
{
    // TODO: Why a special class for variables? Could this not just be done
    // using diagnostic_info? Is it simply because of referencing? If so, could
    // diagnostic_info be made with referencing? or use variable with copying?
    template <typename T>
    struct variable
    {
        explicit constexpr variable(std::string_view name, T& value)
            : name(name), value(value)
        {
        }

        explicit constexpr variable(std::string_view name,
                                    std::remove_reference_t<T>&& value)
            : name(name), value(std::move(value))
        {
        }

        using type = T;

        std::string name;
        T value{};
    };

    // TODO: Is this guide needed?
    variable(std::string_view, char const*) -> variable<std::string>;
    template <typename T>
    variable(std::string_view, T&) -> variable<T&>;
    template <typename T>
    variable(std::string_view, T&&) -> variable<T>;

    namespace detail
    {
        template <typename T>
        auto make_variable(std::string_view name, T&& value)
        {
            return caff::variable{name, std::forward<T>(value)};
        }

        template <typename T>
        std::string_view make_varname(std::string_view name, T const&)
        {
            return name;
        }
    }
}

// NOTE: caff_varname uses two variables so that variable name changing is
// caught by the compiler, which doesn't happen when (#var) is used
// i.e. func(int x) { caff_varname(x); } <- changing x to x2 would not be
// caught if caff_varname was (#var) instead of (#var, var)
#define caff_variable(var) caff::detail::make_variable(#var, var)
#define caff_variable_ex(var, val) caff::detail::make_variable(#var, val)
#define caff_varname(var) caff::detail::make_varname(#var, var)
