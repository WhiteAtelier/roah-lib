// Copyright (C) 2026 White Atelier
// This software is released under the MIT License.
// See the LICENSE file in the project root for more details.

#include "roah/config.hpp"

#include <toml.hpp>

#include <cstdint>
#include <filesystem>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

namespace roah {

class Config::AccessorImpl_ final : public Accessor_
{
public:
    using ItemType = toml::value;

    AccessorImpl_(const ItemType & data);

    ~AccessorImpl_() noexcept override = default;

    const std::string &
    getString(const std::string_view category, const std::string_view key) const override final;

    std::int64_t
    getInt(const std::string_view category, const std::string_view key) const override final;

    bool
    getBool(const std::string_view category, const std::string_view key) const override final;

    double
    getDouble(const std::string_view category, const std::string_view key) const override final;

private:
    const ItemType &
    _get(const std::string_view category, const std::string_view key) const;

    const ItemType & data_;
};

}  // namespace roah

roah::Config::AccessorImpl_::AccessorImpl_(const ItemType & data)
    : data_{ data }
{}

const std::string &
roah::Config::AccessorImpl_::getString(const std::string_view category, const std::string_view key) const
{
    return this->_get(category, key).as_string();
}

std::int64_t
roah::Config::AccessorImpl_::getInt(const std::string_view category, const std::string_view key) const
{
    return static_cast<std::int64_t>(this->_get(category, key).as_integer());
}

bool
roah::Config::AccessorImpl_::getBool(const std::string_view category, const std::string_view key) const
{
    return this->_get(category, key).as_boolean();
}

double
roah::Config::AccessorImpl_::getDouble(const std::string_view category, const std::string_view key) const
{
    return this->_get(category, key).as_floating();
}

const roah::Config::AccessorImpl_::ItemType &
roah::Config::AccessorImpl_::_get(const std::string_view category, const std::string_view key) const
{
    return this->data_.at(std::string{ category }).at(std::string{ key });
}

roah::Config::Config() noexcept = default;

roah::Config::Config(Config &&) noexcept = default;

roah::Config::~Config() noexcept = default;

roah::Config &
roah::Config::operator=(Config &&) noexcept
    = default;

bool
roah::Config::load(const std::filesystem::path & path)
{
    try
    {
        auto          data = toml::parse(path);
        AccessorImpl_ accessor{ data };
        this->_onLoad(accessor);
    }
    catch (const std::exception & e)
    {
        std::cerr << "Failed to load config file: " << path << std::endl;
        std::cerr << "Error: " << e.what() << std::endl;
        return false;
    }
    return true;
}
