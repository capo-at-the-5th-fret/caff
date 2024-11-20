#include <catch2/catch_test_macros.hpp>
#include "caff/stdexcept.h"

#include <memory>
#include <string>
#include <array>
#include <vector>

#include "caff/format.h"

#if 0
TEST_CASE("argument_out_of_range", "[stdexcept][argument_out_of_range]")
{
    using caff::argument_out_of_range;

    SECTION("constructors")
    {
        SECTION("default constructor")
        {
            argument_out_of_range e;
            REQUIRE(e.message() == caff::argument_out_of_range_message);
        }

        SECTION("message constructor")
        {
            argument_out_of_range e{ "Custom Message" };
            REQUIRE(e.message() == "Custom Message");
        }
    }

    SECTION("make_argument_out_of_range")
    {
        SECTION("no arguments")
        {
            const auto e = caff::make_argument_out_of_range();
            REQUIRE(e.message() == caff::argument_out_of_range_message);
        }

        SECTION("message argument")
        {
            const auto e = caff::make_argument_out_of_range("Custom Message");
            REQUIRE(e.message() == "Custom Message");
        }
    }

    SECTION("make_argument_out_of_range_ex")
    {
        std::string exception_message_format =
            "{}\n[ diagnostic info ]\nvariable: testValue\nvalue: {}\n"
            "condition: {}";
        const int testValue = 12;
        const std::string testValue2 = "text";
        const auto conditionInfo = caff::make_closed_range_info(1,10);

        SECTION("message overload")
        {
            const std::string custom_message = "Custom Message";
            const auto exception_message = fmt::format(fmt::runtime(exception_message_format),
                custom_message, testValue, conditionInfo.value);
            const auto e = caff::make_argument_out_of_range_ex(custom_message,
                caff_variable(testValue), conditionInfo);
            REQUIRE(e.what() == exception_message);
        }

        SECTION("default")
        {
            const auto exception_message = fmt::format(fmt::runtime(exception_message_format),
                caff::argument_out_of_range_message, testValue,
                conditionInfo.value);
            const auto e = caff::make_argument_out_of_range_ex(
                caff_variable(testValue), conditionInfo);
            REQUIRE(e.what() == exception_message);
        }

        exception_message_format =
            "{}\n[ diagnostic info ]\nvariable: testValue\nvalue: {}\n"
            "variable: testValue2\nvalue: {}\ncondition: {}";

        SECTION("message overload, 2 variables")
        {
            const std::string custom_message = "Custom Message";
            const auto exception_message = fmt::format(fmt::runtime(exception_message_format)),
                custom_message, testValue, testValue2,
                conditionInfo.value);
            const auto e = caff::make_argument_out_of_range_ex(custom_message,
                caff_variable(testValue), caff_variable(testValue2),
                conditionInfo);
            REQUIRE(e.what() == exception_message);
        }

        SECTION("default, 2 variables")
        {
            const auto exception_message = fmt::format(fmt::runtime(exception_message_format)),
                caff::argument_out_of_range_message, testValue, testValue2,
                conditionInfo.value);
            const auto e = caff::make_argument_out_of_range_ex(
                caff_variable(testValue), caff_variable(testValue2),
                conditionInfo);
            REQUIRE(e.what() == exception_message);
        }
    }
}

TEST_CASE("domain_error")
{
    using caff::domain_error;

    SECTION("constructors")
    {
        SECTION("default constructor")
        {
            domain_error e;
            REQUIRE(e.message() == caff::domain_error_message);
        }

        SECTION("message constructor")
        {
            domain_error e{ "Custom Message" };
            REQUIRE(e.message() == "Custom Message");
        }
    }

    SECTION("make_domain_error")
    {
        SECTION("no arguments")
        {
            const auto e = caff::make_domain_error();
            REQUIRE(e.message() == caff::domain_error_message);
        }

        SECTION("message argument")
        {
            const auto e = caff::make_domain_error("Custom Message");
            REQUIRE(e.message() == "Custom Message");
        }
    }

    SECTION("make_domain_error_range_ex")
    {
        const std::string exception_message_format =
            "{}\n[ diagnostic info ]\nvariable: test_value\nvalue: {}\n"
            "condition: {}";
        const int test_value = 12;
        const auto condition_info = caff::make_closed_range_info(1,10);

        SECTION("message overload")
        {
            const std::string custom_message = "Custom Message";
            const auto exception_message = fmt::format(
                exception_message_format, custom_message, test_value,
                condition_info.value);
            const auto e = make_domain_error_ex(custom_message,
                caff_variable(test_value), condition_info);
            REQUIRE(e.what() == exception_message);
        }

        SECTION("default")
        {
            const auto exception_message = fmt::format(
                exception_message_format, caff::domain_error_message,
                test_value, condition_info.value);
            const auto e = make_domain_error_ex(caff_variable(test_value),
                condition_info);
            REQUIRE(e.what() == exception_message);
        }
    }
}

TEMPLATE_TEST_CASE("caff_ensure_pointer", "",
    int*,
    std::shared_ptr<int>,
    std::unique_ptr<int>
)
{
    SECTION("pointer argument")
    {
        const std::string exception_message_format =
            "{}\n[ diagnostic info ]\nvariable: {}\nvalue: null";
        const auto exception_message = fmt::format(fmt::runtime(exception_message_format)),
            caff::null_argument_message, "ptr");

        SECTION("valid pointer")
        {
            TestType ptr = []() -> TestType
            {
                if constexpr (std::is_same_v<TestType, int*>)
                {
                    static auto temp = std::make_unique<int>(10);
                    return temp.get();
                }
                else
                {
                    return TestType{ new int(10) };
                }
            }();

            bool ok = false;

            try
            {
                caff_ensure_arg(ptr);
                ok = true;
            }
            catch (...)
            {
            }

            REQUIRE(ok);
        }

        SECTION("null pointer")
        {
            TestType ptr{};

            bool ok = false;

            try
            {
                caff_ensure_arg(ptr);
            }
            catch (caff::null_argument const& e)
            {
                const std::string s = e.what();
                ok = (s.find(exception_message) == 0);
            }
            catch (...)
            {
            }

            REQUIRE(ok);
        }
    }

    SECTION("variable")
    {
        const std::string exception_message_format =
            "{}\n[ diagnostic info ]\nvariable: {}\nvalue: null";
        const auto exception_message = fmt::format(fmt::runtime(exception_message_format)),
            caff::null_pointer_message, "ptr");

        SECTION("valid pointer")
        {
            TestType ptr = []() -> TestType
            {
                if constexpr (std::is_same_v<TestType, int*>)
                {
                    static auto temp = std::make_unique<int>(10);
                    return temp.get();
                }
                else
                {
                    return TestType{ new int(10) };
                }
            }();

            bool ok = false;

            try
            {
                caff_ensure_ptr(ptr);
                ok = true;
            }
            catch (...)
            {
            }

            REQUIRE(ok);
        }

        SECTION("null pointer")
        {
            TestType ptr{};

            bool ok = false;

            try
            {
                caff_ensure_ptr(ptr);
            }
            catch (caff::null_pointer const& e)
            {
                const std::string s = e.what();
                ok = (s.find(exception_message) == 0);
            }
            catch (...)
            {
            }

            REQUIRE(ok);
        }
    }
}

