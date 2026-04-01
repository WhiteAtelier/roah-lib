// Copyright (C) 2026 White Atelier
// This software is released under the MIT License.
// See the LICENSE file in the project root for more details.

#include "fixed_word.hpp"

roah::url_parser::impl::FixedWord::FixedWord(const std::string_view word) noexcept
    : word_{ word }
{}

roah::url_parser::impl::FixedWord::~FixedWord() noexcept = default;

bool
roah::url_parser::impl::FixedWord::match(WordIter & iter, const WordIter & end) const
{
    if (iter != end && *iter == this->word_)
    {
        ++iter;
        return true;
    }
    return false;
}

void
roah::url_parser::impl::FixedWord::pick(WordIter & iter, const WordIter &, URLPartsImpl *) const
{
    ++iter;
}
