// Copyright (C) 2026 White Atelier
// This software is released under the MIT License.
// See the LICENSE file in the project root for more details.

#ifndef ROAH_URL_PARSER_HPP
#define ROAH_URL_PARSER_HPP

#include <memory>
#include <string_view>

namespace roah {

class URLParts;

/// @brief URL 文字列を解析するクラス.
///
/// 1クラスにつき 1つの URL パターンを扱います.
class URLParser
{
public:
    /// @brief Constructor.
    ///
    /// "/path/to/resource?query=string#fragment" のような URL に対して, パターンをもとに解析します.
    /// 今のところ, パスとクエリのみに対応しています.
    /// スキーマ, ホスト, ポート, フラグメントはサポートしていません.
    ///
    /// 引数 path_pattern については, パラメータを含む URL パターンを指定します.
    /// ":param_name" のようにコロンで始まる部分は, "/" 区切りの文字列とマッチングします.
    /// "*param_name" のようにアスタリスクで始まる部分は, URL の残りの部分とマッチングします.
    /// 例えば, "/user-folder/:user_id/*file_path" といったような文字列を path_pattern として指定できます.
    ///
    /// 今のところ, / 区切りの単語全体について, ":" や "*" を使用できます.
    /// 例えば, "/file:file_id" (例えば /file123) のようなパターンはサポートしていません.
    ///
    /// @param path_pattern
    ///     The URL path pattern to parse.
    URLParser(const std::string_view & path_pattern);

    URLParser(URLParser &&) noexcept;

    URLParser &
    operator=(URLParser &&) noexcept;

    ~URLParser() noexcept;

    URLParser(const URLParser &) = delete;
    URLParser &
    operator=(const URLParser &)
        = delete;

    /// @brief 指定した URL パスがパターンにマッチするかどうかを判定する.
    ///
    /// マッチする場合は URLParts オブジェクトを返します.
    ///
    /// @param url_path
    ///     判定対象の URL パス文字列.
    ///
    /// @return
    ///     マッチングした場合, 結果を格納した URLParts インスタンス.
    ///     マッチングに失敗した場合, nullptr.
    std::unique_ptr<URLParts>
    match(const std::string_view url_path) const;

private:
    class Impl_;
    std::unique_ptr<Impl_> impl_;
};

}  // namespace roah

#endif