TEST_CASE("caff_ensure_indices")
{
    const std::vector<std::vector<int>> vectors =
    {
        { },
        { 1 },
        { 1, 2, 3 }
    };

    const auto vindex = GENERATE(caff::test::range<std::size_t>(3));
    REQUIRE(vindex < 3);
    CAPTURE(vindex);

    auto const& v = vectors[vindex];

    const std::size_t low = 0;
    const std::size_t high = v.size();

    SECTION("index")
    {
        for (std::size_t i = low; i < (high + 1); ++i)
        {
            const bool expected = (i < high);//(i >= low && i < high);
            CAPTURE(i);

            bool ok = false;

            try
            {
                caff_ensure_index(v, i);
                ok = expected;
            }
            catch (caff::index_out_of_range const& e)
            {
                if (!expected)
                {
                    const std::string exception_message_format =
                        "{}\n[ diagnostic info ]\nvariable: {}\nvalue: {}\n"
                        "condition: >= {} && < {}";
                    const auto exception_message = fmt::format(
                        exception_message_format,
                        caff::index_out_of_range_message, "i", i, low, high);

                    const std::string s = e.what();
                    ok = (s.find(exception_message) == 0);
                }
            }
            catch (...)
            {
            }

            REQUIRE(ok);
        }
    }

    SECTION("closed_index")
    {
        for (std::size_t i = low; i < (high + 1); ++i)
        {
            const bool expected = (i <= high); //(i >= low && i <= high);
            CAPTURE(i);

            bool ok = false;

            try
            {
                caff_ensure_closed_index(v, i);
                ok = expected;
            }
            catch (caff::index_out_of_range const& e)
            {
                if (!expected)
                {
                    const std::string exception_message_format =
                        "{}\n[ diagnostic info ]\nvariable: {}\nvalue: {}\n"
                        "condition: >= {} && <= {}";
                    const auto exception_message = fmt::format(
                        exception_message_format,
                        caff::index_out_of_range_message, "i", i, low, high);

                    const std::string s = e.what();
                    ok = (s.find(exception_message) == 0);
                }
            }
            catch (...)
            {
            }

            REQUIRE(ok);
        }
    }

}

TEST_CASE("caff_ensure_zero related")
{
    const auto i = GENERATE(caff::test::closed_range(-1, 1));
    CAPTURE(i);

    SECTION("non_zero")
    {
        const bool expected = (i != 0);
        bool ok = false;

        try
        {
            caff_ensure_non_zero_arg(i);
            ok = expected;
        }
        catch (caff::argument_out_of_range const& e)
        {
            if (!expected)
            {
                std::string exception_message_format =
                    "{}\n[ diagnostic info ]\nvariable: {}\nvalue: {}\n"
                    "condition: != 0";
                auto exception_message = fmt::format(fmt::runtime(exception_message_format)),
                    caff::argument_out_of_range_message, "i", i);
                
                const std::string s = e.what();
                ok = (s.find(exception_message) == 0);
            }
        }
        catch (...)
        {
        }

        REQUIRE(ok);
    }

    SECTION("greater than zero")
    {
        const bool expected = (i > 0);
        bool ok = false;

        try
        {
            caff_ensure_greater_than_zero_arg(i);
            ok = expected;
        }
        catch (caff::argument_out_of_range const& e)
        {
            if (!expected)
            {
                std::string exception_message_format =
                    "{}\n[ diagnostic info ]\nvariable: {}\nvalue: {}\n"
                    "condition: > 0";
                auto exception_message = fmt::format(fmt::runtime(exception_message_format)),
                    caff::argument_out_of_range_message, "i", i);
                
                const std::string s = e.what();
                ok = (s.find(exception_message) == 0);
            }
        }
        catch (...)
        {
        }

        REQUIRE(ok);
    }
}

TEST_CASE("caff_ensure_in_ranges")
{
    const auto [low, high] = GENERATE(table<int,int>(
        {
            { -6, -6 },
            { -6, -3 },
            { -3, 0 },
            { 0, 0 },
            { 0, 3 },
            { 3, 6 },
            { 6, 6 },

            { -3, -6 },
            { 0, -3 },
            { 6, 3 }
        })
    );

    CAPTURE(low, high);

    SECTION("in_range")
    {
        for (auto i = (low - 1); i <= (high + 1); ++i)
        {
            const bool expected = (i >= low && i < high);
            CAPTURE(i);

            bool ok = false;

            try
            {
                caff_ensure_in_range_arg(i, low, high);
                ok = expected;
            }
            catch (caff::argument_out_of_range const& e)
            {
                if (!expected)
                {
                    const std::string exception_message_format =
                        "{}\n[ diagnostic info ]\nvariable: {}\nvalue: {}\n"
                        "condition: >= {} && < {}";
                    const auto exception_message = fmt::format(
                        exception_message_format,
                        caff::argument_out_of_range_message, "i", i, low, high);

                    const std::string s = e.what();
                    ok = (s.find(exception_message) == 0);
                }
            }
            catch (...)
            {
            }

            REQUIRE(ok);
        }
    }

    SECTION("in_closed_range")
    {
        for (auto i = (low - 1); i <= (high + 1); ++i)
        {
            const bool expected = (i >= low && i <= high);
            CAPTURE(i);

            bool ok = false;

            try
            {
                caff_ensure_in_closed_range_arg(i, low, high);
                ok = expected;
            }
            catch (caff::argument_out_of_range const& e)
            {
                if (!expected)
                {
                    const std::string exception_message_format =
                        "{}\n[ diagnostic info ]\nvariable: {}\nvalue: {}\n"
                        "condition: >= {} && <= {}";
                    const auto exception_message = fmt::format(
                        exception_message_format,
                        caff::argument_out_of_range_message, "i", i, low, high);

                    const std::string s = e.what();
                    ok = (s.find(exception_message) == 0);
                }
            }
            catch (...)
            {
            }

            REQUIRE(ok);
        }
    }
}

TEST_CASE("index_out_of_range")
{
    using caff::index_out_of_range;

    SECTION("constructors")
    {
        SECTION("default constructor")
        {
            index_out_of_range e;
            REQUIRE(e.message() == caff::index_out_of_range_message);
        }

        SECTION("message constructor")
        {
            index_out_of_range e{ "Custom Message" };
            REQUIRE(e.message() == "Custom Message");
        }
    }

    SECTION("make_index_out_of_range")
    {
        SECTION("no arguments")
        {
            const auto e = caff::make_index_out_of_range();
            REQUIRE(e.message() == caff::index_out_of_range_message);
        }

        SECTION("message argument")
        {
            const auto e = caff::make_index_out_of_range("Custom Message");
            REQUIRE(e.message() == "Custom Message");
        }
    }

    SECTION("make_index_out_of_range_ex")
    {
        std::string exception_message_format =
            "{}\n[ diagnostic info ]\nvariable: {}\nvalue: {}\ncondition: {}";
        const std::vector<int> test_array = { 1, 2, 3, 4, 5 };
        const int test_array2[] = { 1, 2, 3, 4 };
        const int test_value = 12;

        std::string condition;
        std::string exception_message;
        const std::string custom_message = "Custom Message";

        std::vector<std::pair<caff::range_type, std::string>> ranges =
        {
            { caff::range_type::closed, "" },
            { caff::range_type::closed, ">= {} && <= {}" },
            { caff::range_type::left_open, "> {} && <= {}" },
            { caff::range_type::right_open, ">= {} && < {}" },
            { caff::range_type::open, "> {} && < {}" },
        };

        const short low = -2;
        const int high = 10;

        int i = 0;
        for (auto const& range : ranges)
        {
            auto [range_type, format] = range;

            // SPECIAL CASE: Test default range type (closed)
            if (i == 0)
            {
                format = ranges[1].second;
            }

            condition = fmt::format(format, low, high);

            CAPTURE(i, range_type, format);

            // TEST CASE: make_index_out_of_range_ex(string_view,
            // variable const&, index type, index type, RangeType)
            {
                exception_message = fmt::format(fmt::runtime(exception_message_format)),
                    custom_message, caff_varname(test_value), test_value,
                    condition);

                index_out_of_range e;

                if (i == 0)
                {
                    e = caff::make_index_out_of_range_ex(custom_message,
                        caff_variable(test_value), low, high);
                }
                else
                {
                    e = caff::make_index_out_of_range_ex(custom_message,
                        caff_variable(test_value), low, high, range_type);
                }

                REQUIRE(e.what() == exception_message);
            }

            // TEST CASE: make_index_out_of_range_ex(variable const&, index type,
            // index type, RangeType)
            {
                exception_message = fmt::format(fmt::runtime(exception_message_format)),
                    caff::index_out_of_range_message, caff_varname(test_value),
                    test_value, condition);

                index_out_of_range e;

                if (i == 0)
                {
                    e = caff::make_index_out_of_range_ex(
                        caff_variable(test_value), low, high);
                }
                else
                {
                    e = caff::make_index_out_of_range_ex(
                        caff_variable(test_value), low, high, range_type);
                }

                REQUIRE(e.what() == exception_message);
            }

            // SPECIAL CASE: Test default range type (half open)
            if (i == 0)
            {
                format = ranges[3].second;
            }

            condition = fmt::format(format, 0, test_array.size());

            // TEST CASE: make_index_out_of_range_ex(CollectionType, message)
            {
                exception_message = fmt::format(fmt::runtime(exception_message_format)),
                    custom_message,
                    caff_varname(test_value),
                    test_value, condition);

                index_out_of_range e;

                if (i == 0)
                {
                    e = caff::make_index_out_of_range_ex(custom_message,
                        caff_variable(test_value), test_array);
                }
                else
                {
                    e = caff::make_index_out_of_range_ex(custom_message,
                        caff_variable(test_value), test_array, range_type);
                }

                REQUIRE(e.what() == exception_message);
            }

            // TEST CASE: make_index_out_of_range_ex(CollectionType)
            {
                exception_message = fmt::format(fmt::runtime(exception_message_format)),
                    caff::index_out_of_range_message, caff_varname(test_value),
                    test_value, condition);

                index_out_of_range e;

                if (i == 0)
                {
                    e = caff::make_index_out_of_range_ex(
                        caff_variable(test_value), test_array);
                }
                else
                {
                    e = caff::make_index_out_of_range_ex(
                        caff_variable(test_value), test_array, range_type);
                }

                REQUIRE(e.what() == exception_message);
            }
        }
    }
}

