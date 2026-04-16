// Copyright (C) 2026 White Atelier
// This software is released under the MIT License.
// See the LICENSE file in the project root for more details.

#include "webv_sink.hpp"

#include <IXNetSystem.h>

#include <format>
#include <iostream>

#if defined(ROAH_ARCH_WIN32)
#    include <Windows.h>
#elif defined(ROAH_ARCH_LINUX)
#    include <unistd.h>
#else
#    error "Unsupported platform"
#endif

namespace {
class NetSystem
{
public:
    NetSystem()
        : ok_{ ix::initNetSystem() }
    {
        if (!this->ok_)
        {
            std::cerr << "Failed to initialize network system" << std::endl;
        }
    }

    ~NetSystem() noexcept { ix::uninitNetSystem(); }

    const bool ok_;
};
}  // namespace

template <typename Mutex>
roah::logger::impl::WebVSink<Mutex>::WebVSink(const std::string_view   host,
                                              const std::uint_fast16_t port,
                                              std::string              application_name)
    : process_id_{ 0 }
    , ws_{}
    , application_name_{ std::move(application_name) }
    , j_root{ { "type", "logs" }, { "logs", nlohmann::json::array() } }
    , j_logs{ &this->j_root["logs"] }
    , thread_{ [this](const std::stop_token st) { this->_flushProc(st); } }
{
    static NetSystem net_system;
    if (!net_system.ok_)
    {
        return;
    }

#if defined(ROAH_ARCH_WIN32)
    this->process_id_ = static_cast<std::uint64_t>(::GetCurrentProcessId());
#elif defined(ROAH_ARCH_LINUX)
    this->process_id_ = static_cast<std::uint64_t>(::getpid());
#else
#    error "Unsupported platform"
#endif
    this->ws_.setOnMessageCallback([](const ix::WebSocketMessagePtr &) {
        // NOP
    });

    this->ws_.setUrl(std::format("ws://{}:{}", host, port));
    this->ws_.setPingInterval(30);
    this->ws_.enableAutomaticReconnection();
    this->ws_.enablePerMessageDeflate();
    this->ws_.start();
}

template <typename Mutex>
roah::logger::impl::WebVSink<Mutex>::~WebVSink() noexcept
{
    if (this->thread_.joinable())
    {
        this->thread_.request_stop();
        this->thread_.join();
    }
    this->ws_.stop();
}

template <typename Mutex>
void
roah::logger::impl::WebVSink<Mutex>::sink_it_(const spdlog::details::log_msg & msg)
{
    this->j_logs->emplace_back(nlohmann::json{
        { "app", this->application_name_ },
        { "lvl", spdlog::level::to_string_view(msg.level) },
        { "tpt", std::format("{:%FT%T%z}", msg.time) },
        { "pid", this->process_id_ },
        { "tid", msg.thread_id },
        { "nam", msg.logger_name },
        { "msg", std::string_view{ msg.payload.data(), msg.payload.size() } },
        { "src", msg.source.filename },
        { "lno", msg.source.line },
        { "fnc", msg.source.funcname },
    });

    if (msg.level >= spdlog::level::err)
    {
        this->flush_();
    }
}

template <typename Mutex>
void
roah::logger::impl::WebVSink<Mutex>::flush_()
{
    if (!this->j_logs->empty())
    {
        this->ws_.sendUtf8Text(this->j_root.dump());
        this->j_logs->clear();
    }
}

template <typename Mutex>
void
roah::logger::impl::WebVSink<Mutex>::_flushProc(const std::stop_token & st)
{
    while (!st.stop_requested())
    {
        constexpr std::chrono::milliseconds interval{ 100 };
        std::this_thread::sleep_for(interval);
        this->flush();
    }

    // 最後に一度 flush する
    this->flush();
}

template class roah::logger::impl::WebVSink<std::mutex>;
template class roah::logger::impl::WebVSink<spdlog::details::null_mutex>;
