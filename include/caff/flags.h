#pragma once

#include <type_traits>
#include <initializer_list>
#include <utility>
#include <bit>

namespace caff
{
    template <typename E, std::underlying_type_t<E> AllBitsMask>
    requires (std::is_enum_v<E> && std::is_unsigned_v<std::underlying_type_t<E>>)
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

        constexpr bool test(E value) const noexcept
        {
            const auto test_bits = std::to_underlying(value);
            return ((bits_ & test_bits) == test_bits);
        }

        constexpr bool all() const noexcept
        {
            return (bits_ == AllBitsMask);
        }

        constexpr bool any() const noexcept
        {
            return (bits_ != 0);
        }

        constexpr bool none() const noexcept
        {
            return (bits_ == 0);
        }

        constexpr std::size_t count() const noexcept
        {
            return std::popcount(bits_);
        }

        constexpr flags& operator&=(const flags& other) noexcept
        {
            bits_ &= other.bits_;
            return *this;
        }

        constexpr flags& operator|=(const flags& other) noexcept
        {
            bits_ |= other.bits_;
            return *this;
        }

        constexpr flags& operator^=(const flags& other) noexcept
        {
            bits_ ^= other.bits_;
            return *this;
        }

        constexpr flags operator~() const noexcept
        {
            return flags(*this).flip();
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

        constexpr flags& set(E value, bool set = true) noexcept
        {
            if (set)
            {
                bits_ |= std::to_underlying(value);
            }
            else
            {
                bits_ &= ~std::to_underlying(value);
            }

            return *this;
        }

        constexpr flags& reset() noexcept
        {
            bits_ = 0;
            return *this;
        }

        constexpr flags& reset(E value) noexcept
        {
            bits_ &= ~std::to_underlying(value);
            return *this;
        }

        constexpr flags& flip() noexcept
        {
            bits_ ^= AllBitsMask;
            return *this;
        }

        constexpr flags& flip(E value) noexcept
        {
            bits_ ^= std::to_underlying(value);
            return *this;
        }

        constexpr underlying_type to_underlying() const noexcept
        {
            return bits_;
        }

        friend constexpr flags operator&(const flags& lhs, const flags& rhs) noexcept
        {
            return flags{ lhs } &= rhs;
        }

        friend constexpr flags operator|(const flags& lhs, const flags& rhs) noexcept
        {
            return flags{ lhs } |= rhs;
        }

        friend constexpr flags operator^(const flags& lhs, const flags& rhs) noexcept
        {
            return flags{ lhs } ^= rhs;
        }

    private:
        underlying_type bits_{0};
    };
}

template <typename E, std::underlying_type_t<E> AllBitsMask>
struct std::hash<caff::flags<E, AllBitsMask>>
{
    std::size_t operator()(const caff::flags<E, AllBitsMask>& f) const noexcept
    {
        using hash_type = std::hash<typename caff::flags<E, AllBitsMask>::underlying_type>;
        return hash_type{}(f.to_underlying());
    }
};
