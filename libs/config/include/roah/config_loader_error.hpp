#ifndef ROAH_CONFIG_LOADER_ERROR_HPP
#define ROAH_CONFIG_LOADER_ERROR_HPP

#include <filesystem>
#include <stdexcept>
#include <string>
#include <string_view>

namespace roah {

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief Config 読み込みにおけるすべての例外の基底クラス.
///
class ConfigLoaderError : public std::runtime_error
{
public:
    /// @brief Constructor.
    ///
    /// このクラスを継承している特殊例外クラス以外の事由によるエラーを扱う.
    ///
    /// @param message
    ///     例外の詳細なメッセージ.
    explicit ConfigLoaderError(const std::string_view message);

    /// @brief 例外クラス名を取得する.
    std::string_view
    getErrorClass() const noexcept;

protected:
    /// @brief Constructor for derived classes.
    ///
    /// @param error_class
    ///     クラス名.
    ///     この string_view は string_view として保持されるため, 寿命には注意すること.
    ///     文字列リテラルの指定を推奨.
    ///
    /// @param message
    ///     メッセージ. ConfigLoaderError クラスのコンストラクタと同様に, クラス名を含む形式でフォーマットされる.
    explicit ConfigLoaderError(const std::string_view error_class, const std::string & message);

private:
    std::string_view error_class_;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief Config ファイルの読み込み時に発生した例外クラス.
///
class ConfigFileIOError final : public ConfigLoaderError
{
public:
    /// @brief Constructor.
    ///
    /// @param path
    ///     Config ファイルパス.
    ///
    /// @param error_message
    ///     例外の詳細なメッセージ.
    explicit ConfigFileIOError(const std::filesystem::path & path, const std::string_view error_message);
};

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief 設定ファイルの構文エラーを表す例外クラス.
///
class ConfigSyntaxError final : public ConfigLoaderError
{
public:
    /// @brief Constructor.
    ///
    /// @param path
    ///     Config ファイルパス.
    ///
    /// @param error_message
    ///     例外の詳細なメッセージ.
    explicit ConfigSyntaxError(const std::filesystem::path & path, const std::string_view error_message);
};

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief Config ファイルに必要なキーが見つからないことを表す例外クラス.
///
class ConfigRequiredKeyNotFoundError final : public ConfigLoaderError
{
public:
    /// @brief Constructor.
    ///
    /// @param category
    ///     必要としているキーが属するカテゴリ名.
    ///
    /// @param key
    ///     必要としているキー名.
    explicit ConfigRequiredKeyNotFoundError(const std::string_view category, const std::string_view key);
};

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief 指定したキーについて, 期待される型と異なる型の値が設定されていることを表す例外クラス.
///
class ConfigTypeError final : public ConfigLoaderError
{
public:
    /// @brief Constructor.
    ///
    /// @param category
    ///     キーが属するカテゴリ名.
    ///
    /// @param key
    ///     キー名.
    ///
    /// @param expected_type
    ///     期待していたキーの型を表す文字列.
    explicit ConfigTypeError(std::string_view category, std::string_view key, std::string_view expected_type);
};

}  // namespace roah

#endif