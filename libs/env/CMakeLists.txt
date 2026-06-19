
roah_add_library(roah-env
    SOURCES
        env.cpp

    HEADERS
        roah/env.hpp

    MSVC_HEADERS_FILTER_ROOT
        roah

    COMPILE_DEFINITIONS
        _CRT_SECURE_NO_WARNINGS
)
