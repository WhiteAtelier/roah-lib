# Copyright (C) 2026 White Atelier
# This software is released under the MIT License.
# See the LICENSE file in the project root for more details.

find_package(toml11 REQUIRED)

roah_add_library(roah-config
    LIBRARIES
        toml11::toml11

    SOURCES
        config_loader_error.cpp
        config_loader.cpp

    HEADERS
        roah/config_loader_error.hpp
        roah/config_loader.hpp

    TESTS
        main.cpp

    MSVC_HEADERS_FILTER_ROOT
        roah
)
