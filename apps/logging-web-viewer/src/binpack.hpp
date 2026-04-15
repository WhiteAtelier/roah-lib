// Copyright (C) 2026 White Atelier
// This software is released under the MIT License.
// See the LICENSE file in the project root for more details.

#ifndef ROAH_LOGGING_WEB_VIEWER_BINPACK_HPP
#define ROAH_LOGGING_WEB_VIEWER_BINPACK_HPP

#include <cstddef>
#include <string_view>
#include <utility>

namespace roah::logging::webv::embedded {

std::pair<const void *, std::size_t>
getFile(const std::string_view relpath);

}  // namespace roah::logging::webv::embedded

#endif
