// This file contains code generated with the assistance of Claude (Anthropic),
// an AI assistant. The generated code is provided as-is.

#include "roah/assert.hpp"

#include <gtest/gtest.h>

#include <source_location>
#include <string>
#include <string_view>

#ifdef _MSC_VER
#    include <crtdbg.h>
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// AssertionError クラスのテスト
//
///////////////////////////////////////////////////////////////////////////////////////////////////

TEST(AssertionErrorTest, ConstructorStoresExpr)
{
    const auto           sl = std::source_location::current();
    roah::AssertionError err{ "x > 0", sl };
    EXPECT_EQ(err.getExpr(), "x > 0");
}

TEST(AssertionErrorTest, ConstructorStoresSourceLocation)
{
    const auto           sl = std::source_location::current();
    roah::AssertionError err{ "expr", sl };
    EXPECT_STREQ(err.getFilename(), sl.file_name());
    EXPECT_EQ(err.getLine(), sl.line());
    EXPECT_STREQ(err.getFunctionName(), sl.function_name());
}

TEST(AssertionErrorTest, GetSourceLocationMatches)
{
    const auto           sl = std::source_location::current();
    roah::AssertionError err{ "expr", sl };
    EXPECT_EQ(err.getSourceLocation().line(), sl.line());
    EXPECT_STREQ(err.getSourceLocation().file_name(), sl.file_name());
}

TEST(AssertionErrorTest, WhatContainsExprAndFile)
{
    const auto           sl = std::source_location::current();
    roah::AssertionError err{ "my_expr", sl };
    const std::string    what{ err.what() };
    EXPECT_NE(what.find("my_expr"), std::string::npos);
    EXPECT_NE(what.find(sl.file_name()), std::string::npos);
}

TEST(AssertionErrorTest, CopyConstructorPreservesData)
{
    const auto           sl = std::source_location::current();
    roah::AssertionError err1{ "copy_me", sl };
    roah::AssertionError err2 = err1;
    EXPECT_EQ(err2.getExpr(), err1.getExpr());
    EXPECT_EQ(err2.getLine(), err1.getLine());
    EXPECT_STREQ(err2.getFilename(), err1.getFilename());
}

TEST(AssertionErrorTest, MoveConstructorPreservesData)
{
    const auto           sl = std::source_location::current();
    roah::AssertionError err1{ "move_me", sl };
    const auto           line = err1.getLine();
    roah::AssertionError err2 = std::move(err1);
    EXPECT_EQ(err2.getExpr(), "move_me");
    EXPECT_EQ(err2.getLine(), line);
}

TEST(AssertionErrorTest, PrintMessageDoesNotThrow)
{
    const auto sl = std::source_location::current();
    EXPECT_NO_THROW(roah::AssertionError::printMessage("expr", sl));
    EXPECT_NO_THROW(roah::AssertionError::printMessage("expr", sl, "追加メッセージ"));
}

