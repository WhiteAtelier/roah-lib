// Copyright (C) 2026 White Atelier
// This software is released under the MIT License.
// See the LICENSE file in the project root for more details.

#include "roah/logger.hpp"

#ifdef ROAH_LOGGER_ENABLE_WEBV_SINK
#    include "impl/webv_sink.hpp"
#endif

#include "roah/process_id.hpp"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <array>
#include <filesystem>
#include <format>
#include <memory>
#include <mutex>
#include <ranges>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace roah {

class Logger::Impl_ final
{
public:
    Impl_(std::string && name);
    ~Impl_() noexcept;

    operator bool() const noexcept;

    void
    setupLogger();

    void
    setupLogger(const std::vector<std::shared_ptr<spdlog::sinks::sink>> & sinks);

    void
    log(const LogLevel               level,
        const std::source_location & source_location,
        const std::string_view       fmt,
        const std::format_args &     args);

    void
    flush();

    static void
    initialize(const std::string_view application_name, const LoggerInitializeArgs & args);

    static std::shared_ptr<Impl_>
    getOrCreateImpl(const std::string_view name);

private:
    const std::string               name_;
    LogLevel                        level_;
    std::shared_ptr<spdlog::logger> logger_;

    static std::mutex &
    _getMutex();

    static std::unordered_map<std::string, std::shared_ptr<Impl_>> &
    _getLoggers();

    static std::vector<std::weak_ptr<spdlog::sinks::sink>> &
    _getSinks();
};

}  // namespace roah

namespace {

static bool           _initialized = false;
static roah::LogLevel _initial_level;

struct Converter_
{
    spdlog::source_loc
    operator()(const std::source_location & source_location) const
    {
        return spdlog::source_loc{ source_location.file_name(),
                                   static_cast<int>(source_location.line()),
                                   source_location.function_name() };
    }

    spdlog::level::level_enum
    operator()(const roah::LogLevel lvl) const
    {
        switch (lvl)
        {
            using enum roah::LogLevel;
        case Trace: return spdlog::level::trace;
        case Debug: return spdlog::level::debug;
        case Info: return spdlog::level::info;
        case Warn: return spdlog::level::warn;
        case Error: return spdlog::level::err;
        case Critical: return spdlog::level::critical;
        default: return spdlog::level::off;
        }
    }
};

static const Converter_ _converter;

}  // namespace

std::mutex &
roah::Logger::Impl_::_getMutex()
{
    static std::mutex mutex;
    return mutex;
}

std::unordered_map<std::string, std::shared_ptr<roah::Logger::Impl_>> &
roah::Logger::Impl_::_getLoggers()
{
    static std::unordered_map<std::string, std::shared_ptr<Impl_>> loggers;
    return loggers;
}

std::vector<std::weak_ptr<spdlog::sinks::sink>> &
roah::Logger::Impl_::_getSinks()
{
    static std::vector<std::weak_ptr<spdlog::sinks::sink>> sinks;
    return sinks;
}

// ============================================================================================= //
// Constructor / Destructor
// ============================================================================================= //
roah::Logger::Impl_::Impl_(std::string && name)
    : name_{ std::move(name) }
    , level_{ _initial_level }
    , logger_{}
{
    this->setupLogger();
}

roah::Logger::Impl_::~Impl_() noexcept = default;

roah::Logger::Logger()
    : impl_{ Impl_::getOrCreateImpl("default") }
{}

roah::Logger::Logger(std::string name)
    : impl_{ Impl_::getOrCreateImpl(std::move(name)) }
{}

roah::Logger::Logger(const Logger &) {}

roah::Logger::Logger(Logger &&) noexcept = default;

roah::Logger::~Logger() noexcept = default;

roah::Logger &
roah::Logger::operator=(const Logger &)
    = default;

roah::Logger &
roah::Logger::operator=(Logger &&) noexcept
    = default;

// ============================================================================================= //
// operator bool / operator!
// ============================================================================================= //
roah::Logger::Impl_::operator bool() const noexcept
{
    return static_cast<bool>(this->logger_);
}