TEST_CASE("invalid_argument")
{
    using caff::invalid_argument;

    SECTION("constructors")
    {
        SECTION("default constructor")
        {
            invalid_argument e;
            REQUIRE(e.message() == caff::invalid_argument_message);
        }

        SECTION("message constructor")
        {
            invalid_argument e{ "Custom Message" };
            REQUIRE(e.message() == "Custom Message");
        }
    }

    SECTION("make_invalid_argument")
    {
        SECTION("no arguments")
        {
            const auto e = caff::make_invalid_argument();
            REQUIRE(e.message() == caff::invalid_argument_message);
        }

        SECTION("message overload")
        {
            const auto e = caff::make_invalid_argument("Custom Message");
            REQUIRE(e.message() == "Custom Message");
        }
    }

    SECTION("make_invalid_argument_ex")
    {
        const std::string exception_message_format =
            "{}\n[ diagnostic info ]\nvariable: test_value\nvalue: {}\n"
            "condition: {}";
        const int test_value = 12;
        const auto condition_info = 
            caff::make_closed_range_info(1,10);

        SECTION("message overload")
        {
            const std::string custom_message = "Custom Message";
            const auto exception_message = fmt::format(
                exception_message_format, custom_message, test_value,
                condition_info.value);
            const auto e = caff::make_invalid_argument_ex(custom_message,
                caff_variable(test_value), condition_info);
            REQUIRE(e.what() == exception_message);
        }

        SECTION("default")
        {
            const auto exception_message = fmt::format(
                exception_message_format, caff::invalid_argument_message,
                test_value, condition_info.value);
            const auto e = caff::make_invalid_argument_ex(
                caff_variable(test_value), condition_info);
            REQUIRE(e.what() == exception_message);
        }
    }
}

TEST_CASE("invalid_operation")
{
    using caff::invalid_operation;

    SECTION("constructors")
    {
        SECTION("default constructor")
        {
            invalid_operation e;
            REQUIRE(e.message() == caff::invalid_operation_message);
        }

        SECTION("message constructor")
        {
            invalid_operation e{ "Custom Message" };
            REQUIRE(e.message() == "Custom Message");
        }
    }

    SECTION("make_invalid_operation")
    {
        SECTION("no arguments")
        {
            const auto e = caff::make_invalid_operation();
            REQUIRE(e.message() == caff::invalid_operation_message);
        }

        SECTION("message argument")
        {
            const auto e = caff::make_invalid_operation("Custom Message");
            REQUIRE(e.message() == "Custom Message");
        }
    }
}

TEST_CASE("length_error")
{
    using caff::length_error;

    SECTION("constructors")
    {
        SECTION("default constructor")
        {
            length_error e;
            REQUIRE(e.message() == caff::length_error_message);
        }

        SECTION("message constructor")
        {
            length_error e{ "Custom Message" };
            REQUIRE(e.message() == "Custom Message");
        }
    }

    SECTION("make_length_error")
    {
        SECTION("no arguments")
        {
            const auto e = caff::make_length_error();
            REQUIRE(e.message() == caff::length_error_message);
        }

        SECTION("message argument")
        {
            const auto e = caff::make_length_error("Custom Message");
            REQUIRE(e.message() == "Custom Message");
        }
    }
}

TEST_CASE("logic_error")
{
    using caff::logic_error;

    SECTION("constructors")
    {
        SECTION("default constructor")
        {
            logic_error e;
            REQUIRE(e.message() == caff::logic_error_message);
        }

        SECTION("message constructor")
        {
            logic_error e{ "Custom Message" };
            REQUIRE(e.message() == "Custom Message");
        }
    }

    SECTION("make_logic_error")
    {
        SECTION("no arguments")
        {
            const auto e = caff::make_logic_error();
            REQUIRE(e.message() == caff::logic_error_message);
        }

        SECTION("message argument")
        {
            const auto e = caff::make_logic_error("Custom Message");
            REQUIRE(e.message() == "Custom Message");
        }
    }
}

TEST_CASE("null_argument")
{
    using caff::null_argument;

    SECTION("constructors")
    {
        SECTION("default constructor")
        {
            null_argument e;
            REQUIRE(e.message() == caff::null_argument_message);
        }

        SECTION("message constructor")
        {
            null_argument e{ "Custom Message" };
            REQUIRE(e.message() == "Custom Message");
        }
    }

    SECTION("make_null_argument")
    {
        SECTION("no arguments")
        {
            const auto e = caff::make_null_argument();
            REQUIRE(e.message() == caff::null_argument_message);
        }

        SECTION("message overload")
        {
            const auto e = caff::make_null_argument("Custom Message");
            REQUIRE(e.message() == "Custom Message");
        }
    }

    SECTION("make_null_argument_ex")
    {
        const std::string exception_message_format =
            "{}\n[ diagnostic info ]\nvariable: {}\nvalue: null";
        const std::string test_value = "variable";

        SECTION("message overload")
        {
            const std::string custom_message = "Custom message";
            const auto exception_message = fmt::format(
                exception_message_format, custom_message, test_value);
            const auto e = caff::make_null_argument_ex(custom_message,
                test_value);
            REQUIRE(e.what() == exception_message);
        }

        SECTION("default")
        {
            const auto exception_message = fmt::format(
                exception_message_format, caff::null_argument_message,
                test_value);
            const auto e = caff::make_null_argument_ex(test_value);
            REQUIRE(e.what() == exception_message);
        }
    }
}

TEST_CASE("null_pointer")
{
    using caff::null_pointer;

    SECTION("constructors")
    {
        SECTION("default constructor")
        {
            null_pointer e;
            REQUIRE(e.message() == caff::null_pointer_message);
        }

        SECTION("message constructor")
        {
            null_pointer e{ "Custom Message" };
            REQUIRE(e.message() == "Custom Message");
        }
    }

    SECTION("make_null_pointer")
    {
        SECTION("no arguments")
        {
            const auto e = caff::make_null_pointer();
            REQUIRE(e.message() == caff::null_pointer_message);
        }

        SECTION("message overload")
        {
            const auto e = caff::make_null_pointer("Custom Message");
            REQUIRE(e.message() == "Custom Message");
        }
    }

    SECTION("make_null_pointer_ex")
    {
        const std::string exception_message_format =
            "{}\n[ diagnostic info ]\nvariable: {}\nvalue: null";
        const std::string test_value = "variable";

        SECTION("message overload")
        {
            const std::string custom_message = "Custom Message";
            const auto exception_message = fmt::format(
                exception_message_format, custom_message, test_value);
            const auto e = caff::make_null_pointer_ex(custom_message,
                test_value);
            REQUIRE(e.what() == exception_message);
        }

        SECTION("default")
        {
            const auto exception_message = fmt::format(
                exception_message_format, caff::null_pointer_message,
                test_value);
            const auto e = caff::make_null_pointer_ex(test_value);
            REQUIRE(e.what() == exception_message);
        }
    }
}

