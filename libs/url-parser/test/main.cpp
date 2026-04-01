// Copyright (C) 2026 White Atelier
// This software is released under the MIT License.
// See the LICENSE file in the project root for more details.

#include "roah/url_parse_error.hpp"
#include "roah/url_parser.hpp"
#include "roah/url_parts.hpp"

#include <gtest/gtest.h>

// ============================================================
// Fixed-path matching
// ============================================================

TEST(URLParserFixed, ExactMatch)
{
    roah::URLParser parser("/user/profile");
    auto            parts = parser.match("/user/profile");
    EXPECT_NE(parts, nullptr);
}

TEST(URLParserFixed, NoMatchWrongSegment)
{
    roah::URLParser parser("/user/profile");
    auto            parts = parser.match("/user/other");
    EXPECT_EQ(parts, nullptr);
}

TEST(URLParserFixed, NoMatchTooShort)
{
    roah::URLParser parser("/user/profile");
    auto            parts = parser.match("/user");
    EXPECT_EQ(parts, nullptr);
}

TEST(URLParserFixed, NoMatchTooLong)
{
    roah::URLParser parser("/user");
    auto            parts = parser.match("/user/profile");
    EXPECT_EQ(parts, nullptr);
}

TEST(URLParserFixed, TrailingSlash)
{
    // 末尾の "/" は無視されてマッチするべき
    roah::URLParser parser("/user/profile");
    auto            parts = parser.match("/user/profile/");
    EXPECT_NE(parts, nullptr);
}

TEST(URLParserFixed, SingleSegment)
{
    roah::URLParser parser("/user");
    auto            parts = parser.match("/user");
    EXPECT_NE(parts, nullptr);
}

TEST(URLParserFixed, NullOnWrongRoot)
{
    roah::URLParser parser("/api/users");
    auto            parts = parser.match("/web/users");
    EXPECT_EQ(parts, nullptr);
}

// ============================================================
// :key パラメータマッチング
// ============================================================

TEST(URLParserParam, SingleParamNumeric)
{
    roah::URLParser parser("/user/:id");
    auto            parts = parser.match("/user/123");
    ASSERT_NE(parts, nullptr);
    EXPECT_EQ(parts->getParam("id"), "123");
}

TEST(URLParserParam, SingleParamString)
{
    roah::URLParser parser("/user/:name");
    auto            parts = parser.match("/user/alice");
    ASSERT_NE(parts, nullptr);
    EXPECT_EQ(parts->getParam("name"), "alice");
}

TEST(URLParserParam, TwoParams)
{
    roah::URLParser parser("/api/:version/:resource");
    auto            parts = parser.match("/api/v1/items");
    ASSERT_NE(parts, nullptr);
    EXPECT_EQ(parts->getParam("version"), "v1");
    EXPECT_EQ(parts->getParam("resource"), "items");
}

TEST(URLParserParam, ParamWithLeadingFixed)
{
    roah::URLParser parser("/user-folder/:user_id/:file_id");
    auto            parts = parser.match("/user-folder/42/readme");
    ASSERT_NE(parts, nullptr);
    EXPECT_EQ(parts->getParam("user_id"), "42");
    EXPECT_EQ(parts->getParam("file_id"), "readme");
}

TEST(URLParserParam, NoMatchMissingSegment)
{
    roah::URLParser parser("/user/:id");
    auto            parts = parser.match("/user");
    EXPECT_EQ(parts, nullptr);
}

TEST(URLParserParam, NoMatchExtraSegment)
{
    // ワイルドカードなしでは余分なセグメントはマッチしない
    roah::URLParser parser("/user/:id");
    auto            parts = parser.match("/user/123/extra");
    EXPECT_EQ(parts, nullptr);
}

// ============================================================
// *path ワイルドカードマッチング
// ============================================================

TEST(URLParserWildcard, SingleSegment)
{
    roah::URLParser parser("/files/*path");
    auto            parts = parser.match("/files/readme.txt");
    ASSERT_NE(parts, nullptr);
    EXPECT_EQ(parts->getParam("path"), "readme.txt");
}

TEST(URLParserWildcard, MultipleSegments)
{
    roah::URLParser parser("/files/*path");
    auto            parts = parser.match("/files/a/b/c");
    ASSERT_NE(parts, nullptr);
    EXPECT_EQ(parts->getParam("path"), "a/b/c");
}

