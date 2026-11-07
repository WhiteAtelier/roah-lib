// This file contains code generated with the assistance of Codex (OpenAI), an AI assistant.
// The generated code is provided as-is.

#include <gtest/gtest.h>
#include <roah/config_loader.hpp>
#include <roah/config_loader_error.hpp>

#include <filesystem>
#include <string_view>
#include <utility>

namespace {
const std::filesystem::path kPathResource{ std::filesystem::path{ ROAH_TEST_RESOURCE_DIR } / "test03_paths.toml" };
}  // namespace

TEST(ConfigLoaderPath, StringPathCanBeSplitAtEveryBoundary)
{
    roah::ConfigLoader loader{};
    loader.load(kPathResource);

    const std::pair<std::string_view, std::string_view> paths[]{
        { "", "app.transport.http.host" }, { "app", "transport.http.host" },  { "app.transport", "http.host" },
        { "app.transport.http", "host" },  { "app.transport.http.host", "" },
    };
    for (const auto & [category, key] : paths)
    {
        SCOPED_TRACE(::testing::Message{} << "category=" << category << ", key=" << key);
        EXPECT_EQ(loader.getString(category, key), "nested.example");
        EXPECT_EQ(loader.getString(category, key, "fallback"), "nested.example");
    }
}

TEST(ConfigLoaderPath, IntSupportsDottedPathsAndEmptyArguments)
{
    roah::ConfigLoader loader{};
    loader.load(kPathResource);

    EXPECT_EQ(loader.getInt("app.transport", "http.port"), 9443);
    EXPECT_EQ(loader.getInt("app.transport", "http.port", -1), 9443);
    EXPECT_EQ(loader.getInt("", "app.transport.http.port"), 9443);
    EXPECT_EQ(loader.getInt("", "app.transport.http.port", -1), 9443);
    EXPECT_EQ(loader.getInt("app.transport.http.port", ""), 9443);
    EXPECT_EQ(loader.getInt("app.transport.http.port", "", -1), 9443);
}

TEST(ConfigLoaderPath, BoolSupportsDottedPathsAndEmptyArguments)
{
    roah::ConfigLoader loader{};
    loader.load(kPathResource);

    EXPECT_TRUE(loader.getBool("app.transport", "http.enabled"));
    EXPECT_TRUE(loader.getBool("app.transport", "http.enabled", false));
    EXPECT_TRUE(loader.getBool("", "app.transport.http.enabled"));
    EXPECT_TRUE(loader.getBool("", "app.transport.http.enabled", false));
    EXPECT_TRUE(loader.getBool("app.transport.http.enabled", ""));
    EXPECT_TRUE(loader.getBool("app.transport.http.enabled", "", false));
}

TEST(ConfigLoaderPath, DoubleSupportsDottedPathsAndEmptyArguments)
{
    roah::ConfigLoader loader{};
    loader.load(kPathResource);

    EXPECT_DOUBLE_EQ(loader.getDouble("app.transport", "http.rate"), 0.25);
    EXPECT_DOUBLE_EQ(loader.getDouble("app.transport", "http.rate", -1.0), 0.25);
    EXPECT_DOUBLE_EQ(loader.getDouble("", "app.transport.http.rate"), 0.25);
    EXPECT_DOUBLE_EQ(loader.getDouble("", "app.transport.http.rate", -1.0), 0.25);
    EXPECT_DOUBLE_EQ(loader.getDouble("app.transport.http.rate", ""), 0.25);
    EXPECT_DOUBLE_EQ(loader.getDouble("app.transport.http.rate", "", -1.0), 0.25);
}

TEST(ConfigLoaderPath, EmptyArgumentAllowsRootValue)
{
    roah::ConfigLoader loader{};
    loader.load(kPathResource);

    EXPECT_EQ(loader.getString("", "title"), "root");
    EXPECT_EQ(loader.getString("", "title", "fallback"), "root");
    EXPECT_EQ(loader.getString("title", ""), "root");
    EXPECT_EQ(loader.getString("title", "", "fallback"), "root");
}

TEST(ConfigLoaderPath, MissingSegmentUsesDefaultOrRequiredKeyError)
{
    roah::ConfigLoader loader{};
    loader.load(kPathResource);

    const std::pair<std::string_view, std::string_view> paths[]{
        { "missing.transport", "http.host" }, { "app.missing", "http.host" },
        { "app.transport", "missing.host" },  { "app.transport", "http.missing" },
        { "", "app.transport.http.missing" }, { "app.transport.http.missing", "" },
    };
    for (const auto & [category, key] : paths)
    {
        SCOPED_TRACE(::testing::Message{} << "category=" << category << ", key=" << key);
        EXPECT_EQ(loader.getString(category, key, "fallback"), "fallback");
        EXPECT_EQ(loader.getInt(category, key, -1), -1);
        EXPECT_TRUE(loader.getBool(category, key, true));
        EXPECT_DOUBLE_EQ(loader.getDouble(category, key, -1.0), -1.0);
        EXPECT_THROW(static_cast<void>(loader.getString(category, key)), roah::ConfigRequiredKeyNotFoundError);
        EXPECT_THROW(static_cast<void>(loader.getInt(category, key)), roah::ConfigRequiredKeyNotFoundError);
        EXPECT_THROW(static_cast<void>(loader.getBool(category, key)), roah::ConfigRequiredKeyNotFoundError);
        EXPECT_THROW(static_cast<void>(loader.getDouble(category, key)), roah::ConfigRequiredKeyNotFoundError);
    }
}

