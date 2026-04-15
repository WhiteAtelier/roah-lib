// Copyright (C) 2026 White Atelier
// This software is released under the MIT License.
// See the LICENSE file in the project root for more details.

#include "roah/process_id.hpp"

#if defined(ROAH_ARCH_WIN32)
#    include <Windows.h>
#elif defined(ROAH_ARCH_LINUX)
#    include <unistd.h>
#else
#    error "Unsupported platform"
#endif

#include <bit>
#include <format>
#include <type_traits>

namespace {
#if defined(ROAH_ARCH_WIN32)
using NativePIDType = DWORD;
#elif defined(ROAH_ARCH_LINUX)
using NativePIDType = pid_t;
#endif
}  // namespace

roah::ProcessID::ProcessID()
    : id_{ 0 }
{}

template <>
roah::ProcessID::ProcessID(NativePIDType id)
    : id_{ 0 }
{
    if constexpr (std::is_unsigned_v<NativePIDType>)
    {
        this->id_ = static_cast<std::uint64_t>(id);
    }
    else
    {
        this->id_ = std::bit_cast<std::uint64_t>(static_cast<std::int64_t>(id));
    }
}

roah::ProcessID::ProcessID(const ProcessID &) noexcept = default;

roah::ProcessID::ProcessID(ProcessID && m) noexcept
    : id_{ m.id_ }
{
    m.id_ = 0;
}

roah::ProcessID &
roah::ProcessID::operator=(const ProcessID &) noexcept
    = default;

roah::ProcessID &
roah::ProcessID::operator=(ProcessID && rhs) noexcept
{
    if (this != &rhs)
    {
        id_     = rhs.id_;
        rhs.id_ = 0;
    }
    return *this;
}

roah::ProcessID::~ProcessID() noexcept = default;

template <>
roah::ProcessID::operator NativePIDType() const noexcept
{
    if constexpr (std::is_unsigned_v<NativePIDType>)
    {
        return static_cast<NativePIDType>(this->id_);
    }
    else
    {
        return static_cast<NativePIDType>(std::bit_cast<std::int64_t>(this->id_));
    }
}

bool
roah::ProcessID::operator==(const ProcessID & rhs) const noexcept
{
    return this->id_ == rhs.id_;
}

std::string
roah::ProcessID::toString() const
{
    return std::to_string(static_cast<NativePIDType>(*this));
}

std::string
roah::ProcessID::toHexString() const
{
    return std::format("{:x}", static_cast<NativePIDType>(*this));
}

roah::ProcessID
roah::getCurrentProcessID()
{
#if defined(ROAH_ARCH_WIN32)
    return ProcessID{ GetCurrentProcessId() };
#elif defined(ROAH_ARCH_LINUX)
    return ProcessID{ getpid() };
#else
#    error "Unsupported platform"
#endif
}