TEST(URLParserWildcard, WithParamAndWildcard)
{
    // SolidJS Router 風: /user-folder/:user_id/*file_path
    roah::URLParser parser("/user-folder/:user_id/*file_path");
    auto            parts = parser.match("/user-folder/42/docs/readme.md");
    ASSERT_NE(parts, nullptr);
    EXPECT_EQ(parts->getParam("user_id"), "42");
    EXPECT_EQ(parts->getParam("file_path"), "docs/readme.md");
}

TEST(URLParserWildcard, MatchesEvenWithQuery)
{
    roah::URLParser parser("/files/*path");
    auto            parts = parser.match("/files/a/b?download=1");
    ASSERT_NE(parts, nullptr);
    EXPECT_EQ(parts->getParam("path"), "a/b");
    ASSERT_EQ(parts->getQuery("download").size(), 1u);
    EXPECT_EQ(parts->getQuery("download")[0], "1");
}

// ============================================================
// *path の後ろにセグメントがあるパターン (常に不一致)
// ============================================================

// PathWords は iter を end まで進めるため、後続の FixedWord / KeyWord は
// 必ずセグメント不足でマッチ失敗する。どんな URL を渡しても nullptr になる。

TEST(URLParserWildcardFollowedByFixed, ShortUrl)
{
    roah::URLParser parser("/hoge/*path/foobar");
    EXPECT_EQ(parser.match("/hoge/a"), nullptr);
}

TEST(URLParserWildcardFollowedByFixed, ExactPatternSegments)
{
    // セグメント数が一致しても不一致になる
    roah::URLParser parser("/hoge/*path/foobar");
    EXPECT_EQ(parser.match("/hoge/something/foobar"), nullptr);
}

TEST(URLParserWildcardFollowedByFixed, LongUrl)
{
    roah::URLParser parser("/hoge/*path/foobar");
    EXPECT_EQ(parser.match("/hoge/a/b/c/foobar"), nullptr);
}

TEST(URLParserWildcardFollowedByFixed, WithQuery)
{
    roah::URLParser parser("/hoge/*path/foobar");
    EXPECT_EQ(parser.match("/hoge/a/b/foobar?q=1"), nullptr);
}

TEST(URLParserWildcardFollowedByFixed, EmptyWildcardPart)
{
    // *path 部分が空になるようなセグメント数でも不一致
    roah::URLParser parser("/hoge/*path/foobar");
    EXPECT_EQ(parser.match("/hoge/foobar"), nullptr);
}

TEST(URLParserWildcardFollowedByParam, AnyUrl)
{
    // 後続が :key でも同様に常に不一致
    roah::URLParser parser("/files/*path/:id");
    EXPECT_EQ(parser.match("/files/a/b/123"), nullptr);
}

TEST(URLParserWildcardFollowedByParam, ShortUrl)
{
    roah::URLParser parser("/files/*path/:id");
    EXPECT_EQ(parser.match("/files/123"), nullptr);
}

// ============================================================
// getParam 型変換
// ============================================================

TEST(URLParserParamConv, GetParamAsInt)
{
    roah::URLParser parser("/item/:id");
    auto            parts = parser.match("/item/42");
    ASSERT_NE(parts, nullptr);
    EXPECT_EQ(parts->getParamAsInt("id"), 42);
}

TEST(URLParserParamConv, GetParamAsIntNegative)
{
    roah::URLParser parser("/delta/:val");
    auto            parts = parser.match("/delta/-7");
    ASSERT_NE(parts, nullptr);
    EXPECT_EQ(parts->getParamAsInt("val"), -7);
}

TEST(URLParserParamConv, GetParamAsIntInvalidThrows)
{
    roah::URLParser parser("/item/:id");
    auto            parts = parser.match("/item/abc");
    ASSERT_NE(parts, nullptr);
    EXPECT_THROW(parts->getParamAsInt("id"), roah::URLParseError);
}

TEST(URLParserParamConv, GetParamAsUInt)
{
    roah::URLParser parser("/item/:id");
    auto            parts = parser.match("/item/100");
    ASSERT_NE(parts, nullptr);
    EXPECT_EQ(parts->getParamAsUInt("id"), 100u);
}

