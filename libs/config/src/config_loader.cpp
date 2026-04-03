// Copyright (C) 2026 White Atelier
// This software is released under the MIT License.
// See the LICENSE file in the project root for more details.

#include "roah/config_loader.hpp"

#include "roah/config_error.hpp"

#include <toml.hpp>

#include <cstdint>
#include <filesystem>
#include <memory>
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

roah::ConfigLoader::operator bool() const noexcept
{
    return static_cast<bool>(this->data_);
}

bool
roah::ConfigLoader::operator!() const noexcept
{
    return !static_cast<bool>(this->data_);
}

void
roah::ConfigLoader::load(const std::filesystem::path & path)
{
    this->data_.reset();
    try
    {
        this->data_ = std::make_unique<Data_>(toml::parse(path));
    }
    catch (const toml::file_io_error & e)
    {
        throw ConfigFileIOError{ path, e.what() };
    }
    catch (const toml::syntax_error & e)
    {
        throw ConfigSyntaxError{ path, e.what() };
    }
    catch (const std::exception & e)
    {
        throw ConfigError{ e.what() };
    }
    catch (...)
    {
        throw ConfigError{ "Unknown error occurred while loading config file: " + path.string() };
    }
}

void
roah::ConfigLoader::reset() noexcept
{
    this->data_.reset();
}

std::string
roah::ConfigLoader::getString(const std::string_view category,
                              const std::string_view key,
                              const std::string_view default_value) const
{
    if (!this->data_)
    {
        return std::string{ default_value };
    }

    if (const auto * const value = this->data_->get(category, key); value != nullptr)
    {
        if (value->is_string())
        {
            return value->as_string();
        }
        throw ConfigTypeError{ category, key, "string" };
    }
    return std::string{ default_value };
}

std::string
roah::ConfigLoader::getString(const std::string_view category, const std::string_view key) const
{
    if (!this->data_)
    {
        throw ConfigRequiredKeyNotFoundError{ category, key };
    }

    if (const auto * const value = this->data_->get(category, key); value != nullptr)
    {
        if (value->is_string())
        {
            return value->as_string();
        }
        throw ConfigTypeError{ category, key, "string" };
    }
    throw ConfigRequiredKeyNotFoundError{ category, key };
}

std::int64_t
roah::ConfigLoader::getInt(const std::string_view category,
                           const std::string_view key,
                           const std::int64_t     default_value) const
{
    if (!this->data_)
    {
        return default_value;
    }

    if (const auto * const value = this->data_->get(category, key); value != nullptr)
    {
        if (value->is_integer())
        {
            return value->as_integer();
        }
        throw ConfigTypeError{ category, key, "integer" };
    }
    return default_value;
}

std::int64_t
roah::ConfigLoader::getInt(const std::string_view category, const std::string_view key) const
{
    if (!this->data_)
    {
        throw ConfigRequiredKeyNotFoundError{ category, key };
    }

    if (const auto * const value = this->data_->get(category, key); value != nullptr)
    {
        if (value->is_integer())
        {
            return value->as_integer();
        }
        throw ConfigTypeError{ category, key, "integer" };
    }
    throw ConfigRequiredKeyNotFoundError{ category, key };
}

bool
roah::ConfigLoader::getBool(const std::string_view category, const std::string_view key, const bool default_value) const
{
    if (!this->data_)
    {
        return default_value;
    }

    if (const auto * const value = this->data_->get(category, key); value != nullptr)
    {
        if (value->is_boolean())
        {
            return value->as_boolean();
        }
        throw ConfigTypeError{ category, key, "boolean" };
    }
    return default_value;
}

bool
roah::ConfigLoader::getBool(const std::string_view category, const std::string_view key) const
{
    if (!this->data_)
    {
        throw ConfigRequiredKeyNotFoundError{ category, key };
    }

    if (const auto * const value = this->data_->get(category, key); value != nullptr)
    {
        if (value->is_boolean())
        {
            return value->as_boolean();
        }
        throw ConfigTypeError{ category, key, "boolean" };
    }
    throw ConfigRequiredKeyNotFoundError{ category, key };
}

double
roah::ConfigLoader::getDouble(const std::string_view category,
                              const std::string_view key,
                              const double           default_value) const
{
    if (!this->data_)
    {
        return default_value;
    }

    if (const auto * const value = this->data_->get(category, key); value != nullptr)
    {
        if (value->is_floating())
        {
            return value->as_floating();
        }
        throw ConfigTypeError{ category, key, "number" };
    }
    return default_value;
}

double
roah::ConfigLoader::getDouble(const std::string_view category, const std::string_view key) const
{
    if (!this->data_)
    {
        throw ConfigRequiredKeyNotFoundError{ category, key };
    }

    if (const auto * const value = this->data_->get(category, key); value != nullptr)
    {
        if (value->is_floating())
        {
            return value->as_floating();
        }
        throw ConfigTypeError{ category, key, "number" };
    }
    throw ConfigRequiredKeyNotFoundError{ category, key };
}
