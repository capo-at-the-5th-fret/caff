add_library(easy INTERFACE)
add_library(easy::easy ALIAS easy)

target_sources(easy
    INTERFACE
        FILE_SET HEADERS
        FILES
            easy/bit.h
            easy/concepts.h
            easy/functional.h
            easy/math.h
            easy/tuple.h
            easy/type_list.h
            easy/type_traits.h
            easy/variant.h

            easy/test/dummy.h
            easy/test/type_list.h
)

target_compile_features(easy INTERFACE cxx_std_23)