TEST(URLParserParamConv, GetParamAsUIntInvalidThrows)
{
    roah::URLParser parser("/item/:id");
    auto            parts = parser.match("/item/notanumber");
    ASSERT_NE(parts, nullptr);
    EXPECT_THROW(parts->getParamAsUInt("id"), roah::URLParseError);
}

TEST(URLParserParamConv, GetParamNotFoundThrows)
{
    roah::URLParser parser("/user/:id");
    auto            parts = parser.match("/user/123");
    ASSERT_NE(parts, nullptr);
    EXPECT_THROW(parts->getParam("nonexistent"), roah::URLParseError);
}

// ============================================================
// クエリ文字列パース
// ============================================================

TEST(URLParserQuery, SingleValue)
{
    roah::URLParser parser("/search");
    auto            parts = parser.match("/search?q=hello");
    ASSERT_NE(parts, nullptr);
    const auto & q = parts->getQuery("q");
    ASSERT_EQ(q.size(), 1u);
    EXPECT_EQ(q[0], "hello");
}

TEST(URLParserQuery, MultiValueComma)
{
    roah::URLParser parser("/search");
    auto            parts = parser.match("/search?tags=a,b,c");
    ASSERT_NE(parts, nullptr);
    const auto & tags = parts->getQuery("tags");
    ASSERT_EQ(tags.size(), 3u);
    EXPECT_EQ(tags[0], "a");
    EXPECT_EQ(tags[1], "b");
    EXPECT_EQ(tags[2], "c");
}

TEST(URLParserQuery, MultipleKeys)
{
    // /hogehoge/foobar/nanana?aa=bb,cc&xx=yy,zz のようなケース
    roah::URLParser parser("/api/data");
    auto            parts = parser.match("/api/data?aa=bb,cc&xx=yy,zz");
    ASSERT_NE(parts, nullptr);
    const auto & aa = parts->getQuery("aa");
    ASSERT_EQ(aa.size(), 2u);
    EXPECT_EQ(aa[0], "bb");
    EXPECT_EQ(aa[1], "cc");
    const auto & xx = parts->getQuery("xx");
    ASSERT_EQ(xx.size(), 2u);
    EXPECT_EQ(xx[0], "yy");
    EXPECT_EQ(xx[1], "zz");
}

TEST(URLParserQuery, KeyOnlyNoValue)
{
    // "=" のないクエリキーは値が空文字列として登録される
    roah::URLParser parser("/page");
    auto            parts = parser.match("/page?flag");
    ASSERT_NE(parts, nullptr);
    const auto & v = parts->getQuery("flag");
    ASSERT_EQ(v.size(), 1u);
    EXPECT_EQ(v[0], "");
}

TEST(URLParserQuery, NoQueryReturnsEmptyVector)
{
    roah::URLParser parser("/page");
    auto            parts = parser.match("/page");
    ASSERT_NE(parts, nullptr);
    EXPECT_TRUE(parts->getQuery("anything").empty());
}

TEST(URLParserQuery, NotFoundReturnsEmptyVector)
{
    roah::URLParser parser("/search");
    auto            parts = parser.match("/search?q=hello");
    ASSERT_NE(parts, nullptr);
    EXPECT_TRUE(parts->getQuery("nonexistent").empty());
}

TEST(URLParserQuery, WithParamAndQuery)
{
    roah::URLParser parser("/user/:id");
    auto            parts = parser.match("/user/42?format=json");
    ASSERT_NE(parts, nullptr);
    EXPECT_EQ(parts->getParam("id"), "42");
    const auto & fmt = parts->getQuery("format");
    ASSERT_EQ(fmt.size(), 1u);
    EXPECT_EQ(fmt[0], "json");
}

TEST(URLParserQuery, DuplicateKeyAmpersand)
{
    // ?a=hoge&a=foobar のように同じキーを & で複数回指定した場合、両方取れる
    roah::URLParser parser("/page");
    auto            parts = parser.match("/page?a=hoge&a=foobar");
    ASSERT_NE(parts, nullptr);
    const auto & a = parts->getQuery("a");
    ASSERT_EQ(a.size(), 2u);
    EXPECT_EQ(a[0], "hoge");
    EXPECT_EQ(a[1], "foobar");
}

