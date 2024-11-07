#include <doctest/doctest.h>
#include "caff/utility.h"

#include <tuple>
#include "caff/type_list.h"

namespace
{
    template <typename T>
    struct in_range_fixture;

    template <caff::signed_standard_integer T>
    struct in_range_fixture<T>
    {
        using OT = std::make_unsigned_t<T>;

        static constexpr std::tuple empty_ranges
        {
            std::pair{ T{ -1 }, T{ -1 } },
            std::pair{ T{ 0 }, OT{ 0 } },
            std::pair{ OT{ 1 }, T{ 1 } },
            std::pair{ OT{ 1 }, OT{ 1 } }
        };

        static constexpr std::tuple single_ranges
        {
            std::pair{ T{ -2 }, T{ -1 } },
            std::pair{ T{ 0 }, OT{ 1 } },
            std::pair{ OT{ 1 }, T{ 2 } },
            std::pair{ OT{ 1 }, OT{ 2 } }
        };

        static constexpr std::tuple multi_ranges
        {
            std::pair{ T{ -2 }, T{ 0 } },
            std::pair{ T{ 0 }, OT{ 2 } },
            std::pair{ OT{ 1 }, T{ 3 } },
            std::pair{ OT{ 1 }, OT{ 3 } }
        };
    };

    template <caff::unsigned_standard_integer T>
    struct in_range_fixture<T>
    {
        using OT = std::make_signed_t<T>;

        static constexpr std::tuple empty_ranges
        {
            std::pair{ OT{ -1 }, OT{ -1 } },
            std::pair{ OT{ 0 }, T{ 0 } },
            std::pair{ T{ 1 }, OT{ 1 } },
            std::pair{ T{ 1 }, T{ 1 } }
        };

        static constexpr std::tuple single_ranges
        {
            std::pair{ OT{ -2 }, OT{ -1 } },
            std::pair{ OT{ 0 }, T{ 1 } },
            std::pair{ T{ 1 }, OT{ 2 } },
            std::pair{ T{ 1 }, T{ 2 } }
        };

        static constexpr std::tuple multi_ranges
        {
            std::pair{ OT{ -2 }, OT{ 0 } },
            std::pair{ OT{ 0 }, T{ 2 } },
            std::pair{ T{ 1 }, OT{ 3 } },
            std::pair{ T{ 1 }, T{ 3 } }
        };
    };
}

TEST_CASE_TEMPLATE_DEFINE("in_open_range", TestType, in_open_range_test_id)
{
    using caff::in_open_range;

    in_range_fixture<TestType> fixture;

    // empty ranges
    {
        caff::tuple_for_each(fixture.empty_ranges,
            [](const auto& r)
            {
                for (auto i = r.first - 1; std::cmp_less_equal(i, r.second + 1);
                    ++i)
                {
                    CAPTURE(i);

                    {
                        const bool expected = (std::cmp_less(r.first, i) &&
                            std::cmp_less(i, r.second));
                        REQUIRE(caff::in_open_range(i, r.first, r.second) ==
                            expected);
                    }
                }
            });
    }

    // single ranges
    {
        caff::tuple_for_each(fixture.single_ranges,
            [](const auto& r)
            {
                for (auto i = r.first - 1; std::cmp_less_equal(i, r.second + 1);
                    ++i)
                {
                    CAPTURE(i);

                    const bool expected = (std::cmp_less(r.first, i) &&
                        std::cmp_less(i, r.second));
                    REQUIRE(caff::in_open_range(i, r.first, r.second) ==
                        expected);
                }
            });
    }

    // multi ranges
    {
        caff::tuple_for_each(fixture.multi_ranges,
            [](const auto& r)
            {
                for (auto i = r.first - 1; std::cmp_less_equal(i, r.second + 1);
                    ++i)
                {
                    CAPTURE(i);

                    const bool expected = (std::cmp_less(r.first, i) &&
                        std::cmp_less(i, r.second));
                    REQUIRE(caff::in_open_range(i, r.first, r.second) ==
                        expected);
                }
            });
    }
}
TEST_CASE_TEMPLATE_APPLY(in_open_range_test_id, caff::standard_integer_types);

