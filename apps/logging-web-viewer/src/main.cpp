// Copyright (C) 2026 White Atelier
// This software is released under the MIT License.
// See the LICENSE file in the project root for more details.

#include "config.hpp"
#include "rest_api_server.hpp"
#include "websocket_server.hpp"
//
#include "roah/logger.hpp"

#include <CLI/CLI.hpp>

#include <IXNetSystem.h>

#include <iostream>
#include <optional>
#include <semaphore>

int
main(const int argc, const char * const argv[])
{
    std::filesystem::path config_path;
    CLI::App              app{ "roah logging webviewer" };
    app.add_option("-c,--config", config_path, "Path to configuration file")  //
        ->check(CLI::ExistingFile);
    const auto subc_check_conf = app.add_subcommand("check-conf", "Check config mode.");
    CLI11_PARSE(app, argc, argv);

    try
    {
        const auto config = roah::logging::webv::Config::load(argc >= 1 ? argv[0] : "", config_path);
        if (subc_check_conf->parsed())
        {
            std::cout << "Configuration file is valid." << std::endl;
            std::cout << "--      HTTP Server: " << config.getServerHost() << ":" << config.getHttpServerPort()
                      << std::endl;
            std::cout << "-- WebSocket Server: " << config.getServerHost() << ":" << config.getWebsocketServerPort()
                      << std::endl;
            return 0;
        }

        roah::initializeLogger("roah-logging-web-viewer",
                               roah::LoggerInitializeArgs{
                                   .console  = { .level = roah::LogLevel::Debug },
                                   .log_file = {},
                                   .webv     = { .level       = roah::LogLevel::Debug,
                                                 .server_host = config.getServerHost(),
                                                 .server_port = config.getWebsocketServerPort() },
                               });

        if (!ix::initNetSystem())
        {
            std::cout << "Failed to initialize network system." << std::endl;
            return 1;
        }

        roah::logging::webv::RestAPIServer rest_server{ config };
        if (!rest_server.start())
        {
            std::cout << "Failed to start RestAPI." << std::endl;
            return 1;
        }

        roah::logging::webv::WebsocketServer ws_server{ config };
        if (!ws_server.start())
        {
            std::cout << "Failed to start WebsocketServer." << std::endl;
            return 1;
        }

        std::binary_semaphore sem{ 0 };
        sem.acquire();  // blocking
    }
    catch (const roah::logging::webv::WebvConfigError & e)
    {
        std::cerr << "Configuration error: " << e.what() << std::endl;
        return 1;
    }

    ix::uninitNetSystem();

    return 0;
}
