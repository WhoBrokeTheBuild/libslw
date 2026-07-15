
#include <slw/unicode.hpp>

#include <gtest/gtest.h>

using namespace slw;

TEST(UnicodeTest, Length)
{
    EXPECT_EQ(unicode::length(u8"hello"), 5);
    EXPECT_EQ(unicode::length(u8"🔥"), 1);
    EXPECT_EQ(unicode::length(u8"© 2026"), 6);
    EXPECT_EQ(unicode::length(u8"Hello, my name is ᛏᚱᛅᚴᛁ"), 23);

    EXPECT_EQ(u8string_view(u8"hello").length(), 5);
    EXPECT_EQ(u8string_view(u8"🔥").length(), 4);
    EXPECT_EQ(u8string_view(u8"© 2026").length(), 7);
    EXPECT_EQ(u8string_view(u8"Hello, my name is ᛏᚱᛅᚴᛁ").length(), 33);
    
    char8_t invalid = 0b10000000;
    EXPECT_THROW(unicode::length({&invalid, 1}), unicode::invalid_sequence);
}

TEST(UnicodeTest, Convert)
{
    EXPECT_EQ(unicode::to_utf32(u8"hello"), U"hello");
    EXPECT_EQ(unicode::to_utf8(U"hello"), u8"hello");

    EXPECT_EQ(unicode::to_utf32(u8"🔥"), U"🔥");
    EXPECT_EQ(unicode::to_utf8(U"🔥"), u8"🔥");

    EXPECT_EQ(unicode::to_utf32(u8"© 2026"), U"© 2026");
    EXPECT_EQ(unicode::to_utf8(U"© 2026"), u8"© 2026");

    EXPECT_EQ(unicode::to_utf32(u8"Hello, my name is ᛏᚱᛅᚴᛁ"), U"Hello, my name is ᛏᚱᛅᚴᛁ");
    EXPECT_EQ(unicode::to_utf8(U"Hello, my name is ᛏᚱᛅᚴᛁ"), u8"Hello, my name is ᛏᚱᛅᚴᛁ");

    char8_t invalid8 = 0b10000000;
    EXPECT_THROW(unicode::to_utf32({&invalid8, 1}), unicode::invalid_sequence);

    char32_t invalid32 = 0xFFFFFFFF;
    EXPECT_THROW(unicode::to_utf8({&invalid32, 1}), unicode::invalid_sequence);
}

TEST(UnicodeTest, CaseFold)
{
    ASSERT_EQ(unicode::case_fold(U"Hello World"), U"hello world");
    ASSERT_EQ(unicode::case_fold(u8"Hello World"), u8"hello world");

    EXPECT_EQ(unicode::case_fold(U"🔥"), U"🔥");
    EXPECT_EQ(unicode::case_fold(u8"🔥"), u8"🔥");

    EXPECT_EQ(unicode::case_fold(U"ÀÁÂÃÄÅÖÜẞÑŌ"), U"àáâãäåöüßñō");
    EXPECT_EQ(unicode::case_fold(u8"ÀÁÂÃÄÅÖÜẞÑŌ"), u8"àáâãäåöüßñō");

    EXPECT_EQ(unicode::case_fold(U"ΑΒΓΔΕΖΗΘΙΚΛΜΝΞΟΠΡΣΤΥΦΧΨΩ"), U"αβγδεζηθικλμνξοπρστυφχψω");
    EXPECT_EQ(unicode::case_fold(u8"ΑΒΓΔΕΖΗΘΙΚΛΜΝΞΟΠΡΣΤΥΦΧΨΩ"), u8"αβγδεζηθικλμνξοπρστυφχψω");
}

TEST(UnicodeTest, Strings)
{
    u8string utf8(u8"hello world");
    const auto& parts8 = u8string_views::split(utf8, ' ');
    ASSERT_EQ(parts8.size(), 2);

    u32string utf32(U"hello world");
    const auto& parts32 = u32string_views::split(utf32, ' ');
    ASSERT_EQ(parts32.size(), 2);

    const auto& temp = u8string_views::split(u8"hello world", ' ');
    ASSERT_EQ(temp.size(), 2);
}

int main(int argc, char * argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}