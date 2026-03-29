// Copyright (C) 2026 White Atelier
// This software is released under the MIT License.
// See the LICENSE file in the project root for more details.

#ifndef ROAH_SERV_CONFIG_HPP
#define ROAH_SERV_CONFIG_HPP

#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>
#include <string_view>

namespace roah::serv {

class Config
{
protected:
    Config() noexcept;

public:
    Config(const Config &) = delete;
    Config(Config &&) noexcept;
    virtual ~Config() noexcept;

    Config &
    operator=(const Config &)
        = delete;
    Config &
    operator=(Config &&) noexcept;

    bool
    load(const std::filesystem::path & path);

protected:
    class Accessor_
    {
    public:
        Accessor_()                   = default;
        virtual ~Accessor_() noexcept = default;

        virtual const std::string &
        getString(const std::string_view category, const std::string_view key) const
            = 0;

        virtual std::int64_t
        getInt(const std::string_view category, const std::string_view key) const
            = 0;

        virtual bool
        getBool(const std::string_view category, const std::string_view key) const
            = 0;

        virtual double
        getDouble(const std::string_view category, const std::string_view key) const
            = 0;
    };

    virtual void
    _onLoad(const Accessor_ & accessor)
        = 0;

private:
    class AccessorImpl_;
};

}  // namespace roah::serv

#endif
