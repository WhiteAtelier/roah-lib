// Copyright (C) 2026 White Atelier
// This software is released under the MIT License.
// See the LICENSE file in the project root for more details.

#ifndef ROAH_PROCESS_ID_HPP
#define ROAH_PROCESS_ID_HPP

#include <cstdint>
#include <string>

namespace roah {

class ProcessID
{
public:
    explicit ProcessID();

    template <typename T>
    [[nodiscard]]
    explicit ProcessID(T id);

    ProcessID(const ProcessID &) noexcept;
    ProcessID(ProcessID &&) noexcept;
    ProcessID &
    operator=(const ProcessID &) noexcept;
    ProcessID &
    operator=(ProcessID &&) noexcept;

    ~ProcessID() noexcept;

    template <typename T>
    [[nodiscard]]  //
    operator T() const noexcept;

    [[nodiscard]]
    bool
    operator==(const ProcessID & rhs) const noexcept;

    [[nodiscard]]
    std::string
    toString() const;

    [[nodiscard]]
    std::string
    toHexString() const;

private:
    std::uint64_t id_;
};

[[nodiscard]]
ProcessID
getCurrentProcessID();

}  // namespace roah

#endif
