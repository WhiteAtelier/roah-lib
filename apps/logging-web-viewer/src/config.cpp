// Copyright (C) 2026 White Atelier
// This software is released under the MIT License.
// See the LICENSE file in the project root for more details.

#include "config.hpp"

#include "roah/config_loader.hpp"
#include "roah/config_loader_error.hpp"

roah::logging::webv::Config::Config()
    : host_{}
    , http_port_{ 0 }
    , wss_port_{ 0 }
{}

roah::logging::webv::Config::Config(Config &&) noexcept = default;

roah::logging::webv::Config::~Config() noexcept = default;

roah::logging::webv::Config
roah::logging::webv::Config::load([[maybe_unused]] const std::string_view arg0, const std::filesystem::path & _path)
{
    Config       config;
    ConfigLoader loader;

    auto path = _path;
    if (path.empty())
    {
        // 以下, 起動引数にて指定されなかった場合のデフォルトパスの規定.
#if defined(ROAH_ARCH_WIN32)
        // Win32 の場合は, exe と同じ場所に配置された conf を読む.
        path = (arg0.empty() ? std::filesystem::current_path() : std::filesystem::path{ arg0 }.parent_path())
             / "roah-logging-web-viewer.conf";
#elif defined(ROAH_ARCH_LINUX)
        // Linux の場合は, /etc/roah/roah-logging-web-viewer.conf を読む.
        path = "/etc/roah/roah-logging-web-viewer.conf";
#else
#    error "Unsupported platform"
#endif
    }

    try
    {
        loader.load(path);
        config.host_         = loader.getString("server", "host", "127.0.0.1");
        const auto http_port = loader.getInt("server", "http_port", 32123);
        const auto wss_port  = loader.getInt("server", "wss_port", 32124);

        if (0 < http_port && http_port <= 65535)
        {
            config.http_port_ = static_cast<std::uint_fast16_t>(http_port);
        }
        else
        {
            throw WebvConfigError{ "Invalid http_port: " + std::to_string(http_port) };
        }

        if (0 < wss_port && wss_port <= 65535)
        {
            config.wss_port_ = static_cast<std::uint_fast16_t>(wss_port);
        }
        else
        {
            throw WebvConfigError{ "Invalid wss_port: " + std::to_string(wss_port) };
        }
    }
    catch (const roah::ConfigLoaderError & e)
    {
        throw WebvConfigError{ e.what() };
    }

    return config;
}

const std::string &
roah::logging::webv::Config::getServerHost() const noexcept
{
    return this->host_;
}

std::uint_fast16_t
roah::logging::webv::Config::getHttpServerPort() const noexcept
{
    return this->http_port_;
}

std::uint_fast16_t
roah::logging::webv::Config::getWebsocketServerPort() const noexcept
{
    return this->wss_port_;
}