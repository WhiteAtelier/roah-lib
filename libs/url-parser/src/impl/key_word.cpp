// Copyright (C) 2026 White Atelier
// This software is released under the MIT License.
// See the LICENSE file in the project root for more details.

#include "key_word.hpp"

#include "url_parts_impl.hpp"

roah::url_parser::impl::KeyWord::KeyWord(const std::string_view key) noexcept
    : key_{ key }
{}

roah::url_parser::impl::KeyWord::~KeyWord() noexcept = default;

bool
roah::url_parser::impl::KeyWord::match(WordIter & iter, const WordIter & end) const
{
    if (iter != end)
    {
        ++iter;
        return true;
    }
    return false;
}

void
roah::url_parser::impl::KeyWord::pick(WordIter & iter, const WordIter &, URLPartsImpl * parts) const
{
    parts->setParam(this->key_, *iter);
    ++iter;
}
