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
/// 名前付きロガーのインスタンスです.
/// コンソールやファイルにログを出力します.
///
/// このログを使用する前に `initializeLogger()` による初期化が必要です.
///
/// ログ出力は `log()` メソッドではなく, マクロを利用してください.
///
/// このロガーは, 内部で spdlog を使用しています.
/// spdlog の薄い wrapper です.
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
    ///
    /// 無効なロガーインスタンスに対してログ出力を行った場合は, 何も行われません.
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
};

/// @brief デフォルトで用意されるロガー.
extern Logger lg_default;

/// @brief ロガーを初期化する.
///
/// @param application_name
///     アプリケーション名.
///
/// @param args
///    ロガー初期化用の引数.
void
initializeLogger(const std::string_view application_name, const LoggerInitializeArgs & args);

/// @brief 文字列から LogLevel を取得する.
///
/// 無効なログレベル文字列が指定された場合は, invalid argument 例外を送出します.
///
/// @param level_str
///     文字列で表現されたログレベル.
///     `trace`, `debug`, `info`, `warn`, `warning`, `err`, `error`, `crit`, `critical` のいずれか.
///
/// @return
///     変換された LogLevel. 変換できない場合は invalid argument 例外を送出します.
LogLevel
getLogLevelFromString(const std::string_view level_str);

/// @brief 文字列から LogLevel を取得する.
///
/// 無効なログレベル文字列が指定された場合に, 第二引数のデフォルトのログレベルを返します.
/// ログレベル文字列がユーザー指定の場合は, この関数を使用することを推奨します.
///
/// @param level_str
///     文字列で表現されたログレベル.
///     `trace`, `debug`, `info`, `warn`, `warning`, `err`, `error`, `crit`, `critical` のいずれか.
///
/// @param default_level
///     指定されたログレベル文字列が, 有効なログレベル文字列でない場合に返すデフォルトのログレベル.
///
/// @return
LogLevel
getLogLevelFromString(const std::string_view level_str, const LogLevel default_level) noexcept;

/// @brief 指定されたログレベルを文字列に変換する.
///
/// 下位から順に, `trace`, `debug`, `info`, `warning`, `error`, `critical`, `off` と変換されます.
///
/// @param log_level
///     ログレベル.
///
/// @return
///     ログレベル文字列.
std::string_view
getLogLevelString(const LogLevel log_level) noexcept;

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
