#include <catch2/catch_test_macros.hpp>
#include "caff/test/type_list.h"

TEST_CASE("primary_type", "[type_list]")
{
    using T = caff::test::primary_types;

    static_assert(std::tuple_size_v<T> == 31);
    static_assert(std::is_same_v<std::tuple_element_t<0,T>, void>);
    static_assert(std::is_same_v<std::tuple_element_t<1,T>, std::nullptr_t>);
    static_assert(std::is_same_v<std::tuple_element_t<2,T>, bool>);
    static_assert(std::is_same_v<std::tuple_element_t<3,T>, char>);
    static_assert(std::is_same_v<std::tuple_element_t<4,T>, wchar_t>);
    static_assert(std::is_same_v<std::tuple_element_t<5,T>, char8_t>);
    static_assert(std::is_same_v<std::tuple_element_t<6,T>, char16_t>);
    static_assert(std::is_same_v<std::tuple_element_t<7,T>, char32_t>);
    static_assert(std::is_same_v<std::tuple_element_t<8,T>, signed char>);
    static_assert(std::is_same_v<std::tuple_element_t<9,T>, short>);
    static_assert(std::is_same_v<std::tuple_element_t<10,T>, int>);
    static_assert(std::is_same_v<std::tuple_element_t<11,T>, long>);
    static_assert(std::is_same_v<std::tuple_element_t<12,T>, long long>);
    static_assert(std::is_same_v<std::tuple_element_t<13,T>, unsigned char>);
    static_assert(std::is_same_v<std::tuple_element_t<14,T>, unsigned short>);
    static_assert(std::is_same_v<std::tuple_element_t<15,T>, unsigned int>);
    static_assert(std::is_same_v<std::tuple_element_t<16,T>, unsigned long>);
    static_assert(std::is_same_v<std::tuple_element_t<17,T>, unsigned long long>);
    static_assert(std::is_same_v<std::tuple_element_t<18,T>, float>);
    static_assert(std::is_same_v<std::tuple_element_t<19,T>, double>);
    static_assert(std::is_same_v<std::tuple_element_t<20,T>, long double>);
    static_assert(std::is_same_v<std::tuple_element_t<21,T>, int[]>);
    static_assert(std::is_same_v<std::tuple_element_t<22,T>, caff::test::dummy_enum>);
    static_assert(std::is_same_v<std::tuple_element_t<23,T>, caff::test::dummy_union>);
    static_assert(std::is_same_v<std::tuple_element_t<24,T>, caff::test::dummy_class>);
    static_assert(std::is_same_v<std::tuple_element_t<25,T>, decltype(caff::test::dummy_function)>);
    static_assert(std::is_same_v<std::tuple_element_t<26,T>, int*>);
    static_assert(std::is_same_v<std::tuple_element_t<27,T>, int&>);
    static_assert(std::is_same_v<std::tuple_element_t<28,T>, int&&>);
    static_assert(std::is_same_v<std::tuple_element_t<29,T>, decltype(&caff::test::dummy_class::member_variable)>);
    static_assert(std::is_same_v<std::tuple_element_t<30,T>, decltype(&caff::test::dummy_class::member_function)>);

    static_assert(std::tuple_size_v<T> == 31);
    static_assert(std::is_same_v<std::tuple_element_t<0,T>, void>);
    static_assert(std::is_same_v<std::tuple_element_t<1,T>, std::nullptr_t>);
    static_assert(std::is_same_v<std::tuple_element_t<2,T>, bool>);
    static_assert(std::is_same_v<std::tuple_element_t<3,T>, char>);
    static_assert(std::is_same_v<std::tuple_element_t<4,T>, wchar_t>);
    static_assert(std::is_same_v<std::tuple_element_t<5,T>, char8_t>);
    static_assert(std::is_same_v<std::tuple_element_t<6,T>, char16_t>);
    static_assert(std::is_same_v<std::tuple_element_t<7,T>, char32_t>);
    static_assert(std::is_same_v<std::tuple_element_t<8,T>, signed char>);
    static_assert(std::is_same_v<std::tuple_element_t<9,T>, short>);
    static_assert(std::is_same_v<std::tuple_element_t<10,T>, int>);
    static_assert(std::is_same_v<std::tuple_element_t<11,T>, long>);
    static_assert(std::is_same_v<std::tuple_element_t<12,T>, long long>);
    static_assert(std::is_same_v<std::tuple_element_t<13,T>, unsigned char>);
    static_assert(std::is_same_v<std::tuple_element_t<14,T>, unsigned short>);
    static_assert(std::is_same_v<std::tuple_element_t<15,T>, unsigned int>);
    static_assert(std::is_same_v<std::tuple_element_t<16,T>, unsigned long>);
    static_assert(std::is_same_v<std::tuple_element_t<17,T>, unsigned long long>);
    static_assert(std::is_same_v<std::tuple_element_t<18,T>, float>);
    static_assert(std::is_same_v<std::tuple_element_t<19,T>, double>);
    static_assert(std::is_same_v<std::tuple_element_t<20,T>, long double>);
    static_assert(std::is_same_v<std::tuple_element_t<21,T>, int[]>);
    static_assert(std::is_same_v<std::tuple_element_t<22,T>, caff::test::dummy_enum>);
    static_assert(std::is_same_v<std::tuple_element_t<23,T>, caff::test::dummy_union>);
    static_assert(std::is_same_v<std::tuple_element_t<24,T>, caff::test::dummy_class>);
    static_assert(std::is_same_v<std::tuple_element_t<25,T>, decltype(caff::test::dummy_function)>);
    static_assert(std::is_same_v<std::tuple_element_t<26,T>, int*>);
    static_assert(std::is_same_v<std::tuple_element_t<27,T>, int&>);
    static_assert(std::is_same_v<std::tuple_element_t<28,T>, int&&>);
    static_assert(std::is_same_v<std::tuple_element_t<29,T>, decltype(&caff::test::dummy_class::member_variable)>);
    static_assert(std::is_same_v<std::tuple_element_t<30,T>, decltype(&caff::test::dummy_class::member_function)>);

    CHECK(true);
}

