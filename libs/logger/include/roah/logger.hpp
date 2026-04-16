// Copyright (C) 2026 White Atelier
// This software is released under the MIT License.
// See the LICENSE file in the project root for more details.

#ifndef ROAH_LOGGER_HPP
#define ROAH_LOGGER_HPP

#include "detail/logger_types.hpp"

#include <format>
#include <memory>
#include <source_location>
#include <string>
#include <string_view>

namespace roah {

/// @brief ロガークラス.
///
/// 事前に用意された名前付きロガー. コンソールやファイルにログを出力する.
///
/// このログを使用する前に `initializeLogger()` による初期化が必要.
/// また, このクラスの公開コンストラクタは用意されておらず,
///
/// ログ出力は `log()` メソッドではなく, マクロを利用すること.
class Logger
{
public:
    /// @brief Default logger constructor.
    explicit Logger();

    /// @brief 名前付きロガー.
    explicit Logger(std::string name);

    /// @brief Copy constructor.
    Logger(const Logger &);

    /// @brief Move constructor.
    Logger(Logger &&) noexcept;

    /// @brief Destructor.
    ~Logger() noexcept;

    /// @brief Copy assignment operator.
    Logger &
    operator=(const Logger &);

    /// @brief Move assignment operator.
    Logger &
    operator=(Logger &&) noexcept;

    /// @brief 有効な Logger インスタンスを保持しているか.
    operator bool() const noexcept;

    /// @brief 有効な Logger インスタンスを保持していないか.
    bool
    operator!() const noexcept;

    /// @brief ログを出力する.
    ///
    /// 以下のマクロを使用すること.
    ///
    /// - PASTEL_TRACE : Trace Level, デバッグ目的の詳細なログ.
    /// - PASTEL_LOG : Debug Level, デバッグ目的の一般的なログ.
    /// - PASTEL_INFO : Info Level, ユーザーに知らせるべき情報.
    /// - PASTEL_WARN : Warn Level, ユーザーに知らせるべき警告.
    /// - PASTEL_ERROR : Error Level, 続行可能なエラー.
    /// - PASTEL_CRITICAL : Critical Level, 致命的なエラー.
    template <typename... Args>
    void
    log(const LogLevel               level,
        const std::source_location & source_location,
        const std::string_view       fmt,
        const Args &... args) const;

    /// @brief ログをフラッシュする.
    ///
    /// Error 以上のログは, 即時フラッシュされる.
    /// それ以外のログのフラッシュのタイミングは不定であるため,
    /// この関数により強制的にフラッシュすることができる.
    void
    flush() const;

private:
    class Impl_;

    void
    _log(const LogLevel               level,
         const std::source_location & source_location,
         const std::string_view       fmt,
         const std::format_args &     args) const;

    static void
    _initialize(const std::string_view application_name, const LoggerInitializeArgs & args);

    std::shared_ptr<Impl_> impl_;

    // --- friends ---
    friend void
    initializeLogger(const std::string_view, const LoggerInitializeArgs &);
    friend void
    resetAllLoggers();
};

extern Logger lg_default;

void
initializeLogger(const std::string_view application_name, const LoggerInitializeArgs & args);

LogLevel
getLogLevelFromString(const std::string_view level_str);

std::string_view
getLogLevelString(const LogLevel log_level);

#if ROAH_LOG_LEVEL == 0
#    define ROAH_TRACE(logger, fmt, ...) \
        (logger).log(::roah::LogLevel::Trace, std::source_location::current(), fmt, ##__VA_ARGS__)
#else
#    define ROAH_TRACE(logger, fmt, ...)
#endif

#if ROAH_LOG_LEVEL <= 1
#    define ROAH_LOG(logger, fmt, ...) \
        (logger).log(::roah::LogLevel::Debug, std::source_location::current(), fmt, ##__VA_ARGS__)
#    define ROAH_INFO(logger, fmt, ...) \
        (logger).log(::roah::LogLevel::Info, std::source_location::current(), fmt, ##__VA_ARGS__)
#else
#    define ROAH_LOG(logger, fmt, ...)
#    define ROAH_INFO(logger, fmt, ...)
#endif

#if ROAH_LOG_LEVEL <= 2
#    define ROAH_WARN(logger, fmt, ...) \
        (logger).log(::roah::LogLevel::Warn, std::source_location::current(), fmt, ##__VA_ARGS__)
#    define ROAH_ERROR(logger, fmt, ...) \
        (logger).log(::roah::LogLevel::Error, std::source_location::current(), fmt, ##__VA_ARGS__)
#    define ROAH_CRITICAL(logger, fmt, ...) \
        (logger).log(::roah::LogLevel::Critical, std::source_location::current(), fmt, ##__VA_ARGS__)
#else
#    define ROAH_WARN(logger, fmt, ...)
#    define ROAH_ERROR(logger, fmt, ...)
#    define ROAH_CRITICAL(logger, fmt, ...)
#endif

}  // namespace roah

template <typename... Args>
void
roah::Logger::log(const LogLevel               level,
                  const std::source_location & source_location,
                  const std::string_view       fmt,
                  const Args &... args) const
{
    this->_log(level, source_location, fmt, std::make_format_args(args...));
}

#endif
