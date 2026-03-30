// Copyright (C) 2026 White Atelier
// This software is released under the MIT License.
// See the LICENSE file in the project root for more details.

#include "roah/config_loader.hpp"

#include <toml.hpp>

#include <cstdint>
#include <filesystem>
#include <format>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

namespace roah {

class ConfigLoader::Data_ final
{
public:
    Data_(toml::value && data);

    ~Data_() noexcept;

    const toml::value *
    get(const std::string_view category, const std::string_view key) const;

private:
    toml::value data_;
};

}  // namespace roah

roah::ConfigLoader::Data_::Data_(toml::value && data)
    : data_{ std::move(data) }
{}

roah::ConfigLoader::Data_::~Data_() noexcept = default;

const toml::value *
roah::ConfigLoader::Data_::get(const std::string_view category, const std::string_view key) const
{
    if (std::string category_str{ category }; this->data_.contains(category_str))
    {
        const auto & c_value = this->data_.at(category_str);
        if (std::string key_str{ key }; c_value.contains(key_str))
        {
            return &c_value.at(key_str);
        }
    }
    return nullptr;
}

roah::ConfigLoader::ConfigLoader() noexcept = default;

roah::ConfigLoader::ConfigLoader(ConfigLoader &&) noexcept = default;

roah::ConfigLoader::~ConfigLoader() noexcept = default;

roah::ConfigLoader &
roah::ConfigLoader::operator=(ConfigLoader &&) noexcept
    = default;

bool
roah::ConfigLoader::load(const std::filesystem::path & path, std::string * const error_message)
{
    this->data_.reset();
    try
    {
        this->data_ = std::make_unique<Data_>(toml::parse(path));
        return true;
    }
    catch (const std::exception & e)
    {
        if (error_message != nullptr)
        {
            *error_message = e.what();
        }
    }
    return false;
}

std::string
roah::ConfigLoader::getString(const std::string_view category,
                              const std::string_view key,
                              const std::string_view default_value) const
{
    if (this->data_ == nullptr)
    {
        return std::string{ default_value };
    }

    if (const auto * const value = this->data_->get(category, key); value != nullptr)
    {
        if (value->is_string())
        {
            return value->as_string();
        }
        throw InvalidConfigTypeError{ category, key, "string" };
    }
    return std::string{ default_value };
}

std::string
roah::ConfigLoader::getString(const std::string_view category, const std::string_view key) const
{
    if (const auto * const value = this->data_->get(category, key); value != nullptr)
    {
        if (value->is_string())
        {
            return value->as_string();
        }
        throw InvalidConfigTypeError{ category, key, "string" };
    }
    throw RequiredConfigKeyNotFoundError{ category, key };
}

std::int64_t
roah::ConfigLoader::getInt(const std::string_view category,
                           const std::string_view key,
                           const std::int64_t     default_value) const
{
    if (const auto * const value = this->data_->get(category, key); value != nullptr)
    {
        if (value->is_integer())
        {
            return value->as_integer();
        }
        throw InvalidConfigTypeError{ category, key, "integer" };
    }
    return default_value;
}

std::int64_t
roah::ConfigLoader::getInt(const std::string_view category, const std::string_view key) const
{
    if (const auto * const value = this->data_->get(category, key); value != nullptr)
    {
        if (value->is_integer())
        {
            return value->as_integer();
        }
        throw InvalidConfigTypeError{ category, key, "integer" };
    }
    throw RequiredConfigKeyNotFoundError{ category, key };
}

bool
roah::ConfigLoader::getBool(const std::string_view category, const std::string_view key, const bool default_value) const
{
    if (const auto * const value = this->data_->get(category, key); value != nullptr)
    {
        if (value->is_boolean())
        {
            return value->as_boolean();
        }
        throw InvalidConfigTypeError{ category, key, "boolean" };
    }
    return default_value;
}

bool
roah::ConfigLoader::getBool(const std::string_view category, const std::string_view key) const
{
    if (const auto * const value = this->data_->get(category, key); value != nullptr)
    {
        if (value->is_boolean())
        {
            return value->as_boolean();
        }
        throw InvalidConfigTypeError{ category, key, "boolean" };
    }
    throw RequiredConfigKeyNotFoundError{ category, key };
}

double
roah::ConfigLoader::getDouble(const std::string_view category,
                              const std::string_view key,
                              const double           default_value) const
{
    if (const auto * const value = this->data_->get(category, key); value != nullptr)
    {
        if (value->is_floating())
        {
            return value->as_floating();
        }
        throw InvalidConfigTypeError{ category, key, "number" };
    }
    return default_value;
}

double
roah::ConfigLoader::getDouble(const std::string_view category, const std::string_view key) const
{
    if (const auto * const value = this->data_->get(category, key); value != nullptr)
    {
        if (value->is_floating())
        {
            return value->as_floating();
        }
        throw InvalidConfigTypeError{ category, key, "number" };
    }
    throw RequiredConfigKeyNotFoundError{ category, key };
}

roah::RequiredConfigKeyNotFoundError::RequiredConfigKeyNotFoundError(const std::string_view category,
                                                                     const std::string_view key)
    : runtime_error{ std::format("Required config key not found: {}.{}", category, key) }
{}

roah::InvalidConfigTypeError::InvalidConfigTypeError(std::string_view category,
                                                     std::string_view key,
                                                     std::string_view expected_type)
    : runtime_error{
        std::format("Invalid config type for key: {}.{}. Expected type: {}", category, key, expected_type)
    }
{}

roah::ConfigUnloadedError::ConfigUnloadedError()
    : std::runtime_error{ "Config is not loaded." }
{}
