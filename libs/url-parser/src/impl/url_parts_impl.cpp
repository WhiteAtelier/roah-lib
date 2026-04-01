// Copyright (C) 2026 White Atelier
// This software is released under the MIT License.
// See the LICENSE file in the project root for more details.

#include "url_parts_impl.hpp"

#include "roah/url_parse_error.hpp"

#include <format>

roah::url_parser::impl::URLPartsImpl::URLPartsImpl() noexcept = default;

roah::url_parser::impl::URLPartsImpl::~URLPartsImpl() noexcept = default;

roah::url_parser::impl::URLPartsImpl::URLPartsImpl(URLPartsImpl &&) noexcept = default;

roah::url_parser::impl::URLPartsImpl &
roah::url_parser::impl::URLPartsImpl::operator=(URLPartsImpl &&) noexcept
    = default;

const std::string &
roah::url_parser::impl::URLPartsImpl::getParam(const std::string_view key) const
{
    if (const auto iter = this->params_.find(std::string{ key }); iter != this->params_.end())
    {
        return iter->second;
    }
    throw URLParseError{ std::format("Parameter \'{}\' not found", key) };
}

std::int64_t
roah::url_parser::impl::URLPartsImpl::getParamAsInt(const std::string_view key) const
{
    const auto & value = this->getParam(key);
    try
    {
        return std::stoll(value);
    }
    catch (const std::exception & e)
    {
        throw roah::URLParseError(std::format("Failed to convert parameter '{}' to int: {}", key, e.what()));
    }
}

std::uint64_t
roah::url_parser::impl::URLPartsImpl::getParamAsUInt(const std::string_view key) const
{
    const auto & value = this->getParam(key);
    try
    {
        return std::stoull(value);
    }
    catch (const std::exception & e)
    {
        throw roah::URLParseError(std::format("Failed to convert parameter '{}' to int: {}", key, e.what()));
    }
}

const std::vector<std::string> &
roah::url_parser::impl::URLPartsImpl::getQuery(const std::string_view key) const
{
    static std::vector<std::string> empty_vec;
    if (const auto iter = this->queries_.find(std::string{ key }); iter != this->queries_.end())
    {
        return iter->second;
    }
    return empty_vec;
}

void
roah::url_parser::impl::URLPartsImpl::setParam(const std::string_view key, const std::string_view value)
{
    this->params_.try_emplace(std::string{ key }, value);
}

void
roah::url_parser::impl::URLPartsImpl::addQuery(const std::string_view key, const std::string_view value)
{
    auto & query_vec = this->queries_.try_emplace(std::string{ key }).first->second;
    query_vec.emplace_back(value);
}
