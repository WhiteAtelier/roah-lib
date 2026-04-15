// This file contains code generated with the assistance of Claude (Anthropic),
// an AI assistant. The generated code is provided as-is.

#include "roah/process_id.hpp"

// プラットフォーム API との比較に使用.
#if defined(ROAH_ARCH_WIN32)
#    include <Windows.h>
#elif defined(ROAH_ARCH_LINUX)
#    include <unistd.h>
#else
#    error "Unsupported platform"
#endif

#include <gtest/gtest.h>

#include <string>

namespace {
#if defined(ROAH_ARCH_WIN32)
using NativePIDType = DWORD;
#elif defined(ROAH_ARCH_LINUX)
using NativePIDType = pid_t;
#endif
}  // namespace

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// ProcessID クラスのテスト
//
///////////////////////////////////////////////////////////////////////////////////////////////////

TEST(ProcessIDTest, DefaultConstructorIsZero)
{
    const roah::ProcessID pid{};
    EXPECT_EQ(static_cast<NativePIDType>(pid), NativePIDType{ 0 });
}

TEST(ProcessIDTest, CopyConstructorPreservesValue)
{
    const roah::ProcessID pid1 = roah::getCurrentProcessID();
    const roah::ProcessID pid2 = pid1;
    EXPECT_EQ(static_cast<NativePIDType>(pid1), static_cast<NativePIDType>(pid2));
}

TEST(ProcessIDTest, MoveConstructorTransfersValue)
{
    roah::ProcessID       pid1 = roah::getCurrentProcessID();
    const auto            val  = static_cast<NativePIDType>(pid1);
    const roah::ProcessID pid2 = std::move(pid1);
    EXPECT_EQ(static_cast<NativePIDType>(pid2), val);
    // ムーブ後のソースは 0 になる
    EXPECT_EQ(static_cast<NativePIDType>(pid1), NativePIDType{ 0 });
}

TEST(ProcessIDTest, CopyAssignmentPreservesValue)
{
    const roah::ProcessID pid1 = roah::getCurrentProcessID();
    roah::ProcessID       pid2{};
    pid2 = pid1;
    EXPECT_EQ(static_cast<NativePIDType>(pid1), static_cast<NativePIDType>(pid2));
}

TEST(ProcessIDTest, MoveAssignmentTransfersValue)
{
    roah::ProcessID pid1 = roah::getCurrentProcessID();
    const auto      val  = static_cast<NativePIDType>(pid1);
    roah::ProcessID pid2{};
    pid2 = std::move(pid1);
    EXPECT_EQ(static_cast<NativePIDType>(pid2), val);
    // ムーブ後のソースは 0 になる
    EXPECT_EQ(static_cast<NativePIDType>(pid1), NativePIDType{ 0 });
}

TEST(ProcessIDTest, SelfMoveAssignmentIsSafe)
{
    roah::ProcessID pid = roah::getCurrentProcessID();
    [[maybe_unused]]
    const auto val
        = static_cast<NativePIDType>(pid);
    pid = std::move(pid);  // NOLINT(bugprone-use-after-move)
    // 自己代入後は値が保持されるか, 少なくともクラッシュしない
    SUCCEED();
}

TEST(ProcessIDTest, EqualityOperatorReturnsTrueForSameValue)
{
    const roah::ProcessID pid1 = roah::getCurrentProcessID();
    const roah::ProcessID pid2 = pid1;
    EXPECT_TRUE(pid1 == pid2);
}

TEST(ProcessIDTest, EqualityOperatorReturnsFalseForDifferentValues)
{
    const roah::ProcessID zero{};
    const roah::ProcessID current = roah::getCurrentProcessID();
    EXPECT_FALSE(zero == current);
}

TEST(ProcessIDTest, ToStringDefaultIsZero)
{
    const roah::ProcessID pid{};
    EXPECT_EQ(pid.toString(), "0");
}

TEST(ProcessIDTest, ToStringCurrentIsNonEmpty)
{
    const roah::ProcessID pid = roah::getCurrentProcessID();
    EXPECT_FALSE(pid.toString().empty());
}

TEST(ProcessIDTest, ToHexStringDefaultIsZero)
{
    const roah::ProcessID pid{};
    EXPECT_EQ(pid.toHexString(), "0");
}

TEST(ProcessIDTest, ToHexStringCurrentIsNonEmpty)
{
    const roah::ProcessID pid = roah::getCurrentProcessID();
    EXPECT_FALSE(pid.toHexString().empty());
}

TEST(ProcessIDTest, ToStringWithKnownValue)
{
    const roah::ProcessID pid{ NativePIDType{ 255 } };
    EXPECT_EQ(pid.toString(), "255");
}

TEST(ProcessIDTest, ToStringWithZeroValue)
{
    const roah::ProcessID pid{ NativePIDType{ 0 } };
    EXPECT_EQ(pid.toString(), "0");
}

TEST(ProcessIDTest, ToHexStringWithKnownValue)
{
    const roah::ProcessID pid{ NativePIDType{ 255 } };
    EXPECT_EQ(pid.toHexString(), "ff");
}

TEST(ProcessIDTest, ToHexStringWithPowerOfTwo)
{
    const roah::ProcessID pid{ NativePIDType{ 256 } };
    EXPECT_EQ(pid.toHexString(), "100");
}

TEST(ProcessIDTest, ToHexStringWithZeroValue)
{
    const roah::ProcessID pid{ NativePIDType{ 0 } };
    EXPECT_EQ(pid.toHexString(), "0");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// getCurrentProcessID() のテスト
//
///////////////////////////////////////////////////////////////////////////////////////////////////

TEST(GetCurrentProcessIDTest, ReturnsNonZeroPID)
{
    const roah::ProcessID pid = roah::getCurrentProcessID();
    EXPECT_NE(static_cast<NativePIDType>(pid), NativePIDType{ 0 });
}

TEST(GetCurrentProcessIDTest, ConsistentAcrossMultipleCalls)
{
    const roah::ProcessID pid1 = roah::getCurrentProcessID();
    const roah::ProcessID pid2 = roah::getCurrentProcessID();
    EXPECT_EQ(static_cast<NativePIDType>(pid1), static_cast<NativePIDType>(pid2));
}

TEST(GetCurrentProcessIDTest, MatchesPlatformAPI)
{
#if defined(ROAH_ARCH_WIN32)
    const NativePIDType expected = ::GetCurrentProcessId();
#elif defined(ROAH_ARCH_LINUX)
    const NativePIDType expected = ::getpid();
#endif
    const roah::ProcessID pid = roah::getCurrentProcessID();
    EXPECT_EQ(static_cast<NativePIDType>(pid), expected);
}

TEST(GetCurrentProcessIDTest, ToStringMatchesPlatformAPI)
{
#if defined(ROAH_ARCH_WIN32)
    const std::string expected = std::to_string(::GetCurrentProcessId());
#elif defined(ROAH_ARCH_LINUX)
    const std::string expected = std::to_string(::getpid());
#endif
    const roah::ProcessID pid = roah::getCurrentProcessID();
    EXPECT_EQ(pid.toString(), expected);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// main
//
///////////////////////////////////////////////////////////////////////////////////////////////////

int
main(int argc, char ** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
