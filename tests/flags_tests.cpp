#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_range.hpp>
#include <unordered_set>
#include <tuple>
#include <array>
#include "fixtures/flags_fixture.h"

using namespace caff::test::dummy;

TEST_CASE("default constructor", "[flags]")
{
    options o;
    CHECK(o.to_underlying() == 0);
}

TEST_CASE("single value constructor", "[flags]")
{
    using enum option;

    SECTION("initialize to none")
    {
        options o{ none };
        CHECK(o.to_underlying() == 0);
    }

    SECTION("initialize to non-none value")
    {
        options o{ read };
        CHECK(o.to_underlying() == std::to_underlying(read));
    }
}

TEST_CASE_METHOD(options_fixture, "initializer list constructor", "[flags]")
{
    options o{ base_option_list };
    REQUIRE(o == base);
}

TEST_CASE_METHOD(options_fixture, "copy constructor", "[flags]")
{
    options o{ base };
    REQUIRE(o == base);
}

TEST_CASE_METHOD(options_fixture, "assignment operator", "[flags]")
{
    SECTION("default")
    {
        options o;
        o = base;
        CHECK(o == base);
    }

    SECTION("assign from initializer_list")
    {
        options o;
        o = base_option_list;
        CHECK(o == base);
    }
}

TEST_CASE_METHOD(options_fixture, "comparison operators", "[flags]")
{
    const auto& [rhs, expected] = GENERATE_REF(from_range(comparisons));

    CHECK(expected.compare_equal(base, rhs));
    CHECK(expected.compare_not_equal(base, rhs));
}

TEST_CASE("set", "[flags]")
{
    using enum option;

    options o;
    o.set(read).set(append);
    CHECK(o.to_underlying() == (std::to_underlying(read) | std::to_underlying(append)));

    o.set(trunc, true);
    CHECK(o.to_underlying() == (std::to_underlying(read) | std::to_underlying(append) |
        std::to_underlying(trunc)));
    
    o.set(read, false);
    CHECK(o.to_underlying() == (std::to_underlying(append) | std::to_underlying(trunc)));

    o.set(append, false);
    CHECK(o.to_underlying() == std::to_underlying(trunc));

    o.set(trunc, false);
    CHECK(o.to_underlying() == 0);

    SECTION("chaining")
    {
        o.reset();
        o.set(read).set(append).set(trunc);
        CHECK(o.to_underlying() == (std::to_underlying(read) | std::to_underlying(append) |
            std::to_underlying(trunc)));
    }
}

TEST_CASE("reset", "[flags]")
{
    using enum option;

    options o{ read, write, append };

    SECTION("no parameters")
    {
        o.reset();
        CHECK(o.to_underlying() == 0);
    }

    SECTION("single parameter")
    {
        o.reset(write);
        CHECK(o.to_underlying() == (std::to_underlying(read) | std::to_underlying(append)));

        o.reset(read);
        CHECK(o.to_underlying() == std::to_underlying(append));

        o.reset(append);
        CHECK(o.to_underlying() == 0);
    }

    SECTION("chaining")
    {
        o.reset(read).reset(write).reset(append);
        CHECK(o.to_underlying() == 0);
    }
}

