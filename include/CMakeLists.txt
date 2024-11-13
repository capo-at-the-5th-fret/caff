add_library(caff INTERFACE)
add_library(caff::caff ALIAS caff)

include(FetchContent)

FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt
    GIT_TAG        11.0.2
    FIND_PACKAGE_ARGS 11.0.2
)

FetchContent_MakeAvailable(fmt)

target_sources(caff
    INTERFACE
        FILE_SET HEADERS
        FILES
            caff/bit.h
            caff/concepts.h
            caff/format.h
            caff/functional.h
            caff/math.h
            caff/tuple.h
            caff/type_list.h
            caff/type_traits.h
            caff/utility.h
            caff/variant.h

            caff/test/dummy.h
            caff/test/type_list.h
)

target_link_libraries(caff INTERFACE fmt::fmt)
