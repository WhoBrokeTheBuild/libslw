
#include <gtest/gtest.h>

#include <slw/UTF8.hpp>

using namespace slw;

TEST(UTF8, Length)
{
    EXPECT_EQ(UTF8::Length(u8"hello"), 5);
    EXPECT_EQ(UTF8::Length(u8"🔥"), 1);
    EXPECT_EQ(UTF8::Length(u8"© 2026"), 6);
    EXPECT_EQ(UTF8::Length(u8"Hello, my name is ᛏᚱᛅᚴᛁ"), 23);
    
    char8_t invalid = 0b10000000;
    EXPECT_THROW(UTF8::Length({&invalid, 1}), UTF8::InvalidSequence);
}

int main(int argc, char * argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}