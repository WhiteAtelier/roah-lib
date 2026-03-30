// Copyright (C) 2026 White Atelier
// This software is released under the MIT License.
// See the LICENSE file in the project root for more details.

#ifndef ROAH_SERV_CONFIG_LOADER_HPP
#define ROAH_SERV_CONFIG_LOADER_HPP

#include <cstdint>
#include <filesystem>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>

namespace roah {

class ConfigUnloadedError final : public std::runtime_error
{
public:
    explicit ConfigUnloadedError();
};

class RequiredConfigKeyNotFoundError final : public std::runtime_error
{
public:
    explicit RequiredConfigKeyNotFoundError(const std::string_view category, const std::string_view key);
};

class InvalidConfigTypeError final : public std::runtime_error
{
public:
    explicit InvalidConfigTypeError(std::string_view category, std::string_view key, std::string_view expected_type);
};

class ConfigLoader
{
public:
    ConfigLoader() noexcept;
    ConfigLoader(const ConfigLoader &) = delete;
    ConfigLoader(ConfigLoader &&) noexcept;
    ~ConfigLoader() noexcept;

    ConfigLoader &
    operator=(const ConfigLoader &)
        = delete;
    ConfigLoader &
    operator=(ConfigLoader &&) noexcept;

    [[nodiscard]]
    explicit
    operator bool() const noexcept;

    bool
    operator!() const noexcept;

    bool
    load(const std::filesystem::path & path, std::string * const error_message = nullptr);

    [[nodiscard]]
    std::string
    getString(const std::string_view category, const std::string_view key, const std::string_view default_value) const;

    [[nodiscard]]
    std::string
    getString(const std::string_view category, const std::string_view key) const;

    [[nodiscard]]
    std::int64_t
    getInt(const std::string_view category, const std::string_view key, const std::int64_t default_value) const;

    [[nodiscard]]
    std::int64_t
    getInt(const std::string_view category, const std::string_view key) const;

    [[nodiscard]]
    bool
    getBool(const std::string_view category, const std::string_view key, const bool default_value) const;

    [[nodiscard]]
    bool
    getBool(const std::string_view category, const std::string_view key) const;

    [[nodiscard]]
    double
    getDouble(const std::string_view category, const std::string_view key, const double default_value) const;

    [[nodiscard]]
    double
    getDouble(const std::string_view category, const std::string_view key) const;

private:
    class Data_;
    std::unique_ptr<Data_> data_;
};

}  // namespace roah

#endif
