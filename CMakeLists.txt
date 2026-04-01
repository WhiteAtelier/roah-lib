# Copyright (C) 2026 White Atelier
# This software is released under the MIT License.
# See the LICENSE file in the project root for more details.

cmake_minimum_required(VERSION 3.30)

project(libroah
    VERSION
        1.3.0
    DESCRIPTION
        "Roah Common Libraries"
    LANGUAGES
        CXX
)

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

option(BUILD_ROAH_ASSERT "Build libroah::roah-assert" ON)
option(BUILD_ROAH_LOGGER "Build libroah::roah-logger" ON)
option(BUILD_ROAH_CONFIG "Build libroah::roah-config" ON)
option(BUILD_ROAH_URL_PARSER "Build libroah::roah-url-parser" ON)

# --- Dependencies ---
if (BUILD_ROAH_LOGGER)
    find_package(spdlog 1 REQUIRED)
endif()

if (BUILD_ROAH_CONFIG)
    find_package(toml11 REQUIRED)
endif()

if (BUILD_ROAH_URL_PARSER)
    # No dependencies for now
endif()

add_subdirectory(libs)

add_doxygen_target()
export_config()

install(
    FILES
        "${CMAKE_SOURCE_DIR}/LICENSE"
    DESTINATION
        "lib/cmake/${CMAKE_PROJECT_NAME}"
    RENAME
        "libroah-LICENSE"
)
