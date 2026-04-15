// This file contains code generated with the assistance of Claude (Anthropic),
// an AI assistant. The generated code is provided as-is.

#include <gtest/gtest.h>
#include <roah/config_loader.hpp>
#include <roah/config_loader_error.hpp>

#include <filesystem>
#include <string>

// [[nodiscard]] な戻り値を意図的に破棄することを示すマクロ.
#define DISCARD(expr) static_cast<void>(expr)

namespace {
const std::filesystem::path kResourceDir = ROAH_TEST_RESOURCE_DIR;
}  // namespace

// ================================================================================================
// Construction / operator bool
// ================================================================================================

TEST(ConfigLoader, DefaultConstructedIsEmpty)
{
    roah::ConfigLoader loader;
    EXPECT_FALSE(static_cast<bool>(loader));
    EXPECT_TRUE(!loader);
}

// ================================================================================================
// load
// ================================================================================================

TEST(ConfigLoader, LoadValidFile)
{
    roah::ConfigLoader loader;
    ASSERT_NO_THROW(loader.load(kResourceDir / "test01.toml"));
    EXPECT_TRUE(static_cast<bool>(loader));
    EXPECT_FALSE(!loader);
}

TEST(ConfigLoader, LoadNonExistentFileThrows)
{
    roah::ConfigLoader loader;
    EXPECT_THROW(loader.load(kResourceDir / "nonexistent.toml"), roah::ConfigFileIOError);
}

TEST(ConfigLoader, LoadSyntaxErrorFileThrows)
{
    roah::ConfigLoader loader;
    EXPECT_THROW(loader.load(kResourceDir / "test02_syntax_error.toml"), roah::ConfigSyntaxError);
}

// ================================================================================================
// reset
// ================================================================================================

TEST(ConfigLoader, ResetClearsData)
{
    roah::ConfigLoader loader;
    loader.load(kResourceDir / "test01.toml");
    ASSERT_TRUE(static_cast<bool>(loader));

    loader.reset();
    EXPECT_FALSE(static_cast<bool>(loader));
}

TEST(ConfigLoader, ResetOnEmptyIsNoOp)
{
    roah::ConfigLoader loader;
    EXPECT_NO_THROW(loader.reset());
    EXPECT_FALSE(static_cast<bool>(loader));
}

// ================================================================================================
// Move semantics
// ================================================================================================

TEST(ConfigLoader, MoveConstruct)
{
    roah::ConfigLoader loader;
    loader.load(kResourceDir / "test01.toml");

    roah::ConfigLoader moved(std::move(loader));
    EXPECT_TRUE(static_cast<bool>(moved));
}

TEST(ConfigLoader, MoveAssign)
{
    roah::ConfigLoader loader;
    loader.load(kResourceDir / "test01.toml");

    roah::ConfigLoader other;
    other = std::move(loader);
    EXPECT_TRUE(static_cast<bool>(other));
}

// ================================================================================================
// getString
// ================================================================================================

class ConfigLoaderGetTest : public ::testing::Test
{
protected:
    void
    SetUp() override
    {
        this->loader_.load(kResourceDir / "test01.toml");
    }

    roah::ConfigLoader loader_;
};

TEST_F(ConfigLoaderGetTest, GetStringExistingKey)
{
    EXPECT_EQ(this->loader_.getString("server", "host", "default"), "localhost");
}

TEST_F(ConfigLoaderGetTest, GetStringMissingKeyReturnsDefault)
{
    EXPECT_EQ(this->loader_.getString("server", "missing_key", "fallback"), "fallback");
}

TEST_F(ConfigLoaderGetTest, GetStringMissingCategoryReturnsDefault)
{
    EXPECT_EQ(this->loader_.getString("nonexistent", "key", "fallback"), "fallback");
}

TEST_F(ConfigLoaderGetTest, GetStringTypeMismatchThrows)
{
    // port は整数なので, getString すると ConfigTypeError
    EXPECT_THROW(DISCARD(this->loader_.getString("server", "port", "default")), roah::ConfigTypeError);
}

TEST_F(ConfigLoaderGetTest, GetStringRequiredExistingKey)
{
    EXPECT_EQ(this->loader_.getString("server", "host"), "localhost");
}

TEST_F(ConfigLoaderGetTest, GetStringRequiredMissingKeyThrows)
{
    EXPECT_THROW(DISCARD(this->loader_.getString("server", "missing_key")), roah::ConfigRequiredKeyNotFoundError);
}

TEST_F(ConfigLoaderGetTest, GetStringRequiredTypeMismatchThrows)
{
    EXPECT_THROW(DISCARD(this->loader_.getString("server", "port")), roah::ConfigTypeError);
}

// ================================================================================================
// getInt
// ================================================================================================

TEST_F(ConfigLoaderGetTest, GetIntExistingKey)
{
    EXPECT_EQ(this->loader_.getInt("server", "port", 0), 8080);
}

TEST_F(ConfigLoaderGetTest, GetIntMissingKeyReturnsDefault)
{
    EXPECT_EQ(this->loader_.getInt("server", "missing_key", 9999), 9999);
}