TEST_CASE("out_of_range")
{
    using caff::out_of_range;

    SECTION("constructors")
    {
        SECTION("default constructor")
        {
            out_of_range e;
            REQUIRE(e.message() == caff::out_of_range_message);
        }

        SECTION("message constructor")
        {
            out_of_range e{ "Custom Message" };
            REQUIRE(e.message() == "Custom Message");
        }
    }

    SECTION("make_out_of_range")
    {
        SECTION("no arguments")
        {
            const auto e = caff::make_out_of_range();
            REQUIRE(e.message() == caff::out_of_range_message);
        }

        SECTION("message argument")
        {
            const auto e = caff::make_out_of_range("Custom Message");
            REQUIRE(e.message() == "Custom Message");
        }
    }
}

TEST_CASE("runtime_error")
{
    using caff::runtime_error;

    SECTION("constructors")
    {
        SECTION("default constructor")
        {
            runtime_error e;
            REQUIRE(e.message() == caff::runtime_error_message);
        }

        SECTION("message constructor")
        {
            runtime_error e{ "Custom Message" };
            REQUIRE(e.message() == "Custom Message");
        }
    }

    SECTION("make_runtime_error")
    {
        SECTION("no arguments")
        {
            const auto e = caff::make_runtime_error();
            REQUIRE(e.message() == caff::runtime_error_message);
        }

        SECTION("message argument")
        {
            const auto e = caff::make_runtime_error("Custom Message");
            REQUIRE(e.message() == "Custom Message");
        }
    }
}

TEST_CASE("format_error")
{
    using caff::format_error;

    SECTION("constructors")
    {
        SECTION("default constructor")
        {
            format_error e;
            REQUIRE(e.message() == caff::format_error_message);
        }

        SECTION("message constructor")
        {
            format_error e{ "Custom Message" };
            REQUIRE(e.message() == "Custom Message");
        }
    }

    SECTION("make_format_error")
    {
        SECTION("no arguments")
        {
            const auto e = caff::make_format_error();
            REQUIRE(e.message() == caff::format_error_message);
        }

        SECTION("message argument")
        {
            const auto e = caff::make_format_error("Custom Message");
            REQUIRE(e.message() == "Custom Message");
        }
    }

    SECTION("make_format_error_ex")
    {
        using namespace std::string_view_literals;

        const std::string exception_message_format =
            "{}\n[ diagnostic info ]\ncondition: {}";
        const auto test_condition = caff::make_set_condition(
            std::array{"a"sv, "b"sv, "c"sv});

        SECTION("message overload")
        {
            const std::string custom_message = "Custom message";
            const auto exception_message = fmt::format(
                exception_message_format, custom_message, test_condition.value);
            const auto e = caff::make_format_error_ex(custom_message,
                test_condition);
            REQUIRE(e.what() == exception_message);
        }

        SECTION("default")
        {
            const auto exception_message = fmt::format(
                exception_message_format, caff::format_error_message,
                test_condition.value);
            const auto e = caff::make_format_error_ex(test_condition);
            REQUIRE(e.what() == exception_message);
        }
    }
}

TEST_CASE("unsupported_operation")
{
    using caff::unsupported_operation;

    SECTION("constructors")
    {
        SECTION("default constructor")
        {
            unsupported_operation e;
            REQUIRE(e.message() == caff::unsupported_operation_message);
        }

        SECTION("message constructor")
        {
            unsupported_operation e{ "Custom Message" };
            REQUIRE(e.message() == "Custom Message");
        }
    }

    SECTION("make_unsupported_operation")
    {
        SECTION("no arguments")
        {
            const auto e = caff::make_unsupported_operation();
            REQUIRE(e.message() == caff::unsupported_operation_message);
        }

        SECTION("message argument")
        {
            const auto e = caff::make_unsupported_operation("Custom Message");
            REQUIRE(e.message() == "Custom Message");
        }
    }
}

TEST_CASE("value_out_of_range")
{
    using caff::value_out_of_range;

    SECTION("constructors")
    {
        SECTION("default constructor")
        {
            value_out_of_range e;
            REQUIRE(e.message() == caff::value_out_of_range_message);
        }

        SECTION("message constructor")
        {
            value_out_of_range e{ "Custom Message" };
            REQUIRE(e.message() == "Custom Message");
        }
    }

    SECTION("make_value_out_of_range")
    {
        SECTION("no arguments")
        {
            const auto e = caff::make_value_out_of_range();
            REQUIRE(e.message() == caff::value_out_of_range_message);
        }

        SECTION("message argument")
        {
            const auto e = caff::make_value_out_of_range("Custom Message");
            REQUIRE(e.message() == "Custom Message");
        }
    }

    SECTION("make_value_out_of_range_ex")
    {
        std::string exception_message_format =
            "{}\n[ diagnostic info ]\nvariable: test_value\nvalue: {}\n"
            "condition: {}";
        const int test_value = 12;
        const auto condition_info =
            caff::make_closed_range_info(1,10);

        SECTION("message overload")
        {
            const std::string custom_message = "Custom Message";
            const auto exception_message = fmt::format(fmt::runtime(exception_message_format)),
                custom_message, test_value, condition_info.value);
            const auto e = caff::make_value_out_of_range_ex(custom_message,
                caff_variable(test_value), condition_info);
            REQUIRE(e.what() == exception_message);
        }

        SECTION("default")
        {
            const auto exception_message = fmt::format(fmt::runtime(exception_message_format)),
                caff::value_out_of_range_message, test_value,
                condition_info.value);
            const auto e = caff::make_value_out_of_range_ex(
                caff_variable(test_value), condition_info);
            REQUIRE(e.what() == exception_message);
        }

        exception_message_format =
            "{}\n[ diagnostic info ]\nvariable: test_value\nvalue: {}\n"
            "variable: testValue2\nvalue: {}\ncondition: {}";

        std::string testValue2 = "text";

        SECTION("message overload, 2 variables")
        {
            const std::string custom_message = "Custom Message";
            const auto exception_message = fmt::format(fmt::runtime(exception_message_format)),
                custom_message, test_value, testValue2, condition_info.value);
            const auto e = caff::make_value_out_of_range_ex(custom_message,
                caff_variable(test_value), caff_variable(testValue2),
                condition_info);
            REQUIRE(e.what() == exception_message);
        }

        SECTION("default, 2 variables")
        {
            const auto exception_message = fmt::format(fmt::runtime(exception_message_format)),
                caff::value_out_of_range_message, test_value, testValue2,
                condition_info.value);
            const auto e = caff::make_value_out_of_range_ex(
                caff_variable(test_value), caff_variable(testValue2),
                condition_info);
            REQUIRE(e.what() == exception_message);
        }
    }
}
#endif

#if 0
TEST_SUITE("caff::argument_out_of_range")
{
    using caff::argument_out_of_range;

    TEST_CASE("constructors")
    {
        SUBCASE("default constructor")
        {
            argument_out_of_range e;
            REQUIRE(e.message() == std::string{ caff::argument_out_of_range_message });
        }

        SUBCASE("message constructor")
        {
            argument_out_of_range e{ "Custom Message" };
            REQUIRE(e.message() == "Custom Message");
        }
    }
}