TEST_CASE("flip", "[flags]")
{
    using enum option;

    SECTION("no parameters")
    {
        constexpr std::array test_values =
        {
            std::tuple{ options{ }, options{ read, write, append, trunc } },
            std::tuple{ options{ read }, options{ write, append, trunc } },
            std::tuple{ options{ write }, options{ read, append, trunc } },
            std::tuple{ options{ append }, options{ read, write, trunc } },
            std::tuple{ options{ trunc }, options{ read, write, append } },
            std::tuple{ options{ read, write }, options{ append, trunc } },
            std::tuple{ options{ read, append }, options{ write, trunc } },
            std::tuple{ options{ read, trunc }, options{ write, append } },
            std::tuple{ options{ write, append }, options{ read, trunc } },
            std::tuple{ options{ write, trunc }, options{ read, append } },
            std::tuple{ options{ append, trunc }, options{ read, write } },
            std::tuple{ options{ read, write, append }, options{ trunc } },
            std::tuple{ options{ read, write, trunc }, options{ append } },
            std::tuple{ options{ read, append, trunc }, options{ write } },
            std::tuple{ options{ write, append, trunc }, options{ read } },
            std::tuple{ options{ read, write, append, trunc }, options{ } }
        };

        for (int i = 0; const auto& [test_value, expected_result] : test_values)
        {
            CAPTURE(i);
            options o{ test_value };
            o.flip();
            CHECK(o == expected_result);
            ++i;
        }
    }

    SECTION("single parameter")
    {
        options o;

        o.flip(read);
        CHECK(o.to_underlying() == std::to_underlying(read));

        o.flip(write);
        CHECK(o.to_underlying() == (std::to_underlying(read) | std::to_underlying(write)));

        o.flip(append);
        CHECK(o.to_underlying() == (std::to_underlying(read) | std::to_underlying(write) |    
            std::to_underlying(append)));

        o.flip(trunc);
        CHECK(o.to_underlying() == (std::to_underlying(read) | std::to_underlying(write) |    
            std::to_underlying(append) | std::to_underlying(trunc)));

        o.flip(read);
        CHECK(o.to_underlying() == (std::to_underlying(write) | std::to_underlying(append) |
            std::to_underlying(trunc)));

        o.flip(write);
        CHECK(o.to_underlying() == (std::to_underlying(append) | std::to_underlying(trunc)));

        o.flip(append);
        CHECK(o.to_underlying() == std::to_underlying(trunc));

        o.flip(trunc);
        CHECK(o.to_underlying() == 0);
    }

    SECTION("chaining")
    {
        options o;
        o.flip(read).flip(read);
        CHECK(o.to_underlying() == 0);

        o.flip(read).flip(write).flip(append).flip(trunc);
        CHECK(o.to_underlying() == (std::to_underlying(read) |
            std::to_underlying(write) | std::to_underlying(append) |
            std::to_underlying(trunc)));
    }
}

TEST_CASE("test", "[flags]")
{
    using enum option;

    options o;
    CHECK(o.test(none));
    CHECK_FALSE(o.test(read));
    CHECK_FALSE(o.test(write));
    CHECK_FALSE(o.test(append));
    CHECK_FALSE(o.test(trunc));

    o.set(read);
    CHECK_FALSE(o.test(none));
    CHECK(o.test(read));
    CHECK_FALSE(o.test(write));
    CHECK_FALSE(o.test(append));
    CHECK_FALSE(o.test(trunc));

    o.set(write);
    CHECK_FALSE(o.test(none));
    CHECK(o.test(read));
    CHECK(o.test(write));
    CHECK_FALSE(o.test(append));
    CHECK_FALSE(o.test(trunc));

    o.set(append);
    CHECK_FALSE(o.test(none));
    CHECK(o.test(read));
    CHECK(o.test(write));
    CHECK(o.test(append));
    CHECK_FALSE(o.test(trunc));

    o.set(trunc);
    CHECK_FALSE(o.test(none));
    CHECK(o.test(read));
    CHECK(o.test(write));
    CHECK(o.test(append));
    CHECK(o.test(trunc));
}

TEST_CASE("all, any, none, count", "[flags]")
{
    using enum option;

    constexpr std::array test_values =
    {
        std::tuple{ options{ }, false, false, true, 0 },
        std::tuple{ options{ read }, false, true, false, 1 },
        std::tuple{ options{ write }, false, true, false, 1 },
        std::tuple{ options{ append }, false, true, false, 1 },
        std::tuple{ options{ trunc }, false, true, false, 1 },
        std::tuple{ options{ read, write }, false, true, false, 2 },
        std::tuple{ options{ read, append }, false, true, false, 2 },
        std::tuple{ options{ read, trunc }, false, true, false, 2 },
        std::tuple{ options{ write, append }, false, true, false, 2 },
        std::tuple{ options{ write, trunc }, false, true, false, 2 },
        std::tuple{ options{ append, trunc }, false, true, false, 2 },
        std::tuple{ options{ read, write, append }, false, true, false, 3 },
        std::tuple{ options{ read, write, trunc }, false, true, false, 3 },
        std::tuple{ options{ read, append, trunc }, false, true, false, 3 },
        std::tuple{ options{ write, append, trunc }, false, true, false, 3 },
        std::tuple{ options{ read, write, append, trunc }, true, true, false, 4 }
    };

    for (int i = 0; const auto& [o, expected_all, expected_any, expected_none, expected_count] :
        test_values)
    {
        CAPTURE(i);
        CHECK(o.all() == expected_all);
        CHECK(o.any() == expected_any);
        CHECK(o.none() == expected_none);
        CHECK(o.count() == expected_count);
        i++;
    }
}

