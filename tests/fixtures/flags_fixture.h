#pragma once

#include <tuple>
#include <vector>
#include <cstdint>
#include "caff/test/compare.h"
#include "caff/flags.h"

namespace caff::test
{
    namespace dummy
    {
        enum class option : std::uint8_t
        {
            none = 0,
            read = 0x01,
            write = 0x02,
            append = 0x04,
            trunc = 0x08
        };

        using options = caff::flags<option>;

        constexpr options options_all = options
        {
            option::read, option::write, option::append, option::trunc
        };

        struct options_fixture
        {
            static constexpr std::initializer_list<option> base_option_list = { option::write, option::append };
            static constexpr std::initializer_list<option> diff_option_list = { option::read, option::trunc };

            options base{ base_option_list };
            options diff{ diff_option_list };

            std::vector<std::tuple<options, equality_comparator>> comparisons =
            {
                { base, equality_equal },
                //{ options{  }, equality_not_equal },
                //{ options{ option::read }, equality_not_equal },
                //{ options{ option::write, option::append, option::trunc }, equality_not_equal },
                //{ options_all, equality_not_equal }
            };
        };
    }
}