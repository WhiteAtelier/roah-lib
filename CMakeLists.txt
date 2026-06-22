# Copyright (C) 2026 White Atelier
# This software is released under the MIT License.
# See the LICENSE file in the project root for more details.

cmake_minimum_required(VERSION 3.30)

project(libroah
    VERSION
        1.10.2
    DESCRIPTION
        "Roah Common Libraries"
    LANGUAGES
        CXX
)

if ("${CMAKE_PROJECT_NAME}" STREQUAL "${PROJECT_NAME}")
    set(LIBROAH_IS_MASTER_PROJECT ON)
else()
    set(LIBROAH_IS_MASTER_PROJECT OFF)
endif()

option(LIBROAH_BUILD_TESTS "Build test project."  OFF)
option(LIBROAH_BUILD_DOCS  "Build documentation." OFF)
option(LIBROAH_INSTALL     "Install libroah"      ${LIBROAH_IS_MASTER_PROJECT})

option(LIBROAH_BUILD_ASSERT                "Build libroah::roah-assert"     ON)
option(LIBROAH_BUILD_SUBPROCESS            "Build libroah::roah-subprocess" ON)
option(LIBROAH_BUILD_ENV                   "Build libroah::roah-env"        ON)
option(LIBROAH_BUILD_LOGGER                "Build libroah::roah-logger"     ON)
option(LIBROAH_BUILD_CONFIG                "Build libroah::roah-config"     ON)
option(LIBROAH_BUILD_STRING                "Build libroah::roah-string"     ON)
option(LIBROAH_BUILD_URL_PARSER            "Build libroah::roah-url-parser" ON)
option(LIBROAH_BUILD_LOGGER_WEB_VIEWER_APP "Build logging-web-viewer app"   ON)

# Logger: Enable WebV sink
option(LIBROAH_ENABLE_LOGGER_WEBV_SINK "Enable WebV sink in roah-logger library" ON)

include(cmake/doxygen/doxygen.cmake)
include(cmake/utils.cmake)

set(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "Debug postfix")

add_compile_options(
    "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>"
    "$<$<CXX_COMPILER_ID:MSVC>:/EHsc>"
    "$<$<CXX_COMPILER_ID:MSVC>:/MP>"
    "$<$<CXX_COMPILER_ID:MSVC>:/W4>"
    "$<$<CXX_COMPILER_ID:MSVC>:/WX>"

    "$<$<CXX_COMPILER_ID:GNU>:-Wall>"
    "$<$<CXX_COMPILER_ID:GNU>:-Wextra>"
    "$<$<CXX_COMPILER_ID:GNU>:-Werror>"
)
add_compile_definitions(
    "$<$<CXX_COMPILER_ID:MSVC>:NOMINMAX>"
    "$<$<CXX_COMPILER_ID:MSVC>:WIN32_LEAN_AND_MEAN>"
)

# --- Dependencies ---
if (LIBROAH_BUILD_TESTS)
    enable_testing()
    find_package(GTest REQUIRED)
    include(GoogleTest)
endif()

if (LIBROAH_BUILD_DOCS)
    find_package(Doxygen REQUIRED)
endif()

if (LIBROAH_BUILD_ASSERT)
    # No dependencies for now
endif()

if (LIBROAH_BUILD_SUBPROCESS)
    # No dependencies for now
endif()

if (LIBROAH_BUILD_ENV)
    # No dependencies for now
endif()

if (LIBROAH_BUILD_LOGGER)
    if (NOT LIBROAH_BUILD_SUBPROCESS)
        message(FATAL_ERROR "roah-logger library requires roah-subprocess library. Please enable LIBROAH_BUILD_SUBPROCESS option.")
    endif()

    find_package(spdlog 1 REQUIRED)

    if (LIBROAH_ENABLE_LOGGER_WEBV_SINK)
        find_package(ixwebsocket REQUIRED)
        find_package(nlohmann_json REQUIRED)
    endif()
endif()

if (LIBROAH_BUILD_CONFIG)
    find_package(toml11 REQUIRED)
endif()

if (LIBROAH_BUILD_STRING)
    # No dependencies for now
endif()

if (LIBROAH_BUILD_URL_PARSER)
    # No dependencies for now
endif()

if (LIBROAH_BUILD_LOGGER_WEB_VIEWER_APP)
    # Require roah libraries...
    # - roah-assert
    # - roah-url-parser
    # - roah-logger
    # - roah-config
    if (NOT LIBROAH_BUILD_ASSERT)
        message(FATAL_ERROR "logging-web-viewer app requires roah-assert library. Please enable LIBROAH_BUILD_ASSERT option.")
    endif()
    if (NOT LIBROAH_BUILD_URL_PARSER)
        message(FATAL_ERROR "logging-web-viewer app requires roah-url-parser library. Please enable LIBROAH_BUILD_URL_PARSER option.")
    endif()
    if (NOT LIBROAH_BUILD_LOGGER)
        message(FATAL_ERROR "logging-web-viewer app requires roah-logger library. Please enable LIBROAH_BUILD_LOGGER option.")
    endif()
    if (NOT LIBROAH_BUILD_CONFIG)
        message(FATAL_ERROR "logging-web-viewer app requires roah-config library. Please enable LIBROAH_BUILD_CONFIG option.")
    endif()

    find_program(NPM_EXECUTABLE npm REQUIRED)
    find_package(CLI11 REQUIRED)
    find_package(ixwebsocket REQUIRED)
    find_package(nlohmann_json REQUIRED)
endif()

add_subdirectory(libs)
add_subdirectory(apps)

add_doxygen_target()
export_config()

install(
    FILES
        "${CMAKE_SOURCE_DIR}/LICENSE"
    DESTINATION
        "lib/cmake/${PROJECT_NAME}"
    RENAME
        "libroah-LICENSE"
)
