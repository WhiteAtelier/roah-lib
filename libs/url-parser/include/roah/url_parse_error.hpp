// Copyright (C) 2026 White Atelier
// This software is released under the MIT License.
// See the LICENSE file in the project root for more details.

#ifndef ROAH_URL_PARSE_ERROR_HPP
#define ROAH_URL_PARSE_ERROR_HPP

#include <stdexcept>
#include <string_view>

namespace roah {

/// @brief URL パースエラーを表す例外クラス.
class URLParseError : public std::runtime_error
{
public:
    using std::runtime_error::runtime_error;
};

}  // namespace roah

#endif
