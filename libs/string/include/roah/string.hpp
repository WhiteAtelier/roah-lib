// Copyright (C) 2026 White Atelier
// This software is released under the MIT License.
// See the LICENSE file in the project root for more details.

#ifndef ROAH_STRING_HPP
#define ROAH_STRING_HPP

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace roah {

/// @brief u8string から, utf-8 encoded string への変換.
///
/// 文字エンコードは変換しません. utf-8 バイト列を有した string への変換を行います.
/// 格納先の string が生成済みである場合, out 引数版を使用することをお勧めします.
///
/// @param u8str
///     変換対象の u8string_view.
///
/// @return
///     変換後の utf-8 エンコードされたバイト列を有した string インスタンス.
[[nodiscard]]
std::string
toString(const std::u8string_view u8str);

/// @brief u8string から, utf-8 encoded string への変換.
///
/// 文字エンコードは変換しません. utf-8 バイト列を有した string への変換を行います.
/// 格納先の string が生成済みである場合, out 引数で指定したインスタンスに格納します.
///
/// @param u8str
///     変換対象の u8string_view.
///
/// @param out
///     変換先の string インスタンスへの参照. 変換後の文字列が格納される.
///
/// @return
///     変換後の utf-8 エンコードされたバイト列を有した string インスタンス.
///     引数 out と同一のインスタンスを返す.
std::string &
toString(const std::u8string_view u8str, std::string & out);

/// @brief utf-8 encoded string から, u8string への変換.
///
/// 文字エンコードは変換しません. utf-8 バイト列を有した string から u8string への変換を行います.
/// 格納先の u8string が生成済みである場合, out 引数版を使用することをお勧めします.
///
/// @param str
///     変換対象の string_view.
///
/// @return
///     変換後の u8string インスタンス. utf-8 バイト列を有した u8string インスタンスが返されます.
[[nodiscard]]
std::u8string
toU8String(const std::string_view str);

/// @brief utf-8 encoded string から, u8string への変換.
///
/// 文字エンコードは変換しません. utf-8 バイト列を有した string から u8string への変換を行います.
/// 格納先の u8string が生成済みである場合, out 引数で指定したインスタンスに格納します.
///
/// @param str
///     変換対象の string_view.
///
/// @param out
///     変換先の u8string インスタンスへの参照. 変換後の文字列が格納される.
///
/// @return
///     変換後の u8string インスタンスへの参照.
///     引数 out と同一インスタンスを返す.
std::u8string &
toU8String(const std::string_view str, std::u8string & out);

/// @brief 文字列を指定した文字で分割する.
///
/// 指定した文字で文字列を分割し, 分割された部分文字列を引数 fn で指定した関数オブジェクトに渡します.
/// 空文字や区切り文字が連続する場合は, 空の部分文字列が生成されます.
///
/// @param str
///     分割対象の文字列.
///
/// @param delimiter
///     分割に使用する文字.
///
/// @param fn
///     分割された部分文字列を受け取る関数オブジェクト.
///     引数として std::string_view を受け取る必要があります.
///
/// @return
///    分割された部分文字列の数.
template <typename Fn>
std::size_t
splitString(const std::string_view str, const char delimiter, Fn && fn);

/// @overload
template <typename Fn>
std::size_t
splitString(const std::u8string_view str, const char8_t delimiter, Fn && fn);

/// @brief 文字列を指定された区切り文字で分割し, 結果をリストとして返す.
///
/// 空文字や区切り文字が連続する場合は, 空の部分文字列が生成されます.
///
/// @param str
///     分割する文字列.
///
/// @param delimiter
///     文字列を分割するために使用する区切り文字.
///
/// @return
///     分割結果を格納した部分文字列のリスト.
///     部分文字列は引数 str に対する参照となるので寿命に注意してください.
std::vector<std::string_view>
splitString(const std::string_view str, const char delimiter);

std::vector<std::u8string_view>
splitString(const std::u8string_view str, const char8_t delimiter);

}  // namespace roah

template <typename Fn>
std::size_t
roah::splitString(const std::string_view str, const char delimiter, Fn && fn)
{
    std::size_t count = 0;
    std::size_t beg   = 0;
    for (std::size_t end = str.find_first_of(delimiter, beg);  //
         end != std::string_view::npos;
         beg = end + 1, end = str.find_first_of(delimiter, beg))
    {
        fn(str.substr(beg, end - beg));
        ++count;
    }
    fn(str.substr(beg));
    return ++count;
}

template <typename Fn>
std::size_t
roah::splitString(const std::u8string_view str, const char8_t delimiter, Fn && fn)
{
    std::size_t count = 0;
    std::size_t beg   = 0;
    for (std::size_t end = str.find_first_of(delimiter, beg);  //
         end != std::u8string_view::npos;
         beg = end + 1, end = str.find_first_of(delimiter, beg))
    {
        fn(str.substr(beg, end - beg));
        ++count;
    }
    fn(str.substr(beg));
    return ++count;
}

#endif
