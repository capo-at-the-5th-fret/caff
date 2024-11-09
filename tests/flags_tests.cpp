#include <doctest/doctest.h>
#include <unordered_set>
#include <tuple>
#include <array>
#include "fixtures/flags_fixture.h"

TEST_SUITE("flags")
{
    using namespace caff::test::dummy;
    using enum option;

    TEST_CASE("default constructor")
    {
        options o;
        REQUIRE(o.to_underlying() == 0);
    }

    TEST_CASE("single value constructor")
    {
        options o{ read };
        REQUIRE(o.to_underlying() == std::to_underlying(read));
    }

    TEST_CASE_FIXTURE(options_fixture, "initializer list constructor")
    {
        options o{ base_option_list };
        REQUIRE(o == base);
    }

    TEST_CASE_FIXTURE(options_fixture, "copy constructor")
    {
        options o{ base };
        REQUIRE(o == base);
    }

    TEST_CASE_FIXTURE(options_fixture, "assignment operator")
    {
        SUBCASE("default")
        {
            options o;
            o = base;
            REQUIRE(o == base);
        }

        SUBCASE("assign from initializer_list")
        {
            options o;
            o = base_option_list;
            REQUIRE(o == base);
        }
    }

    TEST_CASE_FIXTURE(options_fixture, "comparison operators")
    {
        for (int i = 0; const auto& [rhs, expected] : comparisons)
        {
            CAPTURE(i);

            REQUIRE(expected.compare_equal(base, rhs));
            REQUIRE(expected.compare_not_equal(base, rhs));

            ++i;
        }
    }

    TEST_CASE("set")
    {
        options o;
        o.set(read, append);
        REQUIRE(o.to_underlying() == (std::to_underlying(read) | std::to_underlying(append)));
    }

    TEST_CASE("reset")
    {
        options o({read, write, append});
        o.reset(write);
        REQUIRE(o.to_underlying() == (std::to_underlying(read) | std::to_underlying(append)));

        o.reset();
        REQUIRE(o.to_underlying() == 0);
    }

    TEST_CASE("flip")
    {
        options o(read);
        o.flip(read, append);
        REQUIRE(o.to_underlying() == std::to_underlying(append));

        o.flip(write);
        REQUIRE(o.to_underlying() == (std::to_underlying(append) | std::to_underlying(write)));
    }

    TEST_CASE_FIXTURE(options_fixture, "test")
    {
        options o{ read };
        REQUIRE(o.test(read));
        REQUIRE_FALSE(o.test(write));
        REQUIRE_FALSE(o.test(append));
        REQUIRE_FALSE(o.test(trunc));
    }

    TEST_CASE("all, any, none")
    {
        constexpr std::array test_values =
        {
            std::tuple{ options{ }, false, false, true },
            std::tuple{ options{ read }, false, true, false },
            std::tuple{ options{ write }, false, true, false },
            std::tuple{ options{ append }, false, true, false },
            std::tuple{ options{ trunc }, false, true, false },
            std::tuple{ options{ read, write }, false, true, false },
            std::tuple{ options{ read, append }, false, true, false },
            std::tuple{ options{ read, trunc }, false, true, false },
            std::tuple{ options{ write, append }, false, true, false },
            std::tuple{ options{ write, trunc }, false, true, false },
            std::tuple{ options{ append, trunc }, false, true, false },
            std::tuple{ options{ read, write, append }, false, true, false },
            std::tuple{ options{ read, write, trunc }, false, true, false },
            std::tuple{ options{ read, append, trunc }, false, true, false },
            std::tuple{ options{ write, append, trunc }, false, true, false },
            std::tuple{ options{ read, write, append, trunc }, true, true, false }
        };

        for (int i = 0; const auto& [o, expected_all, expected_any, expected_none] :
            test_values)
        {
            CAPTURE(i);
            REQUIRE(o.all() == expected_all);
            REQUIRE(o.any() == expected_any);
            REQUIRE(o.none() == expected_none);
            i++;
        }
    }

    TEST_CASE("test_all_of")
    {
        options o({read, write, append});
        REQUIRE(o.test_all_of(read, write));
        REQUIRE_FALSE(o.test_all_of(read, trunc));
    }

    TEST_CASE("test_any_of")
    {
        options o({read, write});
        REQUIRE(o.test_any_of(read, append));
        REQUIRE_FALSE(o.test_any_of(trunc, append));
    }

    TEST_CASE("test_none_of")
    {
        options o(read);
        REQUIRE(o.test_none_of(write, append));
        REQUIRE_FALSE(o.test_none_of(read, append));
    }

    TEST_CASE_FIXTURE(options_fixture, "hash")
    {
        using underlying_type = typename options::underlying_type;
        using hash_type = std::hash<options>;

        std::hash<options> h;

        options o1{ base };
        options o2{ diff };

        SUBCASE("return type")
        {
            REQUIRE(std::is_same_v<decltype(hash_type{}(o1)), std::size_t>);
        }

        SUBCASE("value")
        {
            const auto h1 = h(o1);
            const auto h2 = std::hash<underlying_type>{}(o1.to_underlying());
            REQUIRE(h1 == h2);
        }

        SUBCASE("usage")
        {
            std::unordered_set<options> s;
            s.insert(o1);
            REQUIRE(s.size() == 1);
            auto pos1 = s.find(o1);
            REQUIRE(pos1 != s.end());
            REQUIRE(*pos1 == o1);

            s.insert(o2);
            REQUIRE(s.size() == 2);
            pos1 = s.find(o1);
            REQUIRE(pos1 != s.end());
            REQUIRE(*pos1 == o1);

            auto pos2 = s.find(o2);
            REQUIRE(pos2 != s.end());
            REQUIRE(*pos2 == o2);
        }
    }
}

