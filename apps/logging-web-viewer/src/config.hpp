// Copyright (C) 2026 White Atelier
// This software is released under the MIT License.
// See the LICENSE file in the project root for more details.

#ifndef ROAH_LOGGING_WEB_VIEWER_CONFIG_HPP
#define ROAH_LOGGING_WEB_VIEWER_CONFIG_HPP

#include <cstdint>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <string_view>

namespace roah::logging::webv {

class WebvConfigError : public std::runtime_error
{
public:
    using std::runtime_error::runtime_error;
};

class Config
{
private:
    Config();

public:
    static Config
    load(const std::string_view arg0, const std::filesystem::path & path);

    Config(const Config &) = delete;
    Config(Config &&) noexcept;
    ~Config() noexcept;

    Config &
    operator=(const Config &)
        = delete;
    Config &
    operator=(Config &&) noexcept
        = delete;

    const std::string &
    getServerHost() const noexcept;

    std::uint_fast16_t
    getHttpServerPort() const noexcept;

    std::uint_fast16_t
    getWebsocketServerPort() const noexcept;

private:
    std::string        host_;
    std::uint_fast16_t http_port_;
    std::uint_fast16_t wss_port_;
};

}  // namespace roah::logging::webv

#endif
