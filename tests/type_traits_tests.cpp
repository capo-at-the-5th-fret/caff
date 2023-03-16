#include <doctest/doctest.h>
#include "easy/type_traits.h"

TEST_CASE("always_false")
{
    static_assert(easy::always_false<int>::value == false);
    static_assert(easy::always_false<int, double>::value == false);

    static_assert(easy::always_false_v<int> == false);
    static_assert(easy::always_false_v<int, double> == false);
}
