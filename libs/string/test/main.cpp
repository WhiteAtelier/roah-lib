// This file contains code generated with the assistance of Claude (Anthropic),
// an AI assistant.
// The generated code is provided as-is.

#include "roah/string.hpp"

#include <gtest/gtest.h>

#include <cstring>
#include <string>
#include <string_view>
#include <vector>

// =============================================================================
// toString テスト
// =============================================================================

// u8string_view から string への変換 (新規インスタンス返却版)
TEST(ToStringTest, BasicAsciiConversion)
{
    const auto result = roah::toString(u8"hello");
    EXPECT_EQ(result, "hello");
}

// 空文字列の変換
TEST(ToStringTest, EmptyString)
{
    const auto result = roah::toString(u8"");
    EXPECT_TRUE(result.empty());
}

// UTF-8 マルチバイト文字列の変換 (バイト列が保持されること)
TEST(ToStringTest, Utf8MultiByte)
{
    const auto               result = roah::toString(u8"こんにちは");
    // バイト列が同一であることを確認
    const std::u8string_view src    = u8"こんにちは";
    ASSERT_EQ(result.size(), src.size());
    EXPECT_EQ(std::memcmp(result.data(), src.data(), src.size()), 0);
}

// 混合文字列 (ASCII + マルチバイト)
TEST(ToStringTest, MixedAsciiAndMultiByte)
{
    const auto               result = roah::toString(u8"abc日本語def");
    const std::u8string_view src    = u8"abc日本語def";
    ASSERT_EQ(result.size(), src.size());
    EXPECT_EQ(std::memcmp(result.data(), src.data(), src.size()), 0);
}

// out 引数版: 変換結果が out に格納されること
TEST(ToStringOutTest, StoresResultInOut)
{
    std::string out;
    roah::toString(u8"hello", out);
    EXPECT_EQ(out, "hello");
}

// out 引数版: 戻り値が out と同一インスタンスであること
TEST(ToStringOutTest, ReturnsSameInstanceAsOut)
{
    std::string out;
    auto &      ref = roah::toString(u8"hello", out);
    EXPECT_EQ(&ref, &out);
}

// out 引数版: 空文字列
TEST(ToStringOutTest, EmptyString)
{
    std::string out;
    roah::toString(u8"", out);
    EXPECT_TRUE(out.empty());
}

// out 引数版: UTF-8 マルチバイト文字列
TEST(ToStringOutTest, Utf8MultiByte)
{
    std::string out;
    roah::toString(u8"テスト", out);
    const std::u8string_view src = u8"テスト";
    ASSERT_EQ(out.size(), src.size());
    EXPECT_EQ(std::memcmp(out.data(), src.data(), src.size()), 0);
}

// =============================================================================
// toU8String テスト
// =============================================================================

// string_view から u8string への変換 (新規インスタンス返却版)
TEST(ToU8StringTest, BasicAsciiConversion)
{
    const auto result = roah::toU8String("hello");
    EXPECT_EQ(result, u8"hello");
}

// 空文字列の変換
TEST(ToU8StringTest, EmptyString)
{
    const auto result = roah::toU8String("");
    EXPECT_TRUE(result.empty());
}

// UTF-8 マルチバイト文字列の変換 (バイト列が保持されること)
TEST(ToU8StringTest, Utf8MultiByte)
{
    // UTF-8 エンコードされた string から u8string への変換
    const auto src    = roah::toString(u8"こんにちは");
    const auto result = roah::toU8String(src);
    EXPECT_EQ(result, u8"こんにちは");
}

// 混合文字列 (ASCII + マルチバイト)
TEST(ToU8StringTest, MixedAsciiAndMultiByte)
{
    const auto src    = roah::toString(u8"abc日本語def");
    const auto result = roah::toU8String(src);
    EXPECT_EQ(result, u8"abc日本語def");
}

// out 引数版: 変換結果が out に格納されること
TEST(ToU8StringOutTest, StoresResultInOut)
{
    std::u8string out;
    roah::toU8String("hello", out);
    EXPECT_EQ(out, u8"hello");
}

