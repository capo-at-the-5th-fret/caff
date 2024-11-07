#pragma once

#include <type_traits>
#include <initializer_list>
#include <utility>

namespace caff
{
    template <typename E>
    requires std::is_enum_v<E>
    class flags
    {
    public:
        using enum_type = E;
        using underlying_type = std::underlying_type_t<E>;

        constexpr flags() = default;

        constexpr explicit flags(E value) : bits_{ std::to_underlying(value) }
        {
        }

        constexpr flags(std::initializer_list<E> values)
        {
            for (auto value : values)
            {
                bits_ |= std::to_underlying(value);
            }
        }

        constexpr flags& operator=(std::initializer_list<E> values)
        {
            bits_ = 0;
            for (auto value : values)
            {
                bits_ |= std::to_underlying(value);
            }
            return *this;
        }

        bool operator==(flags const& rhs) const = default;

        template <typename... Es>
        requires (... && std::is_same_v<Es, E>)
        constexpr flags& set(E value, Es... values) noexcept
        {
            bits_ |= (std::to_underlying(value) | ... |
                std::to_underlying(values));
            return *this;
        }

        template <typename... Es>
        requires (... && std::is_same_v<Es, E>)
        constexpr flags& reset(Es... values) noexcept
        {
            if constexpr (sizeof...(Es) > 0)
            {
                bits_ &= ~(... | std::to_underlying(values));
            }
            else
            {
                bits_ = 0;
            }

            return *this;
        }

        template <typename... Es>
        requires (... && std::is_same_v<Es, E>)
        [[nodiscard]] constexpr bool test(E value, Es... values) const noexcept
        {
            const auto test_bits = (std::to_underlying(value) | ... |
                std::to_underlying(values));
            return ((bits_ & test_bits) == test_bits);
        }

        template <typename... Es>
        requires (... && std::is_same_v<Es, E>)
        constexpr flags& flip(E value, Es... values) noexcept
        {
            bits_ ^= (std::to_underlying(value) | ... |
                std::to_underlying(values));
            return *this;
        }

        template <typename... Es>
        requires (... && std::is_same_v<Es, E>)
        constexpr bool test_all_of(E value, Es... values) const noexcept
        {
            const auto test_bits = (std::to_underlying(value) | ... |
                std::to_underlying(values));
            return ((bits_ & test_bits) == test_bits);
        }

        template <typename... Es>
        requires (... && std::is_same_v<Es, E>)
        constexpr bool test_any_of(E value, Es... values) const noexcept
        {
            return ((bits_ & (std::to_underlying(value) | ... |
                std::to_underlying(values))) != 0);
        }

        template <typename... Es>
        requires (... && std::is_same_v<Es, E>)
        constexpr bool test_none_of(E value, Es... values) const noexcept
        {
            return (bits_ & (std::to_underlying(value) | ... |
                std::to_underlying(values))) == 0;
        }

        constexpr underlying_type to_underlying() const noexcept
        {
            return bits_;
        }

    private:
        underlying_type bits_{0};
    };
}

template <typename E>
struct std::hash<caff::flags<E>>
{
    std::size_t operator()(const caff::flags<E>& f) const noexcept
    {
        using hash_type = std::hash<typename caff::flags<E>::underlying_type>;
        return hash_type{}(f.to_underlying());
    }
};
