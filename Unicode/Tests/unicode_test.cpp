
#include <slw/unicode.hpp>

#include <gtest/gtest.h>

using namespace slw;

TEST(UnicodeTest, Length)
{
    EXPECT_EQ(unicode::length(u8"hello"), 5);
    EXPECT_EQ(unicode::length(u8"🔥"), 1);
    EXPECT_EQ(unicode::length(u8"© 2026"), 6);
    EXPECT_EQ(unicode::length(u8"Hello, my name is ᛏᚱᛅᚴᛁ"), 23);
    
    char8_t invalid = 0b10000000;
    EXPECT_THROW(unicode::length({&invalid, 1}), unicode::invalid_sequence);
}

int main(int argc, char * argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}