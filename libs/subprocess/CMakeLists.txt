# Copyright (C) 2026 White Atelier
# This software is released under the MIT License.
# See the LICENSE file in the project root for more details.

find_package(toml11 REQUIRED)

roah_add_library(roah-subprocess
    SOURCES
        process_id.cpp

    HEADERS
        roah/process_id.hpp

    TESTS
        main.cpp

    MSVC_HEADERS_FILTER_ROOT
        roah
)
