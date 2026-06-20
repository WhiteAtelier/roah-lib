# Copyright (C) 2026 White Atelier
# This software is released under the MIT License.
# See the LICENSE file in the project root for more details.

if (LIBROAH_BUILD_ASSERT)
    add_subdirectory(assert)
endif()

if (LIBROAH_BUILD_SUBPROCESS)
    add_subdirectory(subprocess)
endif()

if (LIBROAH_BUILD_ENV)
    add_subdirectory(env)
endif()

if (LIBROAH_BUILD_LOGGER)
    add_subdirectory(logger)
endif()

if (LIBROAH_BUILD_CONFIG)
    add_subdirectory(config)
endif()

if (LIBROAH_BUILD_STRING)
    add_subdirectory(string)
endif()

if (LIBROAH_BUILD_URL_PARSER)
    add_subdirectory(url-parser)
endif()