#if 0
TEST_CASE("flags")
{
    using namespace caff::test::dummy;
    using enum option;
    using underlying_type = typename options::underlying_type;

    using fixture = options_fixture;

    SUBCASE("constructors")
    {
        SUBCASE("default")
        {
            options o;
            REQUIRE_FALSE(o.test(write));
            REQUIRE_FALSE(o.test(read));
            REQUIRE_FALSE(o.test(append));
            REQUIRE_FALSE(o.test(trunc));
        }

        SUBCASE("single value")
        {
            options o{ write };
            REQUIRE(o.test(write));
            REQUIRE_FALSE(o.test(read));
            REQUIRE_FALSE(o.test(append));
            REQUIRE_FALSE(o.test(trunc));
        }

        SUBCASE("initializer_list")
        {
            options o = { write, append };
            REQUIRE(o.test(write));
            REQUIRE_FALSE(o.test(read));
            REQUIRE(o.test(append));
            REQUIRE_FALSE(o.test(trunc));
        }

        SUBCASE("copy constructor")
        {
            options o{ base };
            REQUIRE(o == base);
        }
    }

    SUBCASE("assignment operator")
    {
        SUBCASE("assign from flags")
        {
            options o2{ write, append };
            o = o2;
            REQUIRE(o.test(write));
            REQUIRE(o.test(append));
            REQUIRE_FALSE(o.test(read));
            REQUIRE_FALSE(o.test(trunc));
        }

        SUBCASE("assign from initializer_list")
        {
            o = { write, append };
            REQUIRE(o.test(write));
            REQUIRE(o.test(append));
            REQUIRE_FALSE(o.test(read));
            REQUIRE_FALSE(o.test(trunc));
        }
    }

#if 0
    SUBCASE("comparison operators")
    {
        SUBCASE("equality")
        {

            options o2{ write, append };
            REQUIRE(o == o2);
        }
    }

    SUBCASE("set single bit at a time")
    {
        o.set(write);
        REQUIRE(o.test(write));
        REQUIRE_FALSE(o.test(read));
        REQUIRE_FALSE(o.test(append));
        REQUIRE_FALSE(o.test(trunc));

        o.set(append);
        REQUIRE(o.test(write));
        REQUIRE_FALSE(o.test(read));
        REQUIRE(o.test(append));
        REQUIRE_FALSE(o.test(trunc));
    }

#endif

#if 0
    SUBCASE("set multiple bits at a time")
    {
        o.set(write, append);
        REQUIRE(o.test(write));
        REQUIRE_FALSE(o.test(read));
        REQUIRE(o.test(append));
        REQUIRE_FALSE(o.test(trunc));
    }

    SUBCASE("reset single bit at a time")
    {
        o.set(write, append);
        o.reset(write);
        REQUIRE_FALSE(o.test(write));
        REQUIRE_FALSE(o.test(read));
        REQUIRE(o.test(append));
        REQUIRE_FALSE(o.test(trunc));

        o.reset(append);
        REQUIRE(o == none);
    }

    SUBCASE("reset multiple bits at a time")
    {
        set(o, write, append);
        reset(o, write, append);
        REQUIRE(o == none);
    }

    SUBCASE("reset all bits")
    {
        set(o, write, append);
        reset(o);
        REQUIRE(o == none);
    }

    SUBCASE("test single bit at a time")
    {
        set(o, write, append);
        REQUIRE(test(o, write));
        REQUIRE_FALSE(test(o, read));
        REQUIRE(test(o, append));
        REQUIRE_FALSE(test(o, trunc));
    }

    SUBCASE("test multiple bits at a time")
    {
        set(o, write, append);
        REQUIRE(test(o, write, append));
        REQUIRE_FALSE(test(o, write, read));
        REQUIRE_FALSE(test(o, write, trunc));
        REQUIRE_FALSE(test(o, read, write));
        REQUIRE_FALSE(test(o, trunc, write));
        REQUIRE_FALSE(test(o, append, read));
        REQUIRE_FALSE(test(o, append, trunc));
        REQUIRE_FALSE(test(o, read, append));
        REQUIRE_FALSE(test(o, trunc, append));
        REQUIRE_FALSE(test(o, read, trunc));
    }

    SUBCASE("flip single bit at a time")
    {
        flip(o, write);
        REQUIRE(test(o, write));
        REQUIRE_FALSE(test(o, read));
        REQUIRE_FALSE(test(o, append));
        REQUIRE_FALSE(test(o, trunc));

        flip(o, append);
        REQUIRE(test(o, write));
        REQUIRE_FALSE(test(o, read));
        REQUIRE(test(o, append));
        REQUIRE_FALSE(test(o, trunc));

        flip(o, write);
        REQUIRE_FALSE(test(o, write));
        REQUIRE_FALSE(test(o, read));
        REQUIRE(test(o, append));
        REQUIRE_FALSE(test(o, trunc));

        flip(o, append);
        REQUIRE(o == none);
    }

    SUBCASE("flip multiple bits at a time")
    {
        flip(o, write, append);
        REQUIRE(test(o, write));
        REQUIRE_FALSE(test(o, read));
        REQUIRE(test(o, append));
        REQUIRE_FALSE(test(o, trunc));

        flip(o, write, append);
        REQUIRE(o == none);
    }

    SUBCASE("test_xxx")
    {
        o.set(write, append);

        REQUIRE(o.test_all_of(write));
        REQUIRE(o.test_all_of(append));
        REQUIRE(o.test_all_of(write, append));
        REQUIRE_FALSE(o.test_all_of(write, append, read));

        REQUIRE(o.test_any_of(write));
        REQUIRE(o.test_any_of(append));
        REQUIRE_FALSE(o.test_any_of(read));

        REQUIRE(o.test_none_of(read));
        REQUIRE_FALSE(o.test_none_of(write));
        REQUIRE_FALSE(o.test_none_of(append));
    }
    #endif

    SUBCASE("hash")
    {
        using hash_type = std::hash<options>;

        std::hash<options> h;

        options o2{ write, append };

        SUBCASE("return type")
        {
            REQUIRE(std::is_same_v<decltype(hash_type{}(o)), std::size_t>);
        }

        SUBCASE("value")
        {
            const auto h1 = h(o);
            const auto h2 = std::hash<underlying_type>{}(o.to_underlying());
            REQUIRE(h1 == h2);

            const auto h3 = h(o2);
            const auto h4 = std::hash<underlying_type>{}(o2.to_underlying());
            REQUIRE(h3 == h4);
        }

        SUBCASE("usage")
        {
            std::unordered_set<options> s;
            s.insert(o);
            REQUIRE(s.size() == 1);
            REQUIRE(s.find(o) != s.end());

            s.insert(o2);
            REQUIRE(s.size() == 2);
            REQUIRE(s.find(o) != s.end());
            REQUIRE(s.find(o2) != s.end());
        }
    }
}

