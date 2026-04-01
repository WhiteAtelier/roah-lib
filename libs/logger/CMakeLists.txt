
find_package(spdlog 1 REQUIRED)

roah_add_library(roah-logger
    LIBRARIES
        spdlog::spdlog

    SOURCES
        logger.cpp

    HEADERS
        roah/logger.hpp

    MSVC_HEADERS_FILTER_ROOT
        roah
)
