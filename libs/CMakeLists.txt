# Copyright (C) 2026 White Atelier
# This software is released under the MIT License.
# See the LICENSE file in the project root for more details.

if (BUILD_ROAH_ASSERT)
    add_subdirectory(assert)
endif()

if (BUILD_ROAH_LOGGER)
    add_subdirectory(logger)
endif()

if (BUILD_ROAH_CONFIG)
    add_subdirectory(config)
endif()

if (BUILD_ROAH_URL_PARSER)
    add_subdirectory(url-parser)
endif()
