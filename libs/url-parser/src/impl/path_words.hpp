// Copyright (C) 2026 White Atelier
// This software is released under the MIT License.
// See the LICENSE file in the project root for more details.

#ifndef ROAH_URL_PARSER_IMPL_PATH_WORDS_HPP
#define ROAH_URL_PARSER_IMPL_PATH_WORDS_HPP

#include "word.hpp"

#include <string>
#include <string_view>

namespace roah::url_parser::impl {

class PathWords final : public Word
{
public:
    explicit PathWords(const std::string_view key) noexcept;

    ~PathWords() noexcept override;

    bool
    match(WordIter & iter, const WordIter & end) const override;

    void
    pick(WordIter & iter, const WordIter & end, URLPartsImpl * parts) const override;

private:
    std::string key_;
};

}  // namespace roah::url_parser::impl

#endif