// out 引数版: 戻り値が out と同一インスタンスであること
TEST(ToU8StringOutTest, ReturnsSameInstanceAsOut)
{
    std::u8string out;
    auto &        ref = roah::toU8String("hello", out);
    EXPECT_EQ(&ref, &out);
}

// out 引数版: 空文字列
TEST(ToU8StringOutTest, EmptyString)
{
    std::u8string out;
    roah::toU8String("", out);
    EXPECT_TRUE(out.empty());
}

// out 引数版: UTF-8 マルチバイト文字列
TEST(ToU8StringOutTest, Utf8MultiByte)
{
    std::u8string out;
    const auto    src = roah::toString(u8"テスト");
    roah::toU8String(src, out);
    EXPECT_EQ(out, u8"テスト");
}

// =============================================================================
// toString / toU8String 往復変換テスト
// =============================================================================

// string -> u8string -> string の往復でバイト列が保持されること
TEST(RoundTripTest, StringToU8StringAndBack)
{
    const std::string original = "hello world";
    const auto        u8ver    = roah::toU8String(original);
    const auto        back     = roah::toString(u8ver);
    EXPECT_EQ(back, original);
}

// u8string -> string -> u8string の往復でバイト列が保持されること
TEST(RoundTripTest, U8StringToStringAndBack)
{
    const std::u8string original = u8"日本語テスト";
    const auto          strver   = roah::toString(original);
    const auto          back     = roah::toU8String(strver);
    EXPECT_EQ(back, original);
}

// =============================================================================
// splitString (コールバック版, string_view) テスト
// =============================================================================

// 基本的な分割: カンマ区切り
TEST(SplitStringCallbackTest, BasicSplit)
{
    std::vector<std::string> parts;
    const auto count = roah::splitString("a,b,c", ',', [&](const std::string_view sv) { parts.emplace_back(sv); });
    EXPECT_EQ(count, 3u);
    ASSERT_EQ(parts.size(), 3u);
    EXPECT_EQ(parts[0], "a");
    EXPECT_EQ(parts[1], "b");
    EXPECT_EQ(parts[2], "c");
}

// 区切り文字が見つからない場合: 元の文字列が1つの部分文字列として返される
TEST(SplitStringCallbackTest, NoDelimiterFound)
{
    std::vector<std::string> parts;
    const auto count = roah::splitString("hello", ',', [&](const std::string_view sv) { parts.emplace_back(sv); });
    EXPECT_EQ(count, 1u);
    ASSERT_EQ(parts.size(), 1u);
    EXPECT_EQ(parts[0], "hello");
}

// 空文字列: 空の部分文字列が1つ生成される
TEST(SplitStringCallbackTest, EmptyString)
{
    std::vector<std::string> parts;
    const auto count = roah::splitString("", ',', [&](const std::string_view sv) { parts.emplace_back(sv); });
    EXPECT_EQ(count, 1u);
    ASSERT_EQ(parts.size(), 1u);
    EXPECT_EQ(parts[0], "");
}

// 先頭に区切り文字がある場合: 空の部分文字列が先頭に生成される
TEST(SplitStringCallbackTest, DelimiterAtStart)
{
    std::vector<std::string> parts;
    const auto count = roah::splitString(",a,b", ',', [&](const std::string_view sv) { parts.emplace_back(sv); });
    EXPECT_EQ(count, 3u);
    ASSERT_EQ(parts.size(), 3u);
    EXPECT_EQ(parts[0], "");
    EXPECT_EQ(parts[1], "a");
    EXPECT_EQ(parts[2], "b");
}

// 末尾に区切り文字がある場合: 空の部分文字列が末尾に生成される
TEST(SplitStringCallbackTest, DelimiterAtEnd)
{
    std::vector<std::string> parts;
    const auto count = roah::splitString("a,b,", ',', [&](const std::string_view sv) { parts.emplace_back(sv); });
    EXPECT_EQ(count, 3u);
    ASSERT_EQ(parts.size(), 3u);
    EXPECT_EQ(parts[0], "a");
    EXPECT_EQ(parts[1], "b");
    EXPECT_EQ(parts[2], "");
}

