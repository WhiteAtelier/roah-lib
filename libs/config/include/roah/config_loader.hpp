// Copyright (C) 2026 White Atelier
// This software is released under the MIT License.
// See the LICENSE file in the project root for more details.

#ifndef ROAH_SERV_CONFIG_LOADER_HPP
#define ROAH_SERV_CONFIG_LOADER_HPP

#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>
#include <string_view>

namespace roah {

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief Config ファイルの読み込みユーティリティクラス.
///
/// このクラスが対応する Config ファイルは TOML 形式です.
/// ファイルの拡張子などの指定はありません.
///
class ConfigLoader
{
public:
    /// @brief Constructor.
    ///
    /// 空の ConfigLoader を構築する.
    ConfigLoader() noexcept;

    /// @brief Descriptor.
    ~ConfigLoader() noexcept;

    /// @brief Move constructor.
    ConfigLoader(ConfigLoader &&) noexcept;

    /// @brief Move assignment operator.
    ConfigLoader &
    operator=(ConfigLoader &&) noexcept;

    ConfigLoader(const ConfigLoader &) = delete;
    ConfigLoader &
    operator=(const ConfigLoader &)
        = delete;

    /// @brief 何らかの有効なデータを保持しているかどうかを取得する.
    ///
    /// インスタンスの作成直後は false を返す.
    /// load() によって有効なデータが読み込まれている場合は true を返す.
    [[nodiscard]]
    explicit
    operator bool() const noexcept;

    /// @brief 有効なデータを保持していないかどうかを取得する.
    ///
    /// operator bool() の負論理を返す.
    [[nodiscard]]
    bool
    operator!() const noexcept;

    /// @brief 指定した path を読み込む.
    ///
    /// @param path
    ///     読み込む Config ファイルのパス.
    ///
    /// @throw ConfigFileIOError
    ///     指定した path についてファイル読み込み時にエラーが発生した場合.
    ///
    /// @throw ConfigSyntaxError
    ///     指定した path について TOML の構文エラーが発生した場合.
    ///
    /// @throw ConfigError
    ///     その他の理由で Config ファイルの読み込みに失敗した場合.
    void
    load(const std::filesystem::path & path);

    /// @brief 保持しているデータをリセットする.
    ///
    /// 有効なデータを保持していない場合は, 何もしません.
    void
    reset() noexcept;

    /// @brief 指定したカテゴリとキーに対応する文字列値を取得する.
    ///
    /// 指定したキーについて設定が記載されていない場合は, デフォルト値を返す.
    /// ただし, 指定したキーが存在するが値の型が文字列でない場合は例外を投げる.
    ///
    /// @param category
    ///     取得する設定値のカテゴリ.
    ///
    /// @param key
    ///     取得する設定値のキー.
    ///
    /// @param default_value
    ///     キーが見つからない場合に返されるデフォルト値.
    ///
    /// @throw ConfigTypeError
    ///     指定したキーに対応する値が文字列でない場合.
    ///
    /// @return
    ///     指定したキーに対応する文字列値. キーが存在しない場合はデフォルト値.
    [[nodiscard]]
    std::string
    getString(const std::string_view category, const std::string_view key, const std::string_view default_value) const;

    /// @brief 指定したカテゴリとキーに対応する文字列値を必須として取得する.
    ///
    /// 指定したキーが必ず明記されていることを期待して, 値の取得を試みる.
    ///
    /// @param category
    ///     取得する設定値のカテゴリ.
    ///
    /// @param key
    ///     取得する設定値のキー.
    ///
    /// @throw ConfigTypeError
    ///     指定したキーに対応する値が文字列でない場合.
    ///
    /// @return
    ///     指定したキーに対応する文字列値. キーが存在しない場合はデフォルト値.
    [[nodiscard]]
    std::string
    getString(const std::string_view category, const std::string_view key) const;

