#include "roah/config_loader_error.hpp"

#include <filesystem>
#include <format>
#include <stdexcept>
#include <string>
#include <string_view>

roah::ConfigLoaderError::ConfigLoaderError(const std::string_view message)
    : std::runtime_error{ std::format("[ConfigLoaderError] {}", message) }
    , error_class_{ "ConfigLoaderError" }
{}

std::string_view
roah::ConfigLoaderError::getErrorClass() const noexcept
{
    return this->error_class_;
}

roah::ConfigLoaderError::ConfigLoaderError(const std::string_view error_class, const std::string & message)
    : std::runtime_error{ std::format("[{}] {}", error_class, message) }
    , error_class_{ error_class }
{}

roah::ConfigFileIOError::ConfigFileIOError(const std::filesystem::path & path, const std::string_view error_message)
    : ConfigLoaderError{ "ConfigFileIOError",
                         std::format("Config file I/O error: {}, {}", path.string(), error_message) }
{}

roah::ConfigSyntaxError::ConfigSyntaxError(const std::filesystem::path & path, const std::string_view error_message)
    : ConfigLoaderError{ "ConfigSyntaxError", std::format("Config syntax error: {}, {}", path.string(), error_message) }
{}

roah::ConfigRequiredKeyNotFoundError::ConfigRequiredKeyNotFoundError(const std::string_view category,
                                                                     const std::string_view key)
    : ConfigLoaderError{ "ConfigRequiredKeyNotFoundError",
                         std::format("Required config key not found: {}.{}", category, key) }
{}

roah::ConfigTypeError::ConfigTypeError(std::string_view category, std::string_view key, std::string_view expected_type)
    : ConfigLoaderError{
        "ConfigTypeError",
        std::format("Invalid config type for key: {}.{}. Expected type: {}", category, key, expected_type)
    }
{}
