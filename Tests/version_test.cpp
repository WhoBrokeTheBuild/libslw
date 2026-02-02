
#include <slw/version.hpp>

#include <gtest/gtest.h>

using namespace slw;

TEST(VersionTest, strings)
{
    ASSERT_EQ(std::format("{}", version()), "0.0.0");

    // std::format("{}", std::array<int, 5>());
}

TEST(VersionTest, compare)
{
    ASSERT_EQ(version(), version());
    ASSERT_EQ(version(1), version(1));
    ASSERT_EQ(version(1, 2), version(1, 2));
    ASSERT_EQ(version(1, 2, 3), version(1, 2, 3));

    ASSERT_NE(version(1), version(3));
    ASSERT_NE(version(1, 2), version(3, 2));
    ASSERT_NE(version(1, 2, 3), version(3, 2, 1));

    ASSERT_LT(version(), version(1));
    ASSERT_LT(version(), version(1, 2));
    ASSERT_LT(version(), version(1, 2, 3));
    ASSERT_LT(version(1), version(2));
    ASSERT_LT(version(1, 2), version(1, 3));
    ASSERT_LT(version(1, 2, 3), version(1, 2, 4));

    ASSERT_GT(version(1), version());
    ASSERT_GT(version(1, 2), version());
    ASSERT_GT(version(1, 2, 3), version());
    ASSERT_GT(version(2), version(1));
    ASSERT_GT(version(1, 3), version(1, 2));
    ASSERT_GT(version(1, 2, 4), version(1, 2, 3));

    ASSERT_LE(version(), version());
    ASSERT_LE(version(1), version(1));
    ASSERT_LE(version(1, 2), version(1, 2));
    ASSERT_LE(version(1, 2, 3), version(1, 2, 3));
    ASSERT_LE(version(), version(1));
    ASSERT_LE(version(), version(1, 2));
    ASSERT_LE(version(), version(1, 2, 3));
    ASSERT_LE(version(1), version(2));
    ASSERT_LE(version(1, 2), version(1, 3));
    ASSERT_LE(version(1, 2, 3), version(1, 2, 4));

    ASSERT_GE(version(), version());
    ASSERT_GE(version(1), version(1));
    ASSERT_GE(version(1, 2), version(1, 2));
    ASSERT_GE(version(1, 2, 3), version(1, 2, 3));
    ASSERT_GE(version(1), version());
    ASSERT_GE(version(1, 2), version());
    ASSERT_GE(version(1, 2, 3), version());
    ASSERT_GE(version(2), version(1));
    ASSERT_GE(version(1, 3), version(1, 2));
    ASSERT_GE(version(1, 2, 4), version(1, 2, 3));
}

int main(int argc, char * argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}