roah::Logger::operator bool() const noexcept
{
    return static_cast<bool>(*this->impl_);
}

bool
roah::Logger::operator!() const noexcept
{
    return !static_cast<bool>(*this->impl_);
}

// ============================================================================================= //
// [IMPL] setupLogger()
// ============================================================================================= //
void
roah::Logger::Impl_::setupLogger()
{
    if (!this->logger_)
    {
        // static に保存されている sink の weak_ptr を, すべて lock して logger を作る.
        const auto & sink_wptrs = Impl_::_getSinks();

        std::vector<std::shared_ptr<spdlog::sinks::sink>> sinks;
        sinks.reserve(sink_wptrs.size());
        for (const auto & sink_wptr : sink_wptrs)
        {
            if (auto sink = sink_wptr.lock(); sink)
            {
                sinks.emplace_back(std::move(sink));
            }
        }
        this->setupLogger(sinks);
    }
}

void
roah::Logger::Impl_::setupLogger(const std::vector<std::shared_ptr<spdlog::sinks::sink>> & sinks)
{
    if (!this->logger_)
    {
        // spdlog 側で同名の Logger がすでに作成されている場合はそれを利用する.
        this->logger_ = spdlog::get(this->name_);

        if (!this->logger_ && !sinks.empty())
        {
            this->logger_ = std::make_shared<spdlog::logger>(this->name_, sinks.begin(), sinks.end());
            this->logger_->set_level(spdlog::level::trace);
            this->level_ = _initial_level;
            spdlog::register_logger(this->logger_);
        }
    }
}

// ============================================================================================= //
// [PRIVATE] _log()
// ============================================================================================= //
void
roah::Logger::Impl_::log(const LogLevel               level,
                         const std::source_location & source_location,
                         const std::string_view       fmt,
                         const std::format_args &     args)
{
    // ログレベルフィルタ
    if (this->level_ > level)
    {
        return;
    }
    if (!this->logger_) [[unlikely]]
    {
        return;
    }

    // ログ出力 (spdlog)
    this->logger_->log(_converter(source_location), _converter(level), std::vformat(fmt, args));
}

void
roah::Logger::_log(const LogLevel               level,
                   const std::source_location & source_location,
                   const std::string_view       fmt,
                   const std::format_args &     args) const
{
    this->impl_->log(level, source_location, fmt, args);
}

// ============================================================================================= //
// flush()
// ============================================================================================= //
void
roah::Logger::Impl_::flush()
{
    if (!this->logger_) [[unlikely]]
    {
        return;
    }
    this->logger_->flush();
}

void
roah::Logger::flush() const
{
    this->impl_->flush();
}

// ============================================================================================= //
// [STATIC] getOrCreateImpl()
// ============================================================================================= //
std::shared_ptr<roah::Logger::Impl_>
roah::Logger::Impl_::getOrCreateImpl(const std::string_view name)
{
    std::string     name_str{ name };
    std::lock_guard lock{ Impl_::_getMutex() };
    auto &          ptr = Impl_::_getLoggers().try_emplace(name_str).first->second;
    if (!ptr)
    {
        ptr = std::make_shared<Impl_>(std::move(name_str));
    }
    return ptr;
}

