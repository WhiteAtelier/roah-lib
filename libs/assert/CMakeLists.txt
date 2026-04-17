# Copyright (C) 2026 White Atelier
# This software is released under the MIT License.
# See the LICENSE file in the project root for more details.

roah_add_library(roah-assert
    SOURCES
        assert.cpp

    HEADERS
        roah/assert.hpp

    TESTS
        main.cpp

    PUBLIC_COMPILE_DEFINITIONS
        $<$<CONFIG:Debug>:ROAH_ASSERT_ENABLE>

    COMPILE_DEFINITIONS
        $<$<BOOL:${BUILD_ROAH_ASSERT}>:ROAH_ASSERT_LOGGER_ENABLE>

    MSVC_HEADERS_FILTER_ROOT
        roah
)

if (BUILD_ROAH_ASSERT)
    target_link_libraries(roah-assert
        PRIVATE
            roah-logger
    )
    if (TARGET roah-assert-test)
        target_link_libraries(roah-assert-test
            PRIVATE
                roah-logger
        )
    endif()
endif()