// 連続する区切り文字: 空の部分文字列が生成される
TEST(SplitStringCallbackTest, ConsecutiveDelimiters)
{
    std::vector<std::string> parts;
    const auto count = roah::splitString("a,,b", ',', [&](const std::string_view sv) { parts.emplace_back(sv); });
    EXPECT_EQ(count, 3u);
    ASSERT_EQ(parts.size(), 3u);
    EXPECT_EQ(parts[0], "a");
    EXPECT_EQ(parts[1], "");
    EXPECT_EQ(parts[2], "b");
}

// 区切り文字のみの文字列
TEST(SplitStringCallbackTest, OnlyDelimiter)
{
    std::vector<std::string> parts;
    const auto count = roah::splitString(",", ',', [&](const std::string_view sv) { parts.emplace_back(sv); });
    EXPECT_EQ(count, 2u);
    ASSERT_EQ(parts.size(), 2u);
    EXPECT_EQ(parts[0], "");
    EXPECT_EQ(parts[1], "");
}

// 戻り値が分割数と一致すること
TEST(SplitStringCallbackTest, ReturnValueMatchesCount)
{
    std::size_t callCount = 0;
    const auto  count     = roah::splitString("one:two:three:four", ':', [&](const std::string_view) { ++callCount; });
    EXPECT_EQ(count, 4u);
    EXPECT_EQ(count, callCount);
}

// =============================================================================
// splitString (コールバック版, u8string_view) テスト
// =============================================================================

// u8string_view 版の基本分割
TEST(SplitStringU8CallbackTest, BasicSplit)
{
    std::vector<std::u8string> parts;
    const auto count = roah::splitString(u8"a,b,c", ',', [&](const std::u8string_view sv) { parts.emplace_back(sv); });
    EXPECT_EQ(count, 3u);
    ASSERT_EQ(parts.size(), 3u);
    EXPECT_EQ(parts[0], u8"a");
    EXPECT_EQ(parts[1], u8"b");
    EXPECT_EQ(parts[2], u8"c");
}

// u8string_view 版: 空文字列: 空の部分文字列が1つ生成される
TEST(SplitStringU8CallbackTest, EmptyString)
{
    std::vector<std::u8string> parts;
    const auto count = roah::splitString(u8"", u8',', [&](const std::u8string_view sv) { parts.emplace_back(sv); });
    EXPECT_EQ(count, 1u);
    ASSERT_EQ(parts.size(), 1u);
    EXPECT_EQ(parts[0], u8"");
}

// u8string_view 版: マルチバイト文字を含む分割
TEST(SplitStringU8CallbackTest, MultiByteSplit)
{
    std::vector<std::u8string> parts;
    const auto                 count
        = roah::splitString(u8"日本,語,テスト", ',', [&](const std::u8string_view sv) { parts.emplace_back(sv); });
    EXPECT_EQ(count, 3u);
    ASSERT_EQ(parts.size(), 3u);
    EXPECT_EQ(parts[0], u8"日本");
    EXPECT_EQ(parts[1], u8"語");
    EXPECT_EQ(parts[2], u8"テスト");
}

// =============================================================================
// splitString (vector 返却版, string_view) テスト
// =============================================================================

// 基本的な分割
TEST(SplitStringVectorTest, BasicSplit)
{
    const auto parts = roah::splitString("a,b,c", ',');
    ASSERT_EQ(parts.size(), 3u);
    EXPECT_EQ(parts[0], "a");
    EXPECT_EQ(parts[1], "b");
    EXPECT_EQ(parts[2], "c");
}

// 区切り文字が見つからない場合
TEST(SplitStringVectorTest, NoDelimiterFound)
{
    const auto parts = roah::splitString("hello", ',');
    ASSERT_EQ(parts.size(), 1u);
    EXPECT_EQ(parts[0], "hello");
}

// 空文字列: 空の部分文字列が1つ生成される
TEST(SplitStringVectorTest, EmptyString)
{
    const auto parts = roah::splitString("", ',');
    ASSERT_EQ(parts.size(), 1u);
    EXPECT_EQ(parts[0], "");
}