TEST_CASE_TEMPLATE_DEFINE("in_half_open_range", TestType,
    in_half_open_range_test_id)
{
    using caff::in_half_open_range;

    in_range_fixture<TestType> fixture;

    // empty ranges
    {
        caff::tuple_for_each(fixture.empty_ranges,
            [](const auto& r)
            {
                for (auto i = r.first - 1; std::cmp_less_equal(i, r.second + 1);
                    ++i)
                {
                    CAPTURE(i);

                    {
                        const bool expected = (std::cmp_less_equal(r.first, i) &&
                            std::cmp_less(i, r.second));
                        REQUIRE(caff::in_half_open_range(i, r.first, r.second) ==
                            expected);
                    }
                }
            });
    }

    // single ranges
    {
        caff::tuple_for_each(fixture.single_ranges,
            [](const auto& r)
            {
                for (auto i = r.first - 1; std::cmp_less_equal(i, r.second + 1);
                    ++i)
                {
                    CAPTURE(i);

                    const bool expected = (std::cmp_less_equal(r.first, i) &&
                        std::cmp_less(i, r.second));
                    REQUIRE(caff::in_half_open_range(i, r.first, r.second) ==
                        expected);
                }
            });
    }

    // multi ranges
    {
        caff::tuple_for_each(fixture.multi_ranges,
            [](const auto& r)
            {
                for (auto i = r.first - 1; std::cmp_less_equal(i, r.second + 1);
                    ++i)
                {
                    CAPTURE(i);

                    const bool expected = (std::cmp_less_equal(r.first, i) &&
                        std::cmp_less(i, r.second));
                    REQUIRE(caff::in_half_open_range(i, r.first, r.second) ==
                        expected);
                }
            });
    }
}
TEST_CASE_TEMPLATE_APPLY(in_half_open_range_test_id,
    caff::standard_integer_types);

TEST_CASE_TEMPLATE_DEFINE("in_closed_range", TestType, in_closed_range_test_id)
{
    using caff::in_closed_range;

    in_range_fixture<TestType> fixture;

    // empty ranges
    {
        caff::tuple_for_each(fixture.empty_ranges,
            [](const auto& r)
            {
                for (auto i = r.first - 1; std::cmp_less_equal(i, r.second + 1);
                    ++i)
                {
                    CAPTURE(i);

                    {
                        const bool expected = (std::cmp_less_equal(r.first, i) &&
                            std::cmp_less_equal(i, r.second));
                        REQUIRE(caff::in_closed_range(i, r.first, r.second) ==
                            expected);
                    }
                }
            });
    }

    // single ranges
    {
        caff::tuple_for_each(fixture.single_ranges,
            [](const auto& r)
            {
                for (auto i = r.first - 1; std::cmp_less_equal(i, r.second + 1);
                    ++i)
                {
                    CAPTURE(i);

                    const bool expected = (std::cmp_less_equal(r.first, i) &&
                        std::cmp_less_equal(i, r.second));
                    REQUIRE(caff::in_closed_range(i, r.first, r.second) ==
                        expected);
                }
            });
    }

    // multi ranges
    {
        caff::tuple_for_each(fixture.multi_ranges,
            [](const auto& r)
            {
                for (auto i = r.first - 1; std::cmp_less_equal(i, r.second + 1);
                    ++i)
                {
                    CAPTURE(i);

                    const bool expected = (std::cmp_less_equal(r.first, i) &&
                        std::cmp_less_equal(i, r.second));
                    REQUIRE(caff::in_closed_range(i, r.first, r.second) ==
                        expected);
                }
            });
    }
}
TEST_CASE_TEMPLATE_APPLY(in_closed_range_test_id, caff::standard_integer_types);