#include "fixture_header"

TEST_CASE_METHOD(fixture_namespace::test::flags_fixture, "test_case_name")
{
    using caff::test::dummy::options;

    SECTION("constructors")
    {
        SECTION("default constructor")
        {
            options o;
            REQUIRE(o.member_name == default_value);
        }

        SECTION("primary constructor")
        {
            options o{ base.member };
            REQUIRE(o == base);
        }

        SECTION("copy constructor")
        {
            options o{ base };
            REQUIRE(o == base);
        }
    }

    SECTION("operators")
    {
        SECTION("copy assignment operator")
        {
            options o;
            o = base;
            REQUIRE(o == base);
        }

    }

    // FIX: When string is constexpr
    #if 0
    SECTION("constexpr")
    {
        SECTION("default constructor")
        {
            constexpr author_type author;
            STATIC_REQUIRE(author.composer.empty());
            STATIC_REQUIRE(author.lyricist == false);
        }

        SECTION("primary constructor")
        {
            constexpr author_type author{ "composer", "lyricist" };
            STATIC_REQUIRE(author.composer == "composer");
            STATIC_REQUIRE(author.lyricist == "lyricist");
        }

        SECTION("comparison operators")
        {
            constexpr author_type lhs{ "composer", "lyricist" };
            constexpr author_type rhs{ "composer", "lyricist" };
            STATIC_REQUIRE(lhs == rhs);
            STATIC_REQUIRE_FALSE(lhs != rhs);
        }
    }
    #endif
}
#endif
