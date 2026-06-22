#ifndef ROAH_APINE_ENV_HPP
#define ROAH_APINE_ENV_HPP

#include <string>

namespace roah {

/// @brief 指定した環境変数の値を取得する.
///
/// @param name
///     環境変数の名前.
///
/// @param default_value
///     デフォルトの値. 環境変数が存在しない場合や値が空文字列の場合に返される値.
///
/// @return
///     取得された値. もしくはデフォルト値.
std::string
getEnv(const std::string & name, std::string default_value = "");

/// @brief 指定した環境変数について, true 値が設定されているかを取得する.
///
/// true 値とみなすのは, "1", "true", "yes", "on" (大文字小文字は区別しない) です.
/// これ以外の値では false と見なします.
///
/// @param name
///     環境変数の名前.
///
/// @param default_value
///     デフォルトの値. 環境変数が存在しないか, 空文字列の場合に返される値.
bool
getEnvSwitch(const std::string & name, const bool default_value = false);

/// @brief 環境変数を展開する.
///
/// @param str
///     環境変数を含む可能性のある文字列.
///
/// @return
///     環境変数が展開された文字列.
std::string
expandEnv(const std::string & str);

}  // namespace roah

#endif
