#pragma once

#if __has_include(<source_location>) && defined(__cpp_lib_source_location)
    #include <source_location>
#else
    #include <cstdint>
#endif

namespace caff
{
#if __has_include(<source_location>) && defined(__cpp_lib_source_location)

    using ::std::source_location;

#else

    class source_location
    {
    public:
        static constexpr source_location current() noexcept
        {
            return {};
        }

        constexpr std::uint_least32_t line() const noexcept
        {
            return line_;
        }

        constexpr std::uint_least32_t column() const noexcept
        {
            return column_;
        }

        constexpr const char* file_name() const noexcept
        {
            return file_name_;
        }

        constexpr const char* function_name() const noexcept
        {
            return function_name_;
        }

    private:
        std::uint_least32_t line_{1};
        std::uint_least32_t column_{1};
        const char* file_name_{"file name"};
        const char* function_name_{"function name"};
    };

#endif

}
