// Copyright (C) 2026 White Atelier
// This software is released under the MIT License.
// See the LICENSE file in the project root for more details.

#ifndef ROAH_LOGGER_IMPL_WEBV_SINK_HPP
#define ROAH_LOGGER_IMPL_WEBV_SINK_HPP

#include "spdlog/details/null_mutex.h"
#include "spdlog/sinks/base_sink.h"

#include <nlohmann/json.hpp>

#include <IXWebSocket.h>

#include <mutex>
#include <string>
#include <thread>

namespace roah::logger::impl {

/// @brief roah-logging-web-viewer 向けの spdlog sink.
template <typename Mutex>
class WebVSink : public spdlog::sinks::base_sink<Mutex>
{
public:
    explicit WebVSink(const std::string_view host, const std::uint_fast16_t port, std::string application_name);

    ~WebVSink() noexcept override;

    WebVSink(const WebVSink &) = delete;
    WebVSink(WebVSink &&)      = delete;
    WebVSink &
    operator=(const WebVSink &)
        = delete;
    WebVSink &
    operator=(WebVSink &&) noexcept
        = delete;

private:
    void
    sink_it_(const spdlog::details::log_msg & msg) override;

    void
    flush_() override;

    void
    _flushProc(const std::stop_token & st);

    std::uint64_t    process_id_;
    ix::WebSocket    ws_;
    std::string      application_name_;
    nlohmann::json   j_root;
    nlohmann::json * j_logs;
    std::jthread     thread_;
};

using WebVSinkMt = WebVSink<std::mutex>;
using WebVSinkSt = WebVSink<spdlog::details::null_mutex>;

}  // namespace roah::logger::impl

#endif
