add_library(easy INTERFACE)
add_library(easy::easy ALIAS easy)

target_sources(easy
    INTERFACE
        FILE_SET HEADERS
        FILES
            easy/bit.hpp
)

target_compile_features(easy INTERFACE cxx_std_23)
