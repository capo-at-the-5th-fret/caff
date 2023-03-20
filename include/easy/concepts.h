#pragma once

#include "easy/type_traits.h"

namespace easy
{
    template <typename T>
    concept boolean = is_boolean_v<T>;

    template <typename T>
    concept standard_integer = is_standard_integer_v<T>;

    template <typename T>
    concept cv_qualifiable = is_cv_qualifiable_v<T>;
}