TEST_CASE("bitwise assignment operators", "[flags]")
{
    using enum option;

    SECTION("default")
    {
        SECTION("AND")
        {
            options o1{ read, write };
            options o2{ write, append };
            o1 &= o2;
            REQUIRE(o1 == options{ write });
        }

        SECTION("OR")
        {
            options o1{ read, write };
            options o2{ write, append };
            o1 |= o2;
            REQUIRE(o1 == options{ read, write, append });
        }

        SECTION("XOR")
        {
            options o1{ read, write };
            options o2{ write, append };
            o1 ^= o2;
            REQUIRE(o1 == options{ read, append });
        }

        SECTION("NOT")
        {
            options o1{ read, write };
            options o2 = ~o1;
            REQUIRE(o2 == options{ append, trunc });
        }
    }

    SECTION("enumerator value")
    {
        SECTION("AND")
        {
            options o1{ read, write };
            o1 &= write;
            REQUIRE(o1 == options{ write });

            o1 &= append;
            REQUIRE(o1 == options{ });
        }

        SECTION("OR")
        {
            options o1{ read, write };
            o1 |= write;
            REQUIRE(o1 == options{ read, write });

            o1 |= append;
            REQUIRE(o1 == options{ read, write, append });
        }

        SECTION("XOR")
        {
            options o1{ read, write };
            o1 ^= write;
            REQUIRE(o1 == options{ read });

            o1 ^= append;
            REQUIRE(o1 == options{ read, append });
        }
    }
}

TEST_CASE("test_all_of", "[flags]")
{
    using enum option;

    options o{ read, write, append };
    REQUIRE(o.test_all_of(read, write));
    REQUIRE_FALSE(o.test_all_of(read, trunc));
}

TEST_CASE("test_any_of", "[flags]")
{
    using enum option;

    options o{ read, write };
    REQUIRE(o.test_any_of(read, append));
    REQUIRE_FALSE(o.test_any_of(trunc, append));
}

TEST_CASE("test_none_of", "[flags]")
{
    using enum option;

    options o{ read };
    REQUIRE(o.test_none_of(write, append));
    REQUIRE_FALSE(o.test_none_of(read, append));
}

TEST_CASE("to_underlying", "[flags]")
{
    using enum option;

    options o;
    REQUIRE(o.to_underlying() == 0);

    o.set(read);
    REQUIRE(o.to_underlying() == std::to_underlying(read));
}

TEST_CASE("bitwise operators", "[flags]")
{
    using enum option;

    SECTION("default")
    {
        SECTION("AND")
        {
            options o1{ read, write };
            options o2{ write, append };
            options o3 = o1 & o2;
            REQUIRE(o3 == options{ write });
        }

        SECTION("OR")
        {
            options o1{ read, write };
            options o2{ write, append };
            options o3 = o1 | o2;
            REQUIRE(o3 == options{ read, write, append });
        }

        SECTION("XOR")
        {
            options o1{ read, write };
            options o2{ write, append };
            options o3 = o1 ^ o2;
            REQUIRE(o3 == options{ read, append });
        }
    }

    SECTION("enumerator value")
    {
        SECTION("AND")
        {
            options o1{ read, write };
            options o2 = o1 & write;
            REQUIRE(o2 == options{ write });

            o2 = o1 & append;
            REQUIRE(o2 == options{ });
        }

        SECTION("OR")
        {
            options o1{ read, write };
            options o2 = o1 | write | append;
            REQUIRE(o2 == options{ read, write, append });
        }

        SECTION("XOR")
        {
            options o1{ read, write };
            options o2 = o1 ^ write ^ append;
            REQUIRE(o2 == options{ read, append });
        }
    }
}

TEST_CASE_METHOD(options_fixture, "hash", "[flags]")
{
    using underlying_type = typename options::underlying_type;
    using hash_type = std::hash<options>;

    std::hash<options> h;

    options o1{ base };
    options o2{ diff };

    SECTION("return type")
    {
        CHECK(std::is_same_v<decltype(hash_type{}(o1)), std::size_t>);
    }

    SECTION("value")
    {
        const auto h1 = h(o1);
        const auto h2 = std::hash<underlying_type>{}(o1.to_underlying());
        CHECK(h1 == h2);
    }

    SECTION("usage")
    {
        std::unordered_set<options> s;
        s.insert(o1);
        REQUIRE(s.size() == 1);
        auto pos1 = s.find(o1);
        REQUIRE(pos1 != s.end());
        CHECK(*pos1 == o1);

        s.insert(o2);
        REQUIRE(s.size() == 2);
        pos1 = s.find(o1);
        REQUIRE(pos1 != s.end());
        CHECK(*pos1 == o1);

        auto pos2 = s.find(o2);
        REQUIRE(pos2 != s.end());
        CHECK(*pos2 == o2);
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