TEST_F(ConfigLoaderGetTest, GetIntMissingCategoryReturnsDefault)
{
    EXPECT_EQ(this->loader_.getInt("nonexistent", "key", 42), 42);
}

TEST_F(ConfigLoaderGetTest, GetIntTypeMismatchThrows)
{
    // host は文字列なので, getInt すると ConfigTypeError
    EXPECT_THROW(DISCARD(this->loader_.getInt("server", "host", 0)), roah::ConfigTypeError);
}

TEST_F(ConfigLoaderGetTest, GetIntRequiredExistingKey)
{
    EXPECT_EQ(this->loader_.getInt("server", "port"), 8080);
}

TEST_F(ConfigLoaderGetTest, GetIntRequiredMissingKeyThrows)
{
    EXPECT_THROW(DISCARD(this->loader_.getInt("server", "missing_key")), roah::ConfigRequiredKeyNotFoundError);
}

TEST_F(ConfigLoaderGetTest, GetIntRequiredTypeMismatchThrows)
{
    EXPECT_THROW(DISCARD(this->loader_.getInt("server", "host")), roah::ConfigTypeError);
}

// ================================================================================================
// getBool
// ================================================================================================

TEST_F(ConfigLoaderGetTest, GetBoolExistingKeyTrue)
{
    EXPECT_EQ(this->loader_.getBool("server", "enabled", false), true);
}

TEST_F(ConfigLoaderGetTest, GetBoolExistingKeyFalse)
{
    EXPECT_EQ(this->loader_.getBool("database", "debug", true), false);
}

TEST_F(ConfigLoaderGetTest, GetBoolMissingKeyReturnsDefault)
{
    EXPECT_EQ(this->loader_.getBool("server", "missing_key", true), true);
}

TEST_F(ConfigLoaderGetTest, GetBoolMissingCategoryReturnsDefault)
{
    EXPECT_EQ(this->loader_.getBool("nonexistent", "key", false), false);
}

TEST_F(ConfigLoaderGetTest, GetBoolTypeMismatchThrows)
{
    // host は文字列なので, getBool すると ConfigTypeError
    EXPECT_THROW(DISCARD(this->loader_.getBool("server", "host", false)), roah::ConfigTypeError);
}

TEST_F(ConfigLoaderGetTest, GetBoolRequiredExistingKey)
{
    EXPECT_EQ(this->loader_.getBool("server", "enabled"), true);
}

TEST_F(ConfigLoaderGetTest, GetBoolRequiredMissingKeyThrows)
{
    EXPECT_THROW(DISCARD(this->loader_.getBool("server", "missing_key")), roah::ConfigRequiredKeyNotFoundError);
}

TEST_F(ConfigLoaderGetTest, GetBoolRequiredTypeMismatchThrows)
{
    EXPECT_THROW(DISCARD(this->loader_.getBool("server", "host")), roah::ConfigTypeError);
}

// ================================================================================================
// getDouble
// ================================================================================================

TEST_F(ConfigLoaderGetTest, GetDoubleExistingKey)
{
    EXPECT_DOUBLE_EQ(this->loader_.getDouble("server", "rate", 0.0), 3.14);
}

TEST_F(ConfigLoaderGetTest, GetDoubleMissingKeyReturnsDefault)
{
    EXPECT_DOUBLE_EQ(this->loader_.getDouble("server", "missing_key", 1.23), 1.23);
}

TEST_F(ConfigLoaderGetTest, GetDoubleMissingCategoryReturnsDefault)
{
    EXPECT_DOUBLE_EQ(this->loader_.getDouble("nonexistent", "key", 9.99), 9.99);
}

TEST_F(ConfigLoaderGetTest, GetDoubleTypeMismatchThrows)
{
    // host は文字列なので, getDouble すると ConfigTypeError
    EXPECT_THROW(DISCARD(this->loader_.getDouble("server", "host", 0.0)), roah::ConfigTypeError);
}

TEST_F(ConfigLoaderGetTest, GetDoubleRequiredExistingKey)
{
    EXPECT_DOUBLE_EQ(this->loader_.getDouble("server", "rate"), 3.14);
}

TEST_F(ConfigLoaderGetTest, GetDoubleRequiredMissingKeyThrows)
{
    EXPECT_THROW(DISCARD(this->loader_.getDouble("server", "missing_key")), roah::ConfigRequiredKeyNotFoundError);
}

TEST_F(ConfigLoaderGetTest, GetDoubleRequiredTypeMismatchThrows)
{
    EXPECT_THROW(DISCARD(this->loader_.getDouble("server", "host")), roah::ConfigTypeError);
}

// ================================================================================================
// Multiple categories
// ================================================================================================

TEST_F(ConfigLoaderGetTest, GetValuesFromDifferentCategories)
{
    EXPECT_EQ(this->loader_.getString("database", "name", ""), "mydb");
    EXPECT_EQ(this->loader_.getInt("database", "max_connections", 0), 100);
    EXPECT_EQ(this->loader_.getBool("database", "debug", true), false);
    EXPECT_DOUBLE_EQ(this->loader_.getDouble("database", "timeout", 0.0), 2.5);
}

// ============================================================

int
main(int argc, char ** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
