// Copyright (C) 2026 White Atelier
// This software is released under the MIT License.
// See the LICENSE file in the project root for more details.

#include "roah/string.hpp"

#include <cstddef>
#include <cstring>
#include <string>
#include <string_view>
#include <vector>

std::string
roah::toString(const std::u8string_view u8str)
{
    std::string result;
    toString(u8str, result);
    return result;
}

std::string &
roah::toString(const std::u8string_view u8str, std::string & out)
{
    out.resize(u8str.size());
    std::memcpy(out.data(), u8str.data(), u8str.size());
    return out;
}

std::u8string
roah::toU8String(const std::string_view str)
{
    std::u8string result;
    toU8String(str, result);
    return result;
}

std::u8string &
roah::toU8String(const std::string_view str, std::u8string & out)
{
    out.resize(str.size());
    std::memcpy(out.data(), str.data(), str.size());
    return out;
}

std::vector<std::string_view>
roah::splitString(const std::string_view str, const char delimiter)
{
    std::vector<std::string_view> ret;
    splitString(str, delimiter, [&ret](const std::string_view part) { ret.emplace_back(part); });
    return ret;
}

std::vector<std::u8string_view>
roah::splitString(const std::u8string_view str, const char8_t delimiter)
{
    std::vector<std::u8string_view> ret;
    splitString(str, delimiter, [&ret](const std::u8string_view part) { ret.emplace_back(part); });
    return ret;
}
