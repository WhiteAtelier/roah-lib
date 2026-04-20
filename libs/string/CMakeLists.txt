# Copyright (C) 2026 White Atelier
# This software is released under the MIT License.
# See the LICENSE file in the project root for more details.

roah_add_library(roah-string
    SOURCES
        string.cpp

    HEADERS
        roah/string.hpp

    TESTS
        main.cpp

    MSVC_HEADERS_FILTER_ROOT
        roah
)
