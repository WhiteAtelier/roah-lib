// Copyright (C) 2026 White Atelier
// This software is released under the MIT License.
// See the LICENSE file in the project root for more details.

#ifndef ROAH_URL_PARSER_IMPL_WORD_HPP
#define ROAH_URL_PARSER_IMPL_WORD_HPP

#include <string_view>
#include <vector>

namespace roah::url_parser::impl {

class URLPartsImpl;

class Word
{
protected:
    explicit Word() noexcept;

public:
    using WordIter = std::vector<std::string_view>::const_iterator;

    virtual ~Word() noexcept;

    Word(const Word &)     = delete;
    Word(Word &&) noexcept = delete;
    Word &
    operator=(const Word &)
        = delete;
    Word &
    operator=(Word &&) noexcept
        = delete;

    virtual bool
    match(WordIter & iter, const WordIter & end) const
        = 0;

    virtual void
    pick(WordIter & iter, const WordIter & end, URLPartsImpl * parts) const
        = 0;
};

}  // namespace roah::url_parser::impl

#endif