TEST_SUITE("caff::make_argument_out_of_range")
{
    TEST_CASE("no arguments")
    {
        const auto e = caff::make_argument_out_of_range();
        REQUIRE(e.message() == std::string{ caff::argument_out_of_range_message });
    }

    TEST_CASE("message argument")
    {
        const auto e = caff::make_argument_out_of_range("Custom Message");
        REQUIRE(e.message() == "Custom Message");
    }
}

TEST_SUITE("caff::make_argument_out_of_range_ex")
{
    const int testValue = 12;
    const std::string testValue2 = "text";
    const auto conditionInfo = caff::make_closed_range_info(1,10);

    TEST_CASE("single variable")
    {
        const std::string_view exception_message_format =
            "{}\n[ diagnostic info ]\nvariable: testValue\nvalue: {}\n"
            "condition: {}";

        SUBCASE("message overload")
        {
            const std::string custom_message = "Custom Message";
            const auto exception_message = fmt::format(fmt::runtime(exception_message_format),
                custom_message, testValue, conditionInfo.value);
            const auto e = caff::make_argument_out_of_range_ex(custom_message,
                caff_variable(testValue), conditionInfo);
            REQUIRE(e.what() == exception_message);
        }

        SUBCASE("default")
        {
            const auto exception_message = fmt::format(fmt::runtime(exception_message_format),
                caff::argument_out_of_range_message, testValue,
                conditionInfo.value);
            const auto e = caff::make_argument_out_of_range_ex(
                caff_variable(testValue), conditionInfo);
            REQUIRE(e.what() == exception_message);
        }
    }

    TEST_CASE("multiple variables")
    {
        const std::string_view exception_message_format =
            "{}\n[ diagnostic info ]\nvariable: testValue\nvalue: {}\n"
            "variable: testValue2\nvalue: {}\ncondition: {}";

        SUBCASE("message overload, 2 variables")
        {
            const std::string custom_message = "Custom Message";
            const auto exception_message = fmt::format(fmt::runtime(exception_message_format),
                custom_message, testValue, testValue2,
                conditionInfo.value);
            const auto e = caff::make_argument_out_of_range_ex(custom_message,
                caff_variable(testValue), caff_variable(testValue2),
                conditionInfo);
            REQUIRE(e.what() == exception_message);
        }

        SUBCASE("default, 2 variables")
        {
            const auto exception_message = fmt::format(fmt::runtime(exception_message_format),
                caff::argument_out_of_range_message, testValue, testValue2,
                conditionInfo.value);
            const auto e = caff::make_argument_out_of_range_ex(
                caff_variable(testValue), caff_variable(testValue2),
                conditionInfo);
            REQUIRE(e.what() == exception_message);
        }
    }
}

TEST_SUITE("caff::domain_error")
{
    using caff::domain_error;

    TEST_CASE("constructors")
    {
        SUBCASE("default constructor")
        {
            domain_error e;
            REQUIRE(e.message() == std::string{ caff::domain_error_message });
        }

        SUBCASE("message constructor")
        {
            domain_error e{ "Custom Message" };
            REQUIRE(e.message() == "Custom Message");
        }
    }

    TEST_CASE("make_domain_error")
    {
        SUBCASE("no arguments")
        {
            const auto e = caff::make_domain_error();
            REQUIRE(e.message() == std::string{ caff::domain_error_message });
        }

        SUBCASE("message argument")
        {
            const auto e = caff::make_domain_error("Custom Message");
            REQUIRE(e.message() == "Custom Message");
        }
    }

    TEST_CASE("make_domain_error_range_ex")
    {
        const std::string exception_message_format =
            "{}\n[ diagnostic info ]\nvariable: test_value\nvalue: {}\n"
            "condition: {}";
        const int test_value = 12;
        const auto condition_info = caff::make_closed_range_info(1,10);

        SUBCASE("message overload")
        {
            const std::string custom_message = "Custom Message";
            const auto exception_message = fmt::format(
                fmt::runtime(exception_message_format), custom_message,
                test_value, condition_info.value);
            const auto e = make_domain_error_ex(custom_message,
                caff_variable(test_value), condition_info);
            REQUIRE(e.what() == exception_message);
        }

        SUBCASE("default")
        {
            const auto exception_message = fmt::format(
                fmt::runtime(exception_message_format),
                caff::domain_error_message,
                test_value, condition_info.value);
            const auto e = make_domain_error_ex(caff_variable(test_value),
                condition_info);
            REQUIRE(e.what() == exception_message);
        }
    }
}

TEST_SUITE("caff::ensure_pointer")
{
    TEST_CASE_TEMPLATE("template types", TestType,
        int*, std::unique_ptr<int>, std::shared_ptr<int>)
    {
        SUBCASE("pointer argument")
        {
            const std::string exception_message_format =
                "{}\n[ diagnostic info ]\nvariable: {}\nvalue: null";
            const auto exception_message = fmt::format(fmt::runtime(exception_message_format),
                caff::null_argument_message, "ptr");

            SUBCASE("valid pointer")
            {
                TestType ptr = []() -> TestType
                {
                    if constexpr (std::is_same_v<TestType, int*>)
                    {
                        static auto temp = std::make_unique<int>(10);
                        return temp.get();
                    }
                    else
                    {
                        return TestType{ new int(10) };
                    }
                }();

                bool ok = false;

                try
                {
                    caff_ensure_arg(ptr);
                    ok = true;
                }
                catch (...)
                {
                }

                REQUIRE(ok);
            }

            SUBCASE("null pointer")
            {
                TestType ptr{};

                bool ok = false;

                try
                {
                    caff_ensure_arg(ptr);
                }
                catch (caff::null_argument const& e)
                {
                    const std::string s = e.what();
                    ok = (s.find(exception_message) == 0);
                }
                catch (...)
                {
                }

                REQUIRE(ok);
            }
        }

        SUBCASE("variable")
        {
            const std::string exception_message_format =
                "{}\n[ diagnostic info ]\nvariable: {}\nvalue: null";
            const auto exception_message = fmt::format(
                fmt::runtime(exception_message_format),
                caff::null_pointer_message, "ptr");

            SUBCASE("valid pointer")
            {
                TestType ptr = []() -> TestType
                {
                    if constexpr (std::is_same_v<TestType, int*>)
                    {
                        static auto temp = std::make_unique<int>(10);
                        return temp.get();
                    }
                    else
                    {
                        return TestType{ new int(10) };
                    }
                }();

                bool ok = false;

                try
                {
                    caff_ensure_ptr(ptr);
                    ok = true;
                }
                catch (...)
                {
                }

                REQUIRE(ok);
            }

            SUBCASE("null pointer")
            {
                TestType ptr{};

                bool ok = false;

                try
                {
                    caff_ensure_ptr(ptr);
                }
                catch (caff::null_pointer const& e)
                {
                    const std::string s = e.what();
                    ok = (s.find(exception_message) == 0);
                }
                catch (...)
                {
                }

                REQUIRE(ok);
            }
        }
    }
}

#if 0
TEST_CASE("caff_ensure_indices")
{
    const std::vector<std::vector<int>> vectors =
    {
        { },
        { 1 },
        { 1, 2, 3 }
    };

    const auto vindex = GENERATE(caff::test::range<std::size_t>(3));
    REQUIRE(vindex < 3);
    CAPTURE(vindex);

    auto const& v = vectors[vindex];

    const std::size_t low = 0;
    const std::size_t high = v.size();

    SECTION("index")
    {
        for (std::size_t i = low; i < (high + 1); ++i)
        {
            const bool expected = (i < high);//(i >= low && i < high);
            CAPTURE(i);

            bool ok = false;

            try
            {
                caff_ensure_index(v, i);
                ok = expected;
            }
            catch (caff::index_out_of_range const& e)
            {
                if (!expected)
                {
                    const std::string exception_message_format =
                        "{}\n[ diagnostic info ]\nvariable: {}\nvalue: {}\n"
                        "condition: >= {} && < {}";
                    const auto exception_message = fmt::format(
                        exception_message_format,
                        caff::index_out_of_range_message, "i", i, low, high);

                    const std::string s = e.what();
                    ok = (s.find(exception_message) == 0);
                }
            }
            catch (...)
            {
            }

            REQUIRE(ok);
        }
    }

    SECTION("closed_index")
    {
        for (std::size_t i = low; i < (high + 1); ++i)
        {
            const bool expected = (i <= high); //(i >= low && i <= high);
            CAPTURE(i);

            bool ok = false;

            try
            {
                caff_ensure_closed_index(v, i);
                ok = expected;
            }
            catch (caff::index_out_of_range const& e)
            {
                if (!expected)
                {
                    const std::string exception_message_format =
                        "{}\n[ diagnostic info ]\nvariable: {}\nvalue: {}\n"
                        "condition: >= {} && <= {}";
                    const auto exception_message = fmt::format(
                        exception_message_format,
                        caff::index_out_of_range_message, "i", i, low, high);

                    const std::string s = e.what();
                    ok = (s.find(exception_message) == 0);
                }
            }
            catch (...)
            {
            }

            REQUIRE(ok);
        }
    }

}

TEST_CASE("caff_ensure_zero related")
{
    const auto i = GENERATE(caff::test::closed_range(-1, 1));
    CAPTURE(i);

    SECTION("non_zero")
    {
        const bool expected = (i != 0);
        bool ok = false;

        try
        {
            caff_ensure_non_zero_arg(i);
            ok = expected;
        }
        catch (caff::argument_out_of_range const& e)
        {
            if (!expected)
            {
                std::string exception_message_format =
                    "{}\n[ diagnostic info ]\nvariable: {}\nvalue: {}\n"
                    "condition: != 0";
                auto exception_message = fmt::format(exception_message_format,
                    caff::argument_out_of_range_message, "i", i);
                
                const std::string s = e.what();
                ok = (s.find(exception_message) == 0);
            }
        }
        catch (...)
        {
        }

        REQUIRE(ok);
    }

    SECTION("greater than zero")
    {
        const bool expected = (i > 0);
        bool ok = false;

        try
        {
            caff_ensure_greater_than_zero_arg(i);
            ok = expected;
        }
        catch (caff::argument_out_of_range const& e)
        {
            if (!expected)
            {
                std::string exception_message_format =
                    "{}\n[ diagnostic info ]\nvariable: {}\nvalue: {}\n"
                    "condition: > 0";
                auto exception_message = fmt::format(exception_message_format,
                    caff::argument_out_of_range_message, "i", i);
                
                const std::string s = e.what();
                ok = (s.find(exception_message) == 0);
            }
        }
        catch (...)
        {
        }

        REQUIRE(ok);
    }
}

TEST_CASE("caff_ensure_in_ranges")
{
    const auto [low, high] = GENERATE(table<int,int>(
        {
            { -6, -6 },
            { -6, -3 },
            { -3, 0 },
            { 0, 0 },
            { 0, 3 },
            { 3, 6 },
            { 6, 6 },

            { -3, -6 },
            { 0, -3 },
            { 6, 3 }
        })
    );

    CAPTURE(low, high);

    SECTION("in_range")
    {
        for (auto i = (low - 1); i <= (high + 1); ++i)
        {
            const bool expected = (i >= low && i < high);
            CAPTURE(i);

            bool ok = false;

            try
            {
                caff_ensure_in_range_arg(i, low, high);
                ok = expected;
            }
            catch (caff::argument_out_of_range const& e)
            {
                if (!expected)
                {
                    const std::string exception_message_format =
                        "{}\n[ diagnostic info ]\nvariable: {}\nvalue: {}\n"
                        "condition: >= {} && < {}";
                    const auto exception_message = fmt::format(
                        exception_message_format,
                        caff::argument_out_of_range_message, "i", i, low, high);

                    const std::string s = e.what();
                    ok = (s.find(exception_message) == 0);
                }
            }
            catch (...)
            {
            }

            REQUIRE(ok);
        }
    }

    SECTION("in_closed_range")
    {
        for (auto i = (low - 1); i <= (high + 1); ++i)
        {
            const bool expected = (i >= low && i <= high);
            CAPTURE(i);

            bool ok = false;

            try
            {
                caff_ensure_in_closed_range_arg(i, low, high);
                ok = expected;
            }
            catch (caff::argument_out_of_range const& e)
            {
                if (!expected)
                {
                    const std::string exception_message_format =
                        "{}\n[ diagnostic info ]\nvariable: {}\nvalue: {}\n"
                        "condition: >= {} && <= {}";
                    const auto exception_message = fmt::format(
                        exception_message_format,
                        caff::argument_out_of_range_message, "i", i, low, high);

                    const std::string s = e.what();
                    ok = (s.find(exception_message) == 0);
                }
            }
            catch (...)
            {
            }

            REQUIRE(ok);
        }
    }
}
#endif

TEST_SUITE("caff::index_out_of_range")
{
    using caff::index_out_of_range;

    TEST_CASE("constructors")
    {
        SUBCASE("default constructor")
        {
            index_out_of_range e;
            REQUIRE(e.message() == std::string{ caff::index_out_of_range_message });
        }

        SUBCASE("message constructor")
        {
            index_out_of_range e{ "Custom Message" };
            REQUIRE(e.message() == "Custom Message");
        }
    }

    TEST_CASE("make_index_out_of_range")
    {
        SUBCASE("no arguments")
        {
            const auto e = caff::make_index_out_of_range();
            REQUIRE(e.message() == std::string{ caff::index_out_of_range_message });
        }

        SUBCASE("message argument")
        {
            const auto e = caff::make_index_out_of_range("Custom Message");
            REQUIRE(e.message() == "Custom Message");
        }
    }

    TEST_CASE("make_index_out_of_range_ex")
    {
        std::string exception_message_format =
            "{}\n[ diagnostic info ]\nvariable: {}\nvalue: {}\ncondition: {}";
        const std::vector<int> test_array = { 1, 2, 3, 4, 5 };
        const int test_array2[] = { 1, 2, 3, 4 };
        const int test_value = 12;

        std::string condition;
        std::string exception_message;
        const std::string custom_message = "Custom Message";

        std::vector<std::pair<caff::range_type, std::string>> ranges =
        {
            { caff::range_type::closed, "" },
            { caff::range_type::closed, ">= {} && <= {}" },
            { caff::range_type::left_open, "> {} && <= {}" },
            { caff::range_type::right_open, ">= {} && < {}" },
            { caff::range_type::open, "> {} && < {}" },
        };

        const short low = -2;
        const int high = 10;

        int i = 0;
        for (auto const& range : ranges)
        {
            auto [range_type, format] = range;

            // SPECIAL CASE: Test default range type (closed)
            if (i == 0)
            {
                format = ranges[1].second;
            }

            condition = fmt::format(fmt::runtime(format), low, high);

            //CAPTURE(i, range_type, format);
            CAPTURE(i);

            // TEST CASE: make_index_out_of_range_ex(string_view,
            // variable const&, index type, index type, RangeType)
            {
                exception_message = fmt::format(
                    fmt::runtime(exception_message_format),
                    custom_message, caff_varname(test_value), test_value,
                    condition);

                index_out_of_range e;

                if (i == 0)
                {
                    e = caff::make_index_out_of_range_ex(custom_message,
                        caff_variable(test_value), low, high);
                }
                else
                {
                    e = caff::make_index_out_of_range_ex(custom_message,
                        caff_variable(test_value), low, high, range_type);
                }

                REQUIRE(e.what() == exception_message);
            }

            // TEST CASE: make_index_out_of_range_ex(variable const&, index type,
            // index type, RangeType)
            {
                exception_message = fmt::format(
                    fmt::runtime(exception_message_format),
                    caff::index_out_of_range_message, caff_varname(test_value),
                    test_value, condition);

                index_out_of_range e;

                if (i == 0)
                {
                    e = caff::make_index_out_of_range_ex(
                        caff_variable(test_value), low, high);
                }
                else
                {
                    e = caff::make_index_out_of_range_ex(
                        caff_variable(test_value), low, high, range_type);
                }

                REQUIRE(e.what() == exception_message);
            }

            // SPECIAL CASE: Test default range type (half open)
            if (i == 0)
            {
                format = ranges[3].second;
            }

            condition = fmt::format(fmt::runtime(format), 0, test_array.size());

            // TEST CASE: make_index_out_of_range_ex(CollectionType, message)
            {
                exception_message = fmt::format(
                    fmt::runtime(exception_message_format),
                    custom_message,
                    caff_varname(test_value),
                    test_value, condition);

                index_out_of_range e;

                if (i == 0)
                {
                    e = caff::make_index_out_of_range_ex(custom_message,
                        caff_variable(test_value), test_array);
                }
                else
                {
                    e = caff::make_index_out_of_range_ex(custom_message,
                        caff_variable(test_value), test_array, range_type);
                }

                REQUIRE(e.what() == exception_message);
            }

            // TEST CASE: make_index_out_of_range_ex(CollectionType)
            {
                exception_message = fmt::format(
                    fmt::runtime(exception_message_format),
                    caff::index_out_of_range_message, caff_varname(test_value),
                    test_value, condition);

                index_out_of_range e;

                if (i == 0)
                {
                    e = caff::make_index_out_of_range_ex(
                        caff_variable(test_value), test_array);
                }
                else
                {
                    e = caff::make_index_out_of_range_ex(
                        caff_variable(test_value), test_array, range_type);
                }

                REQUIRE(e.what() == exception_message);
            }
        }
    }
}

