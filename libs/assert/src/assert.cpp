// Copyright (c) 2026 White Atelier
// This software is released under the MIT License.
// See the LICENSE file in the project root for more details.

#include "roah/assert.hpp"

#include <cstdint>
#include <format>
#include <iostream>
#include <source_location>
#include <stdexcept>
#include <string_view>

#ifdef ROAH_ASSERT_LOGGER_ENABLE
#    include "roah/logger.hpp"
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// AssertionError class implementation
//
///////////////////////////////////////////////////////////////////////////////////////////////////
roah::AssertionError::AssertionError(const std::string_view expr, const std::source_location & source_location)
    : std::logic_error{ std::format("AssertionError. {} - {}#{} {}()",
                                    expr,
                                    source_location.file_name(),
                                    source_location.line(),
                                    source_location.function_name()) }
    , expr_{ expr }
    , source_location_{ source_location }
{}

roah::AssertionError::AssertionError(const AssertionError &) = default;

roah::AssertionError::AssertionError(AssertionError &&) noexcept = default;

roah::AssertionError::~AssertionError() noexcept = default;

roah::AssertionError &
roah::AssertionError::operator=(const AssertionError &)
    = default;

roah::AssertionError &
roah::AssertionError::operator=(AssertionError &&) noexcept
    = default;

std::string_view
roah::AssertionError::getExpr() const noexcept
{
    return this->expr_;
}

const char *
roah::AssertionError::getFilename() const noexcept
{
    return this->source_location_.file_name();
}

std::uint_least32_t
roah::AssertionError::getLine() const noexcept
{
    return this->source_location_.line();
}

const char *
roah::AssertionError::getFunctionName() const noexcept
{
    return this->source_location_.function_name();
}

const std::source_location &
roah::AssertionError::getSourceLocation() const noexcept
{
    return this->source_location_;
}

void
roah::AssertionError::printMessage(const std::string_view expr, const std::source_location & source_location)
{
    AssertionError::printMessage(expr, source_location, "");
}

void
roah::AssertionError::printMessage(const std::string_view       expr,
                                   const std::source_location & source_location,
                                   const std::string_view       message)
{
#ifdef ROAH_ASSERT_LOGGER_ENABLE
    static const Logger logger{ "Roah-Assertion" };
    if (logger)
    {
        ROAH_CRITICAL(logger,
                      "Assertion Fault. {} - {}#{} {}()\n-- Message: {}",
                      expr,
                      source_location.file_name(),
                      source_location.line(),
                      source_location.function_name(),
                      message);
    }
    else
    {
#else
    {
        std::cerr << std::format("Assertion Fault. {} - {}#{} {}\n",
                                 expr,
                                 source_location.file_name(),
                                 source_location.line(),
                                 source_location.function_name())
                  << std::endl;
        if (!message.empty())
        {
            std::cerr << std::format("-- Message: {}\n", message);
        }
#endif
    }
}
