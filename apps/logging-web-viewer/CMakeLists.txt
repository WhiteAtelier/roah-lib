# Copyright (C) 2026 White Atelier
# This software is released under the MIT License.
# See the LICENSE file in the project root for more details.

# --- frontend, npm build target ---
find_program(NPM_EXECUTABLE npm REQUIRED)

# config 時にのみ npm install を /http/webv で実行
if(WIN32)
    set(NPM_EXECUTE_CMD cmd /c ${NPM_EXECUTABLE})
else()
    set(NPM_EXECUTE_CMD ${NPM_EXECUTABLE})
endif()
execute_process(
    COMMAND
        ${NPM_EXECUTE_CMD} install
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_SOURCE_DIR}/http/webv
    COMMAND_ERROR_IS_FATAL
        ANY
)

file(GLOB_RECURSE WEBV_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/http/webv/src/*.tsx
    ${CMAKE_CURRENT_SOURCE_DIR}/http/webv/src/*.scss
)

add_custom_command(
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/dist/index.html
    COMMAND
        ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/dist
    COMMAND
        ${NPM_EXECUTABLE} run build -- --outDir ${CMAKE_CURRENT_BINARY_DIR}/dist

    WORKING_DIRECTORY
        ${CMAKE_CURRENT_SOURCE_DIR}/http/webv

    DEPENDS
        ${WEBV_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/http/webv/index.html
        ${CMAKE_CURRENT_SOURCE_DIR}/http/webv/package.json
        ${CMAKE_CURRENT_SOURCE_DIR}/http/webv/tsconfig.json
        ${CMAKE_CURRENT_SOURCE_DIR}/http/webv/vite.config.ts

    COMMENT
        "Building frontend (npm run build)"
)

# --- embedding frontend files into C++ ---
set(WEBV_BINPACK_CPP ${CMAKE_CURRENT_BINARY_DIR}/embedded/binpack.cpp)

add_custom_command(
    OUTPUT ${WEBV_BINPACK_CPP}
    COMMAND ${CMAKE_COMMAND}
        -DDIST_DIR=${CMAKE_CURRENT_BINARY_DIR}/dist
        -DOUTPUT_DIR=${CMAKE_CURRENT_BINARY_DIR}/embedded
        -DTEMPLATE_FILE=${CMAKE_CURRENT_SOURCE_DIR}/binpack.cpp.in
        -DOUTPUT_CPP=${CMAKE_CURRENT_BINARY_DIR}/embedded/binpack.cpp
        -P ${CMAKE_CURRENT_LIST_DIR}/embed_files.cmake
    
    DEPENDS
        ${CMAKE_CURRENT_BINARY_DIR}/dist/index.html

    COMMENT
        "Embedding frontend files into .inl files"
)

# --- application ---
roah_add_executable(roah-logging-web-viewer
    LIBRARIES
        nlohmann_json::nlohmann_json
        ixwebsocket::ixwebsocket
        CLI11::CLI11
        roah-assert
        roah-logger
        roah-config
        roah-url-parser

    SOURCES
        binpack.hpp
        config.cpp
        config.hpp
        main.cpp
        rest_api_server.cpp
        rest_api_server.hpp
        websocket_server.cpp
        websocket_server.hpp
)

target_sources(roah-logging-web-viewer
    PRIVATE
        ${WEBV_BINPACK_CPP}
)

roah_add_executable(roah-logging-web-viewer-testclient
    LIBRARIES
        CLI11::CLI11
        roah-logger

    SOURCES
        test_client.cpp
)
