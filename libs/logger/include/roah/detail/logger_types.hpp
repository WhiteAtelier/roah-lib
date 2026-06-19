// Copyright (C) 2026 White Atelier
// This software is released under the MIT License.
// See the LICENSE file in the project root for more details.

#ifndef ROAH_LOGGER_INITIALIZE_ARGS_HPP
#define ROAH_LOGGER_INITIALIZE_ARGS_HPP

#include <cstdint>
#include <filesystem>
#include <string>

namespace roah {

/// @brief ログの重要度レベル.
enum struct LogLevel : int
{
    Trace = 0,  ///< トレース目的.
    Debug,      ///< デバッグ.
    Info,       ///< ユーザーに知らせるべき情報.
    Warn,       ///< ユーザーに知らせるべき警告.
    Error,      ///< 続行可能なエラー.
    Critical,   ///< 致命的なエラー.
    Off,        ///< (Off)
};

/// @brief ロガー出力先.
enum struct LoggerConsoleOutputTarget : std::uint8_t
{
    StdErr,  ///< 標準エラー出力 (stderr).
    StdOut,  ///< 標準出力 (stdout).
};

/// @brief Console ログの初期化設定.
struct LoggerConsoleInitializeArgs
{
    /// @brief Console ログの出力レベル.
    ///
    /// デフォルトは Info.
    /// Off に設定した場合はコンソールへログ出力を行わない.
    LogLevel level = LogLevel::Info;

    /// @brief Console ログの出力先.
    ///
    /// console_log_level が Off でない場合に, ログが出力される標準出力先を指定します.
    LoggerConsoleOutputTarget target = LoggerConsoleOutputTarget::StdErr;
};

/// @brief ログファイル用の初期化設定.
struct LoggerLogFileInitializeArgs
{
    /// @brief ファイルログへの出力レベル.
    ///
    /// デフォルトは Off.
    /// Off に設定した場合は, ログファイルを作成しません.
    LogLevel level = LogLevel::Off;

    /// @brief ログ出力先.
    ///
    /// デフォルトでは, Windows の場合 temporary ディレクトリ, Linux の場合は /var/log へ書き出します.
    std::filesystem::path dir_path;

    /// @brief ログファイル名.
    ///
    /// デフォルトでは, "{appname}_{datetime:yyyymmdd-HHMMSS}_{pid}.log"
    std::string file_name;
};

/// @brief LoggingWebView アプリ向けの初期化設定.
///
/// この機能を利用するには, ROAH_LOGGER_ENABLE_WEBV_SINK を有効にしてビルドする必要があります.
/// 有効になっていない環境では無視されます.
struct LoggerWebvInitializeArgs
{
    /// @brief ログ出力レベル.
    ///
    /// デフォルトでは Off.
    LogLevel level = LogLevel::Off;

    /// @brief サーバーホスト名.
    ///
    /// Websocket サーバーのホスト名を指定します.
    /// デフォルトでは "127.0.0.1".
    std::string server_host = "127.0.0.1";

    /// @brief サーバーポート番号.
    ///
    /// Websocket サーバーのポート番号を指定します.
    /// デフォルトは 32124 で, WebViewer のデフォルトポート番号と一致します.
    std::uint_fast16_t server_port = 32124;
};

/// @brief ロガーの初期化引数.
struct LoggerInitializeArgs
{
    /// @brief Console 用の設定.
    LoggerConsoleInitializeArgs console;

    /// @brief ログファイル用の設定.
    LoggerLogFileInitializeArgs log_file;

    /// @brief LoggingWebViewer アプリクライアントとしての設定.
    LoggerWebvInitializeArgs webv;
};

}  // namespace roah

#endif