TEST_SUITE("caff::invalid_argument")
{
    using caff::invalid_argument;

    TEST_CASE("constructors")
    {
        SUBCASE("default constructor")
        {
            invalid_argument e;
            REQUIRE(e.message() == std::string{ caff::invalid_argument_message });
        }

        SUBCASE("message constructor")
        {
            invalid_argument e{ "Custom Message" };
            REQUIRE(e.message() == "Custom Message");
        }
    }

    TEST_CASE("make_invalid_argument")
    {
        SUBCASE("no arguments")
        {
            const auto e = caff::make_invalid_argument();
            REQUIRE(e.message() == std::string{ caff::invalid_argument_message });
        }

        SUBCASE("message overload")
        {
            const auto e = caff::make_invalid_argument("Custom Message");
            REQUIRE(e.message() == "Custom Message");
        }
    }

    TEST_CASE("make_invalid_argument_ex")
    {
        const std::string exception_message_format =
            "{}\n[ diagnostic info ]\nvariable: test_value\nvalue: {}\n"
            "condition: {}";
        const int test_value = 12;
        const auto condition_info = 
            caff::make_closed_range_info(1,10);

        SUBCASE("message overload")
        {
            const std::string custom_message = "Custom Message";
            const auto exception_message = fmt::format(
                fmt::runtime(exception_message_format),
                custom_message,
                test_value,
                condition_info.value);
            const auto e = caff::make_invalid_argument_ex(custom_message,
                caff_variable(test_value), condition_info);
            REQUIRE(e.what() == exception_message);
        }

        SUBCASE("default")
        {
            const auto exception_message = fmt::format(
                fmt::runtime(exception_message_format),
                caff::invalid_argument_message,
                test_value, condition_info.value);
            const auto e = caff::make_invalid_argument_ex(
                caff_variable(test_value), condition_info);
            REQUIRE(e.what() == exception_message);
        }
    }
}

TEST_SUITE("caff::invalid_operation")
{
    using caff::invalid_operation;

    TEST_CASE("constructors")
    {
        SUBCASE("default constructor")
        {
            invalid_operation e;
            REQUIRE(e.message() == std::string{ caff::invalid_operation_message });
        }

        SUBCASE("message constructor")
        {
            invalid_operation e{ "Custom Message" };
            REQUIRE(e.message() == "Custom Message");
        }
    }

    TEST_CASE("make_invalid_operation")
    {
        SUBCASE("no arguments")
        {
            const auto e = caff::make_invalid_operation();
            REQUIRE(e.message() == std::string{ caff::invalid_operation_message });
        }

        SUBCASE("message argument")
        {
            const auto e = caff::make_invalid_operation("Custom Message");
            REQUIRE(e.message() == "Custom Message");
        }
    }
}

TEST_SUITE("caff::length_error")
{
    using caff::length_error;

    TEST_CASE("constructors")
    {
        SUBCASE("default constructor")
        {
            length_error e;
            REQUIRE(e.message() == std::string{ caff::length_error_message });
        }

        SUBCASE("message constructor")
        {
            length_error e{ "Custom Message" };
            REQUIRE(e.message() == "Custom Message");
        }
    }

    TEST_CASE("make_length_error")
    {
        SUBCASE("no arguments")
        {
            const auto e = caff::make_length_error();
            REQUIRE(e.message() == std::string{ caff::length_error_message });
        }

        SUBCASE("message argument")
        {
            const auto e = caff::make_length_error("Custom Message");
            REQUIRE(e.message() == "Custom Message");
        }
    }
}

TEST_SUITE("caff::logic_error")
{
    using caff::logic_error;

    TEST_CASE("constructors")
    {
        SUBCASE("default constructor")
        {
            logic_error e;
            REQUIRE(e.message() == std::string{ caff::logic_error_message });
        }

        SUBCASE("message constructor")
        {
            logic_error e{ "Custom Message" };
            REQUIRE(e.message() == "Custom Message");
        }
    }

    TEST_CASE("make_logic_error")
    {
        SUBCASE("no arguments")
        {
            const auto e = caff::make_logic_error();
            REQUIRE(e.message() == std::string{ caff::logic_error_message });
        }

        SUBCASE("message argument")
        {
            const auto e = caff::make_logic_error("Custom Message");
            REQUIRE(e.message() == "Custom Message");
        }
    }
}

TEST_SUITE("caff::null_argument")
{
    using caff::null_argument;

    TEST_CASE("constructors")
    {
        SUBCASE("default constructor")
        {
            null_argument e;
            REQUIRE(e.message() == std::string{ caff::null_argument_message });
        }

        SUBCASE("message constructor")
        {
            null_argument e{ "Custom Message" };
            REQUIRE(e.message() == "Custom Message");
        }
    }

    TEST_CASE("make_null_argument")
    {
        SUBCASE("no arguments")
        {
            const auto e = caff::make_null_argument();
            REQUIRE(e.message() == std::string{ caff::null_argument_message });
        }

        SUBCASE("message overload")
        {
            const auto e = caff::make_null_argument("Custom Message");
            REQUIRE(e.message() == "Custom Message");
        }
    }

    TEST_CASE("make_null_argument_ex")
    {
        const std::string exception_message_format =
            "{}\n[ diagnostic info ]\nvariable: {}\nvalue: null";
        const std::string test_value = "variable";

        SUBCASE("message overload")
        {
            const std::string custom_message = "Custom message";
            const auto exception_message = fmt::format(
                fmt::runtime(exception_message_format), custom_message, test_value);
            const auto e = caff::make_null_argument_ex(custom_message,
                test_value);
            REQUIRE(e.what() == exception_message);
        }

        SUBCASE("default")
        {
            const auto exception_message = fmt::format(
                fmt::runtime(exception_message_format), caff::null_argument_message,
                test_value);
            const auto e = caff::make_null_argument_ex(test_value);
            REQUIRE(e.what() == exception_message);
        }
    }
}

