// Copyright (C) 2026 White Atelier
// This software is released under the MIT License.
// See the LICENSE file in the project root for more details.

#include "roah/logger.hpp"

#include <CLI/CLI.hpp>

#include <chrono>
#include <cstdint>
#include <random>
#include <string>
#include <string_view>
#include <thread>

namespace {

constexpr std::string_view lorem_ipsum
    = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. "
      "Vivamus posuere ante diam, eget dapibus leo aliquam quis. "
      "Donec tristique vitae ante id venenatis. "
      "Curabitur at dapibus odio. "
      "Cras semper nulla eu convallis pharetra. "
      "Ut a odio ultrices, imperdiet dui non, aliquet neque. "
      "Integer pretium massa felis, vitae auctor tellus hendrerit et. "
      "Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. "
      "Praesent gravida ipsum ex, eget faucibus purus ultrices in. "
      "Donec tempus cursus tellus, ac ullamcorper lectus placerat ut. "
      "Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. "
      "Maecenas id porttitor est.";

std::string_view
_getDummy(const std::size_t len)
{
    return lorem_ipsum.substr(0, len);
}

}  // namespace

int
main(const int argc, const char * const argv[])
{
    CLI::App      app;
    std::string   host     = "127.0.0.1";
    std::uint16_t port     = 32124;
    std::uint32_t interval = 100;
    std::uint32_t cycle    = 100;
    app.add_option("--host", host, "host address")->default_val(host);
    app.add_option("--port", port, "wss port")->default_val(port);
    app.add_option("-i,--interval", interval, "Interval msec")->default_val(interval);
    app.add_option("-c,--cycle", cycle, "Cycle count")->default_val(cycle);
    CLI11_PARSE(app, argc, argv);

    roah::initializeLogger(  //
        "roah-webv-test-client",
        {
            .console  = { .level = roah::LogLevel::Trace },
            .log_file = {},
            .webv     = { .level       = roah::LogLevel::Trace,  //
                          .server_host = host,
                          .server_port = port },
        });

    std::chrono::milliseconds interval_t{ interval };

    const roah::Logger logger;

    std::default_random_engine                 engine{ std::random_device{}() };
    std::uniform_int_distribution<std::size_t> dist{ 4ull, lorem_ipsum.size() };

    for (std::uint32_t i = 0; i < cycle; ++i)
    {
        ROAH_TRACE(logger, "This is trace log! {}/{} {}", i + 1, cycle, _getDummy(dist(engine)));
        std::this_thread::sleep_for(interval_t);
        ROAH_LOG(logger, "This is debug log! {}/{} {}", i + 1, cycle, _getDummy(dist(engine)));
        std::this_thread::sleep_for(interval_t);
        ROAH_INFO(logger, "This is info log! {}/{} {}", i + 1, cycle, _getDummy(dist(engine)));
        std::this_thread::sleep_for(interval_t);
        ROAH_WARN(logger, "This is warning log! {}/{} {}", i + 1, cycle, _getDummy(dist(engine)));
        std::this_thread::sleep_for(interval_t);
        ROAH_ERROR(logger, "This is error log! {}/{} {}", i + 1, cycle, _getDummy(dist(engine)));
        std::this_thread::sleep_for(interval_t);
        ROAH_CRITICAL(logger, "This is critical log! {}/{} {}", i + 1, cycle, _getDummy(dist(engine)));
        std::this_thread::sleep_for(interval_t);
    }

    return 0;
}