    /// @brief 指定したカテゴリとキーに対応する整数値を取得する.
    ///
    /// 指定したキーについて設定が記載されていない場合は, デフォルト値を返す.
    /// ただし, 指定したキーが存在するが値の型が整数でない場合は例外を投げる.
    ///
    /// @param category
    ///     取得する設定値のカテゴリ.
    ///
    /// @param key
    ///     取得する設定値のキー.
    ///
    /// @param default_value
    ///     キーが見つからない場合に返されるデフォルト値.
    ///
    /// @throw ConfigTypeError
    ///     指定したキーに対応する値が整数でない場合.
    ///
    /// @return
    ///     指定したキーに対応する整数値. キーが存在しない場合はデフォルト値.
    [[nodiscard]]
    std::int64_t
    getInt(const std::string_view category, const std::string_view key, const std::int64_t default_value) const;

    /// @brief 指定したカテゴリとキーに対応する整数値を必須として取得する.
    ///
    /// 指定したキーが必ず明記されていることを期待して, 値の取得を試みる.
    ///
    /// @param category
    ///     取得する設定値のカテゴリ.
    ///
    /// @param key
    ///     取得する設定値のキー.
    ///
    /// @throw ConfigTypeError
    ///     指定したキーに対応する値が整数でない場合.
    ///
    /// @return
    ///     指定したキーに対応する整数値.
    [[nodiscard]]
    std::int64_t
    getInt(const std::string_view category, const std::string_view key) const;

    /// @brief 指定したカテゴリとキーに対応するブール値を取得する.
    ///
    /// 指定したキーについて設定が記載されていない場合は, デフォルト値を返す.
    /// ただし, 指定したキーが存在するが値の型がブール値でない場合は例外を投げる.
    ///
    /// @param category
    ///     取得する設定値のカテゴリ.
    ///
    /// @param key
    ///     取得する設定値のキー.
    ///
    /// @param default_value
    ///     キーが見つからない場合に返されるデフォルト値.
    ///
    /// @throw ConfigTypeError
    ///     指定したキーに対応する値がブール値でない場合.
    ///
    /// @return
    ///     指定したキーに対応するブール値. キーが存在しない場合はデフォルト値.
    [[nodiscard]]
    bool
    getBool(const std::string_view category, const std::string_view key, const bool default_value) const;

    /// @brief 指定したカテゴリとキーに対応するブール値を必須として取得する.
    ///
    /// 指定したキーが必ず明記されていることを期待して, 値の取得を試みる.
    ///
    /// @param category
    ///     取得する設定値のカテゴリ.
    ///
    /// @param key
    ///     取得する設定値のキー.
    ///
    /// @throw ConfigTypeError
    ///     指定したキーに対応する値がブール値でない場合.
    ///
    /// @return
    ///     指定したキーに対応するブール値.
    [[nodiscard]]
    bool
    getBool(const std::string_view category, const std::string_view key) const;

    /// @brief 指定したカテゴリとキーに対応する浮動小数点数値を取得する.
    ///
    /// 指定したキーについて設定が記載されていない場合は, デフォルト値を返す.
    /// ただし, 指定したキーが存在するが値の型が浮動小数点数でない場合は例外を投げる.
    ///
    /// @param category
    ///     取得する設定値のカテゴリ.
    ///
    /// @param key
    ///     取得する設定値のキー.
    ///
    /// @param default_value
    ///     キーが見つからない場合に返されるデフォルト値.
    ///
    /// @throw ConfigTypeError
    ///     指定したキーに対応する値が浮動小数点数でない場合.
    ///
    /// @return
    ///     指定したキーに対応する浮動小数点数値. キーが存在しない場合はデフォルト値.
    [[nodiscard]]
    double
    getDouble(const std::string_view category, const std::string_view key, const double default_value) const;

    /// @brief 指定したカテゴリとキーに対応する浮動小数点数値を必須として取得する.
    ///
    /// 指定したキーが必ず明記されていることを期待して, 値の取得を試みる.
    ///
    /// @param category
    ///     取得する設定値のカテゴリ.
    ///
    /// @param key
    ///     取得する設定値のキー.
    ///
    /// @throw ConfigTypeError
    ///     指定したキーに対応する値が浮動小数点数でない場合.
    ///
    /// @return
    ///     指定したキーに対応する浮動小数点数値.
    [[nodiscard]]
    double
    getDouble(const std::string_view category, const std::string_view key) const;

private:
    class Data_;
    std::unique_ptr<Data_> data_;
};

}  // namespace roah

#endif
