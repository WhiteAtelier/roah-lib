#include "roah/config_error.hpp"

#include <filesystem>
#include <format>
#include <stdexcept>
#include <string>
#include <string_view>

roah::ConfigError::ConfigError(const std::string_view message)
    : std::runtime_error{ std::format("[ConfigError] {}", message) }
    , error_class_{ "ConfigError" }
{}

std::string_view
roah::ConfigError::getErrorClass() const noexcept
{
    return this->error_class_;
}

roah::ConfigError::ConfigError(const std::string_view error_class, const std::string & message)
    : std::runtime_error{ std::format("[{}] {}", error_class, message) }
    , error_class_{ error_class }
{}

roah::ConfigFileIOError::ConfigFileIOError(const std::filesystem::path & path, const std::string_view error_message)
    : ConfigError{ "ConfigFileIOError", std::format("Config file I/O error: {}, {}", path.string(), error_message) }
{}

roah::ConfigSyntaxError::ConfigSyntaxError(const std::filesystem::path & path, const std::string_view error_message)
    : ConfigError{ "ConfigSyntaxError", std::format("Config syntax error: {}, {}", path.string(), error_message) }
{}

roah::ConfigRequiredKeyNotFoundError::ConfigRequiredKeyNotFoundError(const std::string_view category,
                                                                     const std::string_view key)
    : ConfigError{ "ConfigRequiredKeyNotFoundError",
                   std::format("Required config key not found: {}.{}", category, key) }
{}

roah::ConfigTypeError::ConfigTypeError(std::string_view category, std::string_view key, std::string_view expected_type)
    : ConfigError{ "ConfigTypeError",
                   std::format("Invalid config type for key: {}.{}. Expected type: {}", category, key, expected_type) }
{}
