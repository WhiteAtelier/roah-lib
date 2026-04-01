// Copyright (C) 2026 White Atelier
// This software is released under the MIT License.
// See the LICENSE file in the project root for more details.

#include "roah/url_parser.hpp"

#include "impl/fixed_word.hpp"
#include "impl/key_word.hpp"
#include "impl/path_words.hpp"
#include "impl/url_parts_impl.hpp"
#include "impl/word.hpp"

#include <vector>

namespace roah {

class URLParser::Impl_ final
{
public:
    explicit Impl_(const std::string_view & path_pattern);

    Impl_(Impl_ &&) noexcept;

    Impl_ &
    operator=(Impl_ &&) noexcept;

    ~Impl_() noexcept;

    std::unique_ptr<URLParts>
    match(const std::string_view url_path) const;

private:
    std::vector<std::unique_ptr<url_parser::impl::Word>> pattern_words_;
};

}  // namespace roah

namespace {
const auto split_fn = [](const std::string_view src, const char sep, auto push_fn) {
    if (src.empty())
    {
        push_fn(std::string_view{});
        return;
    }

    std::size_t beg = 0;
    for (auto end = src.find_first_of(sep);  //
         end != std::string_view::npos;
         beg = end + 1, end = src.find_first_of(sep, beg))
    {
        push_fn(src.substr(beg, end - beg));
    }
    // 最後の部分も追加する.
    if (beg < src.size())
    {
        push_fn(src.substr(beg));
    }
};
}  // namespace

// ============================================================================================= //
// Constructor / Destructor / operators
// ============================================================================================= //
roah::URLParser::Impl_::Impl_(const std::string_view & path_pattern)
{
    split_fn(path_pattern, '/', [&](const std::string_view word) {
        if (!word.empty())
        {
            if (word[0] == ':')
            {
                this->pattern_words_.emplace_back(std::make_unique<url_parser::impl::KeyWord>(word.substr(1)));
                return;
            }
            if (word[0] == '*')
            {
                this->pattern_words_.emplace_back(std::make_unique<url_parser::impl::PathWords>(word.substr(1)));
                return;
            }
        }
        // それ以外の場合は, FixedWord として扱う.
        this->pattern_words_.emplace_back(std::make_unique<url_parser::impl::FixedWord>(word));
    });
}

roah::URLParser::Impl_::Impl_(Impl_ &&) noexcept = default;

roah::URLParser::Impl_ &
roah::URLParser::Impl_::operator=(Impl_ &&) noexcept
    = default;

roah::URLParser::Impl_::~Impl_() noexcept = default;

roah::URLParser::URLParser(const std::string_view & path_pattern)
    : impl_{ std::make_unique<Impl_>(path_pattern) }
{}

roah::URLParser::~URLParser() noexcept = default;

roah::URLParser::URLParser(URLParser &&) noexcept = default;

roah::URLParser &
roah::URLParser::operator=(URLParser &&) noexcept
    = default;

// ============================================================================================= //
// Constructor / Destructor / operators
// ============================================================================================= //
std::unique_ptr<roah::URLParts>
roah::URLParser::Impl_::match(const std::string_view url_path) const
{
    thread_local std::vector<std::string_view> words;
    words.clear();

    // まずすべて "/" 区切りで分割する.
    split_fn(url_path, '/', [&](const std::string_view word) { words.emplace_back(word); });

    // words の最後に, クエリ文字列が含まれている可能性がある.
    std::string_view query_str;
    if (!words.empty())
    {
        query_str = words.back();
        auto beg  = query_str.find_first_of('?');
        if (beg != std::string_view::npos)
        {
            words.back() = query_str.substr(0, beg);
            query_str    = query_str.substr(beg + 1);
        }
        else
        {
            query_str = std::string_view{};
        }
    }

    // path 末尾が "/" で終わる, つまり words の最後の要素が空文字列の場合は, それを削除する.
    // ただし, ルートパス "/" の場合 words = [""} となるため, 要素が1つのときは削除しない.
    if (words.size() > 1 && words.back().empty())
    {
        words.pop_back();
    }

    // クエリ分割する前に, パスマッチングを行う
    auto match_iter = words.cbegin();
    for (const auto & pattern_word : this->pattern_words_)
    {
        if (!pattern_word->match(match_iter, words.cend()))
        {
            // マッチングに失敗した場合は, nullptr を返す.
            return nullptr;
        }
    }
    // パターンより URL のセグメント数が多い場合はマッチしない.
    if (match_iter != words.cend())
    {
        return nullptr;
    }

    // パスマッチングに成功した場合は, URLPartsImpl を作成して, パラメータをセットする.
    auto parts = std::make_unique<url_parser::impl::URLPartsImpl>();
    for (auto iter = words.cbegin(); const auto & pattern_word : this->pattern_words_)
    {
        pattern_word->pick(iter, words.cend(), parts.get());
    }

    // マッチングしたのでクエリ文字列を分割する.
    split_fn(query_str, '&', [&](const std::string_view query) {
        // クエリ文字列を "=" 区切りで分割する.
        const auto eq_pos = query.find_first_of('=');
        if (eq_pos != std::string_view::npos)
        {
            const auto key   = query.substr(0, eq_pos);
            const auto value = query.substr(eq_pos + 1);
            // さらに value について "," 区切りで分割する.
            split_fn(value, ',', [&](const std::string_view word) { parts->addQuery(key, word); });
        }
        else
        {
            // "=" がない場合は, キーのみのクエリとみなす.
            parts->addQuery(query, std::string_view{});
        }
    });
    return parts;
}

std::unique_ptr<roah::URLParts>
roah::URLParser::match(const std::string_view url_path) const
{
    return this->impl_->match(url_path);
}