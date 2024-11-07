add_library(caff INTERFACE)
add_library(caff::caff ALIAS caff)

target_sources(caff
    INTERFACE
        FILE_SET HEADERS
        FILES
            caff/bit.h
            caff/concepts.h
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

target_compile_features(caff INTERFACE cxx_std_23)
