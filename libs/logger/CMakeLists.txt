
find_package(spdlog 1 REQUIRED)

roah_add_library(roah-logger
    LIBRARIES
        spdlog::spdlog
        roah-subprocess

    SOURCES
        logger.cpp

    HEADERS
        roah/detail/logger_types.hpp
        roah/logger.hpp

    MSVC_HEADERS_FILTER_ROOT
        roah
)

if (LIBROAH_ENABLE_LOGGER_WEBV_SINK)
    find_package(nlohmann_json REQUIRED)
    target_sources(roah-logger
        PRIVATE
            src/impl/webv_sink.cpp
            src/impl/webv_sink.hpp
    )
    target_compile_definitions(roah-logger
        PRIVATE
            LIBROAH_ENABLE_LOGGER_WEBV_SINK
    )
    target_link_libraries(roah-logger
        PRIVATE
            nlohmann_json::nlohmann_json
            ixwebsocket::ixwebsocket
    )
endif()