// ============================================================================================= //
// [STATIC] initializeLogger()
// ============================================================================================= //
void
roah::Logger::Impl_::initialize(const std::string_view application_name, const LoggerInitializeArgs & args)
{
    if (_initialized)
    {
        return;
    }
    if (application_name.empty())
    {
        throw std::invalid_argument{ "Application name must not be empty." };
    }

    std::vector<spdlog::sink_ptr> sinks;
    sinks.reserve(3);

    _initial_level      = LogLevel::Off;
    const auto lower_fn = [](const LogLevel level) {
        if (_initial_level > level)
        {
            _initial_level = level;
        }
    };

    // ** Console sink
    if (args.console.level != LogLevel::Off)
    {
        const auto & s_stdout = sinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
        s_stdout->set_level(_converter(args.console.level));
        s_stdout->set_pattern("%^%8l%$ %H:%M:%S.%f t-%-5t [%n] %v");
        lower_fn(args.console.level);
    }

    // ** File sink
    if (args.log_file.level != LogLevel::Off)
    {
        // dirpath について, 空である場合はデフォルトとする.
        auto dir_path = args.log_file.dir_path;
        if (dir_path.empty())
        {
#if defined(ROAH_ARCH_WIN32)
            dir_path = std::filesystem::temp_directory_path();
#elif defined(ROAH_ARCH_LINUX)
            dir_path = "/var/log";
#else
#    error "Unsupported platform"
#endif
        }
        auto file_name = args.log_file.file_name;
        if (file_name.empty())
        {
            const auto pid = getCurrentProcessID();
            file_name      = std::format("{}_{}_{}.log",  //
                                    application_name,
                                    std::chrono::system_clock::now(),
                                    pid.toHexString());
        }

        const auto & s_file = sinks.emplace_back(
            std::make_shared<spdlog::sinks::basic_file_sink_mt>((dir_path / file_name).string(), true));
        s_file->set_level(_converter(args.log_file.level));
        s_file->set_pattern("%Y-%m-%d %H:%M:%S.%f (+%i) [%n] PID=%5P, TID=%5t, %@\n[%l] %v\n");
        lower_fn(args.log_file.level);
    }

#ifdef ROAH_LOGGER_ENABLE_WEBV_SINK
    if (args.webv.level != LogLevel::Off)
    {
        const auto & s_webv = sinks.emplace_back(std::make_shared<logger::impl::WebVSinkMt>(  //
            args.webv.server_host,
            args.webv.server_port,
            std::string{ application_name }));
        s_webv->set_level(_converter(args.webv.level));
        lower_fn(args.webv.level);
    }
#endif

    // sinks を weak_ptr で保管
    auto & sink_wptrs = Impl_::_getSinks();
    sink_wptrs.clear();
    sink_wptrs.reserve(sinks.size());
    for (const auto & sink : sinks)
    {
        sink_wptrs.emplace_back(sink);
    }

    // すでに作成されている logger 群を(遅延)初期化
    std::lock_guard lock{ Impl_::_getMutex() };
    for (const auto & logger : Impl_::_getLoggers() | std::views::values)
    {
        logger->setupLogger(sinks);
    }

    _initialized = true;
}

void
roah::Logger::_initialize(const std::string_view application_name, const LoggerInitializeArgs & args)
{
    Impl_::initialize(application_name, args);
}

roah::Logger roah::lg_default{ "default" };

void
roah::initializeLogger(const std::string_view application_name, const LoggerInitializeArgs & args)
{
    Logger::_initialize(application_name, args);
}

roah::LogLevel
roah::getLogLevelFromString(const std::string_view level_str)
{
    if (level_str == "trace")
    {
        return LogLevel::Trace;
    }
    else if (level_str == "debug")
    {
        return LogLevel::Debug;
    }
    else if (level_str == "info")
    {
        return LogLevel::Info;
    }
    else if (level_str == "warn")
    {
        return LogLevel::Warn;
    }
    else if (level_str == "error")
    {
        return LogLevel::Error;
    }
    else if (level_str == "critical")
    {
        return LogLevel::Critical;
    }
    else if (level_str == "off")
    {
        return LogLevel::Off;
    }
    else
    {
        throw std::invalid_argument{ "Invalid log level string: " + std::string{ level_str } };
    }
}

std::string_view
roah::getLogLevelString(const LogLevel log_level)
{
    switch (log_level)
    {
        using enum LogLevel;
    case Trace: return "trace";
    case Debug: return "debug";
    case Info: return "info";
    case Warn: return "warn";
    case Error: return "error";
    case Critical: return "critical";
    case Off: return "off";
    default: return "unknown";
    }
}