TEST_CASE("cv_qualifiable_types", "[type_list]")
{
    using T = caff::test::cv_qualifiable_types;

    static_assert(std::tuple_size_v<T> == 28);
    static_assert(std::is_same_v<std::tuple_element_t<0,T>, void>);
    static_assert(std::is_same_v<std::tuple_element_t<1,T>, std::nullptr_t>);
    static_assert(std::is_same_v<std::tuple_element_t<2,T>, bool>);
    static_assert(std::is_same_v<std::tuple_element_t<3,T>, char>);
    static_assert(std::is_same_v<std::tuple_element_t<4,T>, wchar_t>);
    static_assert(std::is_same_v<std::tuple_element_t<5,T>, char8_t>);
    static_assert(std::is_same_v<std::tuple_element_t<6,T>, char16_t>);
    static_assert(std::is_same_v<std::tuple_element_t<7,T>, char32_t>);
    static_assert(std::is_same_v<std::tuple_element_t<8,T>, signed char>);
    static_assert(std::is_same_v<std::tuple_element_t<9,T>, short>);
    static_assert(std::is_same_v<std::tuple_element_t<10,T>, int>);
    static_assert(std::is_same_v<std::tuple_element_t<11,T>, long>);
    static_assert(std::is_same_v<std::tuple_element_t<12,T>, long long>);
    static_assert(std::is_same_v<std::tuple_element_t<13,T>, unsigned char>);
    static_assert(std::is_same_v<std::tuple_element_t<14,T>, unsigned short>);
    static_assert(std::is_same_v<std::tuple_element_t<15,T>, unsigned int>);
    static_assert(std::is_same_v<std::tuple_element_t<16,T>, unsigned long>);
    static_assert(std::is_same_v<std::tuple_element_t<17,T>, unsigned long long>);
    static_assert(std::is_same_v<std::tuple_element_t<18,T>, float>);
    static_assert(std::is_same_v<std::tuple_element_t<19,T>, double>);
    static_assert(std::is_same_v<std::tuple_element_t<20,T>, long double>);
    static_assert(std::is_same_v<std::tuple_element_t<21,T>, int[]>);
    static_assert(std::is_same_v<std::tuple_element_t<22,T>, caff::test::dummy_enum>);
    static_assert(std::is_same_v<std::tuple_element_t<23,T>, caff::test::dummy_union>);
    static_assert(std::is_same_v<std::tuple_element_t<24,T>, caff::test::dummy_class>);
    //static_assert(std::is_same_v<std::tuple_element_t<25,T>, decltype(caff::test::dummy_function)>);
    static_assert(std::is_same_v<std::tuple_element_t<25,T>, int*>);
    //static_assert(std::is_same_v<std::tuple_element_t<27,T>, int&>);
    //static_assert(std::is_same_v<std::tuple_element_t<28,T>, int&&>);
    static_assert(std::is_same_v<std::tuple_element_t<26,T>, decltype(&caff::test::dummy_class::member_variable)>);
    static_assert(std::is_same_v<std::tuple_element_t<27,T>, decltype(&caff::test::dummy_class::member_function)>);

    static_assert(std::tuple_size_v<T> == 28);
    static_assert(std::is_same_v<std::tuple_element_t<0,T>, void>);
    static_assert(std::is_same_v<std::tuple_element_t<1,T>, std::nullptr_t>);
    static_assert(std::is_same_v<std::tuple_element_t<2,T>, bool>);
    static_assert(std::is_same_v<std::tuple_element_t<3,T>, char>);
    static_assert(std::is_same_v<std::tuple_element_t<4,T>, wchar_t>);
    static_assert(std::is_same_v<std::tuple_element_t<5,T>, char8_t>);
    static_assert(std::is_same_v<std::tuple_element_t<6,T>, char16_t>);
    static_assert(std::is_same_v<std::tuple_element_t<7,T>, char32_t>);
    static_assert(std::is_same_v<std::tuple_element_t<8,T>, signed char>);
    static_assert(std::is_same_v<std::tuple_element_t<9,T>, short>);
    static_assert(std::is_same_v<std::tuple_element_t<10,T>, int>);
    static_assert(std::is_same_v<std::tuple_element_t<11,T>, long>);
    static_assert(std::is_same_v<std::tuple_element_t<12,T>, long long>);
    static_assert(std::is_same_v<std::tuple_element_t<13,T>, unsigned char>);
    static_assert(std::is_same_v<std::tuple_element_t<14,T>, unsigned short>);
    static_assert(std::is_same_v<std::tuple_element_t<15,T>, unsigned int>);
    static_assert(std::is_same_v<std::tuple_element_t<16,T>, unsigned long>);
    static_assert(std::is_same_v<std::tuple_element_t<17,T>, unsigned long long>);
    static_assert(std::is_same_v<std::tuple_element_t<18,T>, float>);
    static_assert(std::is_same_v<std::tuple_element_t<19,T>, double>);
    static_assert(std::is_same_v<std::tuple_element_t<20,T>, long double>);
    static_assert(std::is_same_v<std::tuple_element_t<21,T>, int[]>);
    static_assert(std::is_same_v<std::tuple_element_t<22,T>, caff::test::dummy_enum>);
    static_assert(std::is_same_v<std::tuple_element_t<23,T>, caff::test::dummy_union>);
    static_assert(std::is_same_v<std::tuple_element_t<24,T>, caff::test::dummy_class>);
    //static_assert(std::is_same_v<std::tuple_element_t<25,T>, decltype(caff::test::dummy_function)>);
    static_assert(std::is_same_v<std::tuple_element_t<25,T>, int*>);
    //static_assert(std::is_same_v<std::tuple_element_t<27,T>, int&>);
    //static_assert(std::is_same_v<std::tuple_element_t<28,T>, int&&>);
    static_assert(std::is_same_v<std::tuple_element_t<26,T>, decltype(&caff::test::dummy_class::member_variable)>);
    static_assert(std::is_same_v<std::tuple_element_t<27,T>, decltype(&caff::test::dummy_class::member_function)>);

    CHECK(true);
}