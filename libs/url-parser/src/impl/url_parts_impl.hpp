// Copyright (C) 2026 White Atelier
// This software is released under the MIT License.
// See the LICENSE file in the project root for more details.

#ifndef ROAH_URL_PARSER_IMPL_URL_PARTS_IMPL_HPP
#define ROAH_URL_PARSER_IMPL_URL_PARTS_IMPL_HPP

#include "roah/url_parts.hpp"

#include <cstdint>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace roah::url_parser::impl {

class URLPartsImpl final : public URLParts
{
public:
    URLPartsImpl() noexcept;

    ~URLPartsImpl() noexcept override;

    URLPartsImpl(URLPartsImpl &&) noexcept;

    URLPartsImpl &
    operator=(URLPartsImpl &&) noexcept;

    const std::string &
    getParam(const std::string_view key) const override;

    std::int64_t
    getParamAsInt(const std::string_view key) const override;

    std::uint64_t
    getParamAsUInt(const std::string_view key) const override;

    const std::vector<std::string> &
    getQuery(const std::string_view key) const override;

    void
    setParam(const std::string_view key, const std::string_view value);

    void
    addQuery(const std::string_view key, const std::string_view value);

private:
    std::unordered_map<std::string, std::string>              params_;
    std::unordered_map<std::string, std::vector<std::string>> queries_;
};

}  // namespace roah::url_parser::impl

#endif
