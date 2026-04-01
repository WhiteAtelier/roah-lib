// Copyright (C) 2026 White Atelier
// This software is released under the MIT License.
// See the LICENSE file in the project root for more details.

#include "path_words.hpp"

#include "url_parts_impl.hpp"

roah::url_parser::impl::PathWords::PathWords(const std::string_view key) noexcept
    : key_{ key }
{}

roah::url_parser::impl::PathWords::~PathWords() noexcept = default;

bool
roah::url_parser::impl::PathWords::match(WordIter & iter, const WordIter & end) const
{
    iter = end;
    return true;
}

void
roah::url_parser::impl::PathWords::pick(WordIter & iter, const WordIter & end, URLPartsImpl * parts) const
{
    std::string value;
    for (; iter != end; ++iter)
    {
        value += *iter;
        value.push_back('/');
    }
    if (!value.empty())
    {
        // 末尾の '/' を削除する
        value.pop_back();
    }
    parts->setParam(this->key_, value);
}