TEST(URLParserQuery, DuplicateKeyMixedCommaAndAmpersand)
{
    // ?a=x,y&a=z のようにコンマ複数値と & 重複を組み合わせた場合
    roah::URLParser parser("/page");
    auto            parts = parser.match("/page?a=x,y&a=z");
    ASSERT_NE(parts, nullptr);
    const auto & a = parts->getQuery("a");
    ASSERT_EQ(a.size(), 3u);
    EXPECT_EQ(a[0], "x");
    EXPECT_EQ(a[1], "y");
    EXPECT_EQ(a[2], "z");
}

// ============================================================
// HTTP 仕様上の考慮点
// ============================================================

// ルートパス "/" のマッチング
// 末尾スラッシュ除去ロジックが words.size() > 1 の条件付きで動作することで正しくマッチする
TEST(URLParserSpec, RootPathMatch)
{
    roah::URLParser parser("/");
    auto            parts = parser.match("/");
    EXPECT_NE(parts, nullptr);
}

TEST(URLParserSpec, RootPathNoMatchNonRoot)
{
    roah::URLParser parser("/");
    auto            parts = parser.match("/user");
    EXPECT_EQ(parts, nullptr);
}

// ?key= のように明示的に空値を指定した場合、空文字列として1件登録される
TEST(URLParserSpec, QueryEmptyValueRegistersEmpty)
{
    roah::URLParser parser("/page");
    auto            parts = parser.match("/page?key=");
    ASSERT_NE(parts, nullptr);
    const auto & v = parts->getQuery("key");
    ASSERT_EQ(v.size(), 1u);
    EXPECT_EQ(v[0], "");
}

// ?key= と ?key (値なし) は異なる: 前者は [""], 後者も [""] だが意味が異なる
TEST(URLParserSpec, QueryEmptyValueVsNoValue)
{
    roah::URLParser parser("/page");
    auto with_eq    = parser.match("/page?key=");
    auto without_eq = parser.match("/page?key");
    ASSERT_NE(with_eq, nullptr);
    ASSERT_NE(without_eq, nullptr);
    // どちらも size==1, value=="" の要素を持つ
    EXPECT_EQ(with_eq->getQuery("key").size(), 1u);
    EXPECT_EQ(without_eq->getQuery("key").size(), 1u);
}

// パーセントエンコーディング (%XX) はデコードされずリテラル文字列として扱われる
// ルーティング用途ではフレームワーク側でデコードするため、この仕様は許容範囲内
TEST(URLParserSpec, PercentEncodingTreatedAsLiteral)
{
    roah::URLParser parser("/user/:name");
    auto            parts = parser.match("/user/hello%20world");
    ASSERT_NE(parts, nullptr);
    EXPECT_EQ(parts->getParam("name"), "hello%20world");
}

TEST(URLParserSpec, PercentEncodingInQuery)
{
    roah::URLParser parser("/search");
    auto            parts = parser.match("/search?q=hello%20world");
    ASSERT_NE(parts, nullptr);
    const auto & q = parts->getQuery("q");
    ASSERT_EQ(q.size(), 1u);
    EXPECT_EQ(q[0], "hello%20world");
}

// クエリ値に "=" が含まれる場合、最初の "=" のみ区切りとして機能し、残りは値に含まれる
TEST(URLParserSpec, QueryValueContainingEquals)
{
    roah::URLParser parser("/page");
    auto            parts = parser.match("/page?expr=a=b");
    ASSERT_NE(parts, nullptr);
    const auto & v = parts->getQuery("expr");
    ASSERT_EQ(v.size(), 1u);
    EXPECT_EQ(v[0], "a=b");
}

// 連続スラッシュ "//" は空セグメントを生成し、パターンにマッチしない
TEST(URLParserSpec, ConsecutiveSlashesNoMatch)
{
    roah::URLParser parser("/user/:id");
    EXPECT_EQ(parser.match("/user//123"), nullptr);
}

TEST(URLParserSpec, ConsecutiveSlashesAtRoot)
{
    roah::URLParser parser("/user/:id");
    EXPECT_EQ(parser.match("//user/123"), nullptr);
}

// ============================================================

int
main(int argc, char ** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