TEST_SUITE("caff::null_pointer")
{
    using caff::null_pointer;

    TEST_CASE("constructors")
    {
        SUBCASE("default constructor")
        {
            null_pointer e;
            REQUIRE(e.message() == std::string{ caff::null_pointer_message });
        }

        SUBCASE("message constructor")
        {
            null_pointer e{ "Custom Message" };
            REQUIRE(e.message() == "Custom Message");
        }
    }

    TEST_CASE("make_null_pointer")
    {
        SUBCASE("no arguments")
        {
            const auto e = caff::make_null_pointer();
            REQUIRE(e.message() == std::string{ caff::null_pointer_message });
        }

        SUBCASE("message overload")
        {
            const auto e = caff::make_null_pointer("Custom Message");
            REQUIRE(e.message() == "Custom Message");
        }
    }

    TEST_CASE("make_null_pointer_ex")
    {
        const std::string exception_message_format =
            "{}\n[ diagnostic info ]\nvariable: {}\nvalue: null";
        const std::string test_value = "variable";

        SUBCASE("message overload")
        {
            const std::string custom_message = "Custom Message";
            const auto exception_message = fmt::format(
                fmt::runtime(exception_message_format), custom_message, test_value);
            const auto e = caff::make_null_pointer_ex(custom_message,
                test_value);
            REQUIRE(e.what() == exception_message);
        }

        SUBCASE("default")
        {
            const auto exception_message = fmt::format(
                fmt::runtime(exception_message_format), caff::null_pointer_message,
                test_value);
            const auto e = caff::make_null_pointer_ex(test_value);
            REQUIRE(e.what() == exception_message);
        }
    }
}

TEST_SUITE("caff::out_of_range")
{
    using caff::out_of_range;

    TEST_CASE("constructors")
    {
        SUBCASE("default constructor")
        {
            out_of_range e;
            REQUIRE(e.message() == std::string{ caff::out_of_range_message });
        }

        SUBCASE("message constructor")
        {
            out_of_range e{ "Custom Message" };
            REQUIRE(e.message() == "Custom Message");
        }
    }

    TEST_CASE("make_out_of_range")
    {
        SUBCASE("no arguments")
        {
            const auto e = caff::make_out_of_range();
            REQUIRE(e.message() == std::string{ caff::out_of_range_message });
        }

        SUBCASE("message argument")
        {
            const auto e = caff::make_out_of_range("Custom Message");
            REQUIRE(e.message() == "Custom Message");
        }
    }
}

TEST_SUITE("caff::runtime_error")
{
    using caff::runtime_error;

    TEST_CASE("constructors")
    {
        SUBCASE("default constructor")
        {
            runtime_error e;
            REQUIRE(e.message() == std::string{ caff::runtime_error_message });
        }

        SUBCASE("message constructor")
        {
            runtime_error e{ "Custom Message" };
            REQUIRE(e.message() == "Custom Message");
        }
    }

    TEST_CASE("make_runtime_error")
    {
        SUBCASE("no arguments")
        {
            const auto e = caff::make_runtime_error();
            REQUIRE(e.message() == std::string{ caff::runtime_error_message });
        }

        SUBCASE("message argument")
        {
            const auto e = caff::make_runtime_error("Custom Message");
            REQUIRE(e.message() == "Custom Message");
        }
    }
}

TEST_SUITE("caff::format_error")
{
    using caff::format_error;

    TEST_CASE("constructors")
    {
        SUBCASE("default constructor")
        {
            format_error e;
            REQUIRE(e.message() == std::string{ caff::format_error_message });
        }

        SUBCASE("message constructor")
        {
            format_error e{ "Custom Message" };
            REQUIRE(e.message() == "Custom Message");
        }
    }

    TEST_CASE("make_format_error")
    {
        SUBCASE("no arguments")
        {
            const auto e = caff::make_format_error();
            REQUIRE(e.message() == std::string{ caff::format_error_message });
        }

        SUBCASE("message argument")
        {
            const auto e = caff::make_format_error("Custom Message");
            REQUIRE(e.message() == "Custom Message");
        }
    }

    TEST_CASE("make_format_error_ex")
    {
        using namespace std::string_view_literals;

        const std::string exception_message_format =
            "{}\n[ diagnostic info ]\ncondition: {}";
        const auto test_condition = caff::make_set_condition(
            std::array{"a"sv, "b"sv, "c"sv});

        SUBCASE("message overload")
        {
            const std::string custom_message = "Custom message";
            const auto exception_message = fmt::format(
                fmt::runtime(exception_message_format), custom_message,
                test_condition.value);
            const auto e = caff::make_format_error_ex(custom_message,
                test_condition);
            REQUIRE(e.what() == exception_message);
        }

        SUBCASE("default")
        {
            const auto exception_message = fmt::format(
                fmt::runtime(exception_message_format),
                caff::format_error_message,
                test_condition.value);
            const auto e = caff::make_format_error_ex(test_condition);
            REQUIRE(e.what() == exception_message);
        }
    }
}

TEST_SUITE("caff::unsupported_operation")
{
    using caff::unsupported_operation;

    TEST_CASE("constructors")
    {
        SUBCASE("default constructor")
        {
            unsupported_operation e;
            REQUIRE(e.message() == std::string{ caff::unsupported_operation_message });
        }

        SUBCASE("message constructor")
        {
            unsupported_operation e{ "Custom Message" };
            REQUIRE(e.message() == "Custom Message");
        }
    }

    TEST_CASE("make_unsupported_operation")
    {
        SUBCASE("no arguments")
        {
            const auto e = caff::make_unsupported_operation();
            REQUIRE(e.message() == std::string{ caff::unsupported_operation_message });
        }

        SUBCASE("message argument")
        {
            const auto e = caff::make_unsupported_operation("Custom Message");
            REQUIRE(e.message() == "Custom Message");
        }
    }
}

TEST_SUITE("caff::value_out_of_range")
{
    using caff::value_out_of_range;

    TEST_CASE("constructors")
    {
        SUBCASE("default constructor")
        {
            value_out_of_range e;
            REQUIRE(e.message() == std::string{ caff::value_out_of_range_message });
        }

        SUBCASE("message constructor")
        {
            value_out_of_range e{ "Custom Message" };
            REQUIRE(e.message() == "Custom Message");
        }
    }

    TEST_CASE("make_value_out_of_range")
    {
        SUBCASE("no arguments")
        {
            const auto e = caff::make_value_out_of_range();
            REQUIRE(e.message() == std::string{ caff::value_out_of_range_message });
        }

        SUBCASE("message argument")
        {
            const auto e = caff::make_value_out_of_range("Custom Message");
            REQUIRE(e.message() == "Custom Message");
        }
    }

    TEST_CASE("make_value_out_of_range_ex")
    {
        std::string exception_message_format =
            "{}\n[ diagnostic info ]\nvariable: test_value\nvalue: {}\n"
            "condition: {}";
        const int test_value = 12;
        const auto condition_info =
            caff::make_closed_range_info(1,10);

        SUBCASE("message overload")
        {
            const std::string custom_message = "Custom Message";
            const auto exception_message = fmt::format(fmt::runtime(exception_message_format),
                custom_message, test_value, condition_info.value);
            const auto e = caff::make_value_out_of_range_ex(custom_message,
                caff_variable(test_value), condition_info);
            REQUIRE(e.what() == exception_message);
        }

        SUBCASE("default")
        {
            const auto exception_message = fmt::format(fmt::runtime(exception_message_format),
                caff::value_out_of_range_message, test_value,
                condition_info.value);
            const auto e = caff::make_value_out_of_range_ex(
                caff_variable(test_value), condition_info);
            REQUIRE(e.what() == exception_message);
        }

        exception_message_format =
            "{}\n[ diagnostic info ]\nvariable: test_value\nvalue: {}\n"
            "variable: testValue2\nvalue: {}\ncondition: {}";

        std::string testValue2 = "text";

        SUBCASE("message overload, 2 variables")
        {
            const std::string custom_message = "Custom Message";
            const auto exception_message = fmt::format(fmt::runtime(exception_message_format),
                custom_message, test_value, testValue2, condition_info.value);
            const auto e = caff::make_value_out_of_range_ex(custom_message,
                caff_variable(test_value), caff_variable(testValue2),
                condition_info);
            REQUIRE(e.what() == exception_message);
        }

        SUBCASE("default, 2 variables")
        {
            const auto exception_message = fmt::format(fmt::runtime(exception_message_format),
                caff::value_out_of_range_message, test_value, testValue2,
                condition_info.value);
            const auto e = caff::make_value_out_of_range_ex(
                caff_variable(test_value), caff_variable(testValue2),
                condition_info);
            REQUIRE(e.what() == exception_message);
        }
    }
}
#endif
