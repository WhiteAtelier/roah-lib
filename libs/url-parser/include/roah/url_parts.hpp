// Copyright (C) 2026 White Atelier
// This software is released under the MIT License.
// See the LICENSE file in the project root for more details.

#ifndef ROAH_URL_PARTS_HPP
#define ROAH_URL_PARTS_HPP

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace roah {

/// @brief URL のパラメータとクエリを表すクラスのインターフェース.
class URLParts
{
protected:
    explicit URLParts() noexcept = default;

public:
    virtual ~URLParts() noexcept = default;

    URLParts(URLParts &&) noexcept = default;

    URLParts &
    operator=(URLParts &&) noexcept
        = default;

    URLParts(const URLParts &) = delete;
    URLParts &
    operator=(const URLParts &)
        = delete;

    /// @brief 指定された key に関連付けられたパラメータ値を取得します.
    ///
    /// @throws URLParseError
    ///     指定した key が存在しない場合.
    ///
    /// @param key
    ///     パラメータの key 文字列.
    ///
    /// @return
    ///     key に関連付けられたパラメータ値への定数参照.
    virtual const std::string &
    getParam(const std::string_view key) const
        = 0;

    /// @brief 指定された key に関連付けられたパラメータ値を整数として取得します.
    ///
    /// @throws URLParseError
    ///     指定した key が存在しない場合, または値が整数に変換できない場合.
    ///
    /// @param key
    ///     パラメータの key 文字列.
    ///
    /// @return
    ///     key に関連付けられたパラメータ値を整数として返します.
    virtual std::int64_t
    getParamAsInt(const std::string_view key) const
        = 0;

    /// @brief 指定された key に関連付けられたパラメータ値を符号なし整数として取得します.
    ///
    /// @throws URLParseError
    ///     指定した key が存在しない場合, または値が符号なし整数に変換できない場合.
    ///
    /// @param key
    ///     パラメータの key 文字列.
    ///
    /// @return
    ///     key に関連付けられたパラメータ値を符号なし整数として返します.
    virtual std::uint64_t
    getParamAsUInt(const std::string_view key) const
        = 0;

    /// @brief 指定された key に関連付けられたクエリ値のリストを取得します.
    ///
    /// 存在しない key を指定した場合は, 空のリストを返します.
    ///
    /// @param key
    ///     クエリの key 文字列.
    ///
    /// @return
    ///     key に関連付けられたクエリ値のリストへの定数参照.
    virtual const std::vector<std::string> &
    getQuery(const std::string_view key) const
        = 0;
};

}  // namespace roah

#endif