TEST(ConfigLoaderPath, WrongLeafTypeThrowsEvenWithDefault)
{
    roah::ConfigLoader loader{};
    loader.load(kPathResource);

    EXPECT_THROW(static_cast<void>(loader.getString("app.transport", "http.port")), roah::ConfigTypeError);
    EXPECT_THROW(static_cast<void>(loader.getString("app.transport", "http.port", "fallback")), roah::ConfigTypeError);
    EXPECT_THROW(static_cast<void>(loader.getInt("app.transport", "http.host")), roah::ConfigTypeError);
    EXPECT_THROW(static_cast<void>(loader.getInt("app.transport", "http.host", -1)), roah::ConfigTypeError);
    EXPECT_THROW(static_cast<void>(loader.getBool("app.transport", "http.host")), roah::ConfigTypeError);
    EXPECT_THROW(static_cast<void>(loader.getBool("app.transport", "http.host", false)), roah::ConfigTypeError);
    EXPECT_THROW(static_cast<void>(loader.getDouble("app.transport", "http.host")), roah::ConfigTypeError);
    EXPECT_THROW(static_cast<void>(loader.getDouble("app.transport", "http.host", -1.0)), roah::ConfigTypeError);
}

TEST(ConfigLoaderPath, TraversalBeyondScalarOrArrayIsMissing)
{
    roah::ConfigLoader loader{};
    loader.load(kPathResource);

    const std::pair<std::string_view, std::string_view> paths[]{
        { "", "title.child.leaf" },
        { "title.child", "leaf" },
        { "app.transport.http.host", "child.leaf" },
        { "app.transport.http.ports", "child.leaf" },
    };
    for (const auto & [category, key] : paths)
    {
        SCOPED_TRACE(::testing::Message{} << "category=" << category << ", key=" << key);
        EXPECT_EQ(loader.getString(category, key, "fallback"), "fallback");
        EXPECT_THROW(static_cast<void>(loader.getString(category, key)), roah::ConfigRequiredKeyNotFoundError);
    }
}

TEST(ConfigLoaderPath, TableAndEmptyPathAreNotScalarValues)
{
    roah::ConfigLoader loader{};
    loader.load(kPathResource);

    const std::pair<std::string_view, std::string_view> paths[]{
        { "app", "transport.http" },
        { "", "" },
    };
    for (const auto & [category, key] : paths)
    {
        SCOPED_TRACE(::testing::Message{} << "category=" << category << ", key=" << key);
        EXPECT_THROW(static_cast<void>(loader.getString(category, key)), roah::ConfigTypeError);
        EXPECT_THROW(static_cast<void>(loader.getString(category, key, "fallback")), roah::ConfigTypeError);
        EXPECT_THROW(static_cast<void>(loader.getInt(category, key)), roah::ConfigTypeError);
        EXPECT_THROW(static_cast<void>(loader.getInt(category, key, -1)), roah::ConfigTypeError);
        EXPECT_THROW(static_cast<void>(loader.getBool(category, key)), roah::ConfigTypeError);
        EXPECT_THROW(static_cast<void>(loader.getBool(category, key, false)), roah::ConfigTypeError);
        EXPECT_THROW(static_cast<void>(loader.getDouble(category, key)), roah::ConfigTypeError);
        EXPECT_THROW(static_cast<void>(loader.getDouble(category, key, -1.0)), roah::ConfigTypeError);
    }
}

TEST(ConfigLoaderPath, WhitespaceInNamesIsPreserved)
{
    roah::ConfigLoader loader{};
    loader.load(kPathResource);

    EXPECT_EQ(loader.getString("app.transport.http", "white space"), "preserved");
    EXPECT_EQ(loader.getString("app.transport.http", " "), "space key");
    EXPECT_EQ(loader.getString("app.transport.http", "\t"), "tab key");
    EXPECT_EQ(loader.getString(" ", "value"), "space category");
    EXPECT_EQ(loader.getString("\t", "value"), "tab category");
}

TEST(ConfigLoaderPath, StringViewLengthIsRespected)
{
    roah::ConfigLoader loader{};
    loader.load(kPathResource);

    const std::string_view category{ "app.transport.unused", 13 };
    const std::string_view key{ "http.host.unused", 9 };
    EXPECT_EQ(loader.getString(category, key), "nested.example");
    EXPECT_EQ(loader.getString(category, key, "fallback"), "nested.example");
}
