#include "roah/env.hpp"

#include <algorithm>
#include <cctype>
#include <cstdlib>

#ifdef ROAH_ARCH_WIN32
#    include <Windows.h>
#else
#    include <wordexp.h>
#endif

std::string
roah::getEnv(const std::string & name, std::string default_value)
{
    // std::string_view は null 終端を保証しないので, 一時的に std::string に変換する.
    const char * val = std::getenv(name.c_str());
    if (val == nullptr || val[0] == '\0')
    {
        return default_value;
    }
    return std::string{ val };
}

bool
roah::getEnvSwitch(const std::string & name, const bool default_value)
{
    auto val = getEnv(name);
    if (val.empty())
    {
        return default_value;
    }
    // 大文字小文字を区別せずに比較するため, 小文字に変換する.
    std::ranges::transform(val, val.begin(), [](const unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return val == "1" || val == "true" || val == "yes" || val == "on";
}

std::string
roah::expandEnv(const std::string & str)
{
#ifdef ROAH_ARCH_WIN32
    // ExpandEnvironmentStringsA は null 終端を含むバイト数を返す.
    // 1回目と2回目の呼び出しの間に環境変数が変化した場合に備えて, ループで再試行する.
    DWORD size = ::ExpandEnvironmentStringsA(str.c_str(), nullptr, 0);
    if (size == 0)
    {
        return str;
    }
    std::string result;
    DWORD       written = 0;
    do
    {
        result.resize(static_cast<std::size_t>(size) - 1ull);
        written = ::ExpandEnvironmentStringsA(str.c_str(), result.data(), size);
        if (written == 0)
        {
            return str;
        }
        size = written;
    }
    while (written > static_cast<DWORD>(result.size() + 1));

    // 書き込まれたバイト数 (null 終端含む) に合わせてサイズを調整する.
    if (written >= 1)
    {
        result.resize(static_cast<std::size_t>(written) - 1ull);
    }
    else
    {
        result.clear();
    }
    return result;
#else
    // POSIX 環境では wordexp(3) を使って環境変数を展開する.
    // WRDE_NOCMD でコマンド置換 (`...` や $(...)) を禁止する.
    wordexp_t we;
    if (::wordexp(str.c_str(), &we, WRDE_NOCMD) != 0)
    {
        return str;
    }
    std::string result;
    for (std::size_t j = 0; j < we.we_wordc; ++j)
    {
        if (j > 0)
        {
            result += ' ';
        }
        result += we.we_wordv[j];
    }
    ::wordfree(&we);
    return result;
#endif
}
