# Copyright (C) 2026 White Atelier
# This software is released under the MIT License.
# See the LICENSE file in the project root for more details.

roah_add_library(roah-url-parser
    SOURCES
        url_parse_error.cpp
        url_parser.cpp

        impl/fixed_word.cpp
        impl/fixed_word.hpp
        impl/key_word.cpp
        impl/key_word.hpp
        impl/path_words.cpp
        impl/path_words.hpp
        impl/url_parts_impl.cpp
        impl/url_parts_impl.hpp
        impl/word.cpp
        impl/word.hpp

    HEADERS
        roah/url_parse_error.hpp
        roah/url_parser.hpp
        roah/url_parts.hpp

    TESTS
        main.cpp

    MSVC_HEADERS_FILTER_ROOT
        roah
)