// 先頭に区切り文字
TEST(SplitStringVectorTest, DelimiterAtStart)
{
    const auto parts = roah::splitString(",a,b", ',');
    ASSERT_EQ(parts.size(), 3u);
    EXPECT_EQ(parts[0], "");
    EXPECT_EQ(parts[1], "a");
    EXPECT_EQ(parts[2], "b");
}

// 末尾に区切り文字
TEST(SplitStringVectorTest, DelimiterAtEnd)
{
    const auto parts = roah::splitString("a,b,", ',');
    ASSERT_EQ(parts.size(), 3u);
    EXPECT_EQ(parts[0], "a");
    EXPECT_EQ(parts[1], "b");
    EXPECT_EQ(parts[2], "");
}

// 連続する区切り文字
TEST(SplitStringVectorTest, ConsecutiveDelimiters)
{
    const auto parts = roah::splitString("a,,b", ',');
    ASSERT_EQ(parts.size(), 3u);
    EXPECT_EQ(parts[0], "a");
    EXPECT_EQ(parts[1], "");
    EXPECT_EQ(parts[2], "b");
}

// 返却される string_view が元の文字列を参照していること (寿命に関する仕様)
TEST(SplitStringVectorTest, ResultReferencesOriginalString)
{
    const std::string original = "abc,def,ghi";
    const auto        parts    = roah::splitString(std::string_view{ original }, ',');
    ASSERT_EQ(parts.size(), 3u);
    // 部分文字列の data ポインタが元の文字列の範囲内であること
    EXPECT_GE(parts[0].data(), original.data());
    EXPECT_LE(parts[0].data() + parts[0].size(), original.data() + original.size());
    EXPECT_GE(parts[2].data(), original.data());
    EXPECT_LE(parts[2].data() + parts[2].size(), original.data() + original.size());
}

// 異なる区切り文字でも動作すること
TEST(SplitStringVectorTest, DifferentDelimiters)
{
    const auto parts = roah::splitString("path/to/file", '/');
    ASSERT_EQ(parts.size(), 3u);
    EXPECT_EQ(parts[0], "path");
    EXPECT_EQ(parts[1], "to");
    EXPECT_EQ(parts[2], "file");
}

// =============================================================================
// splitString (vector 返却版, u8string_view) テスト
// =============================================================================

// u8string_view 版の基本分割
TEST(SplitStringU8VectorTest, BasicSplit)
{
    const auto parts = roah::splitString(u8"a,b,c", ',');
    ASSERT_EQ(parts.size(), 3u);
    EXPECT_EQ(parts[0], u8"a");
    EXPECT_EQ(parts[1], u8"b");
    EXPECT_EQ(parts[2], u8"c");
}

// u8string_view 版: 空文字列: 空の部分文字列が1つ生成される
TEST(SplitStringU8VectorTest, EmptyString)
{
    const auto parts = roah::splitString(u8"", u8',');
    ASSERT_EQ(parts.size(), 1u);
    EXPECT_EQ(parts[0], u8"");
}

// u8string_view 版: マルチバイト文字を含む分割
TEST(SplitStringU8VectorTest, MultiByteSplit)
{
    const auto parts = roah::splitString(u8"日本,語,テスト", ',');
    ASSERT_EQ(parts.size(), 3u);
    EXPECT_EQ(parts[0], u8"日本");
    EXPECT_EQ(parts[1], u8"語");
    EXPECT_EQ(parts[2], u8"テスト");
}

// u8string_view 版: 返却される u8string_view が元の文字列を参照していること
TEST(SplitStringU8VectorTest, ResultReferencesOriginalString)
{
    const std::u8string original = u8"abc,def,ghi";
    const auto          parts    = roah::splitString(std::u8string_view{ original }, ',');
    ASSERT_EQ(parts.size(), 3u);
    EXPECT_GE(parts[0].data(), original.data());
    EXPECT_LE(parts[0].data() + parts[0].size(), original.data() + original.size());
}

int
main(int argc, char ** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