TEST(AssertionErrorTest, PrintMessageWithEmptyMessageDoesNotThrow)
{
    const auto sl = std::source_location::current();
    EXPECT_NO_THROW(roah::AssertionError::printMessage("expr", sl, ""));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// ROAH_CODING_ERROR のテスト
//
///////////////////////////////////////////////////////////////////////////////////////////////////

TEST(CodingErrorTest, DoesNotThrowWhenConditionIsTrue)
{
    EXPECT_NO_THROW(ROAH_CODING_ERROR(true));
}

TEST(CodingErrorTest, ThrowsAssertionErrorWhenConditionIsFalse)
{
    EXPECT_THROW(ROAH_CODING_ERROR(false), roah::AssertionError);
}

TEST(CodingErrorTest, ThrownExceptionContainsExpression)
{
    int a = 1;
    int b = 2;
    try
    {
        ROAH_CODING_ERROR(a == b);
        FAIL() << "AssertionError が送出されることを期待した";
    }
    catch (const roah::AssertionError & e)
    {
        EXPECT_NE(std::string_view{ e.getExpr() }.find("a == b"), std::string_view::npos);
    }
}

TEST(CodingErrorTest, ThrownExceptionIsStdLogicError)
{
    const bool value = false;
    EXPECT_THROW(ROAH_CODING_ERROR(value), std::logic_error);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// ROAH_CODING_ERROR_C のテスト
//
///////////////////////////////////////////////////////////////////////////////////////////////////

TEST(CodingErrorConstexprTest, DoesNotThrowWhenTrue)
{
    EXPECT_NO_THROW(ROAH_CODING_ERROR_C(true));
}

TEST(CodingErrorConstexprTest, ThrowsAssertionErrorWhenFalse)
{
    EXPECT_THROW(ROAH_CODING_ERROR_C(false), roah::AssertionError);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// ROAH_CODING_ERROR_M のテスト
//
///////////////////////////////////////////////////////////////////////////////////////////////////

TEST(CodingErrorWithMessageTest, DoesNotThrowWhenConditionIsTrue)
{
    EXPECT_NO_THROW(ROAH_CODING_ERROR_M(true, "should not fail"));
}

TEST(CodingErrorWithMessageTest, ThrowsAssertionErrorWhenConditionIsFalse)
{
    EXPECT_THROW(ROAH_CODING_ERROR_M(false, "error message"), roah::AssertionError);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// ROAH_CODING_ERROR_M_C のテスト
//
///////////////////////////////////////////////////////////////////////////////////////////////////

TEST(CodingErrorWithMessageConstexprTest, DoesNotThrowWhenTrue)
{
    EXPECT_NO_THROW(ROAH_CODING_ERROR_M_C(true, "should not fail"));
}

TEST(CodingErrorWithMessageConstexprTest, ThrowsAssertionErrorWhenFalse)
{
    EXPECT_THROW(ROAH_CODING_ERROR_M_C(false, "error message"), roah::AssertionError);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// ROAH_ASSERT_ENABLE 時のみ有効なテスト
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef ROAH_ASSERT_ENABLE

// assert が発火する = プロセス終了 のため, EXPECT_DEATH (子プロセスで実行して終了を観測)
// を使う. GTest が子プロセスをスポーンするとき main() が再実行されるため,
// main() 内の CRT 設定も子プロセスで有効になる.

TEST(AssertionMacroDebugTest, DoesNotTriggerAssertWhenConditionIsTrue)
{
    // 条件が真のとき assert は発火せず, プロセスは継続する.
    ROAH_ASSERTION(true);
    SUCCEED();
}

TEST(AssertionMacroDebugTest, TriggersAssertWhenConditionIsFalse)
{
    EXPECT_DEATH(ROAH_ASSERTION(false), "");
}

TEST(AssertionConstexprDebugTest, DoesNotTriggerAssertWhenTrue)
{
    ROAH_ASSERTION_C(true);
    SUCCEED();
}

TEST(AssertionConstexprDebugTest, TriggersAssertWhenFalse)
{
    EXPECT_DEATH(ROAH_ASSERTION_C(false), "");
}

TEST(NotImplementedDebugTest, TriggersAssert)
{
    EXPECT_DEATH({ ROAH_NOT_IMPLEMENTED(); }, "");
}

TEST(VerifyDebugTest, ReturnsTrueWhenTrue)
{
    const bool result = ROAH_VERIFY(true);
    EXPECT_TRUE(result);
}

TEST(VerifyDebugTest, TriggersAssertWhenFalse)
{
    EXPECT_DEATH(ROAH_VERIFY(false), "");
}

#else  // !ROAH_ASSERT_ENABLE

TEST(AssertionMacroReleaseTest, MacroExpandsToNothing)
{
    // Release ビルドでは ROAH_ASSERTION は何も展開しない.
    // コンパイルが通ること自体がテスト.
    ROAH_ASSERTION(false);
    SUCCEED();
}

TEST(NotImplementedReleaseTest, OnlyCallsPrintMessageWithoutAbort)
{
    // Release ビルドでは ROAH_NOT_IMPLEMENTED は assert を呼ばず printMessage のみ実行する.
    EXPECT_NO_THROW(ROAH_NOT_IMPLEMENTED());
}

#endif  // ROAH_ASSERT_ENABLE

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// main
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef ROAH_ASSERT_LOGGER_ENABLE
#    include "roah/logger.hpp"
#endif

int
main(int argc, char ** argv)
{
#ifdef _MSC_VER
    // MSVC デバッグ CRT: アサーション発生時にダイアログを抑制して abort() まで直接到達させる.
    // EXPECT_DEATH が子プロセス (--gtest_internal_run_death_test) で正常動作するために必要.
    // 子プロセスも main() を再実行するため, この設定は子プロセスでも有効になる.
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
    _set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);
#endif

#ifdef ROAH_ASSERT_LOGGER_ENABLE
    roah::initializeLogger("roah-assert-test", {});
#endif

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
