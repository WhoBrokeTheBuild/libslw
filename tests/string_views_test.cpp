
#include <slw/strings.hpp>
#include <slw/ranges.hpp>

#include <slw/containers/list.hpp>

#include <gtest/gtest.h>

using namespace slw;

TEST(StringViewsTest, Split)
{
    const auto& parts = string_views::split("Hello World ", ' ');
    ASSERT_EQ(parts.size(), 3);
    ASSERT_EQ(parts[0], "Hello");
    ASSERT_EQ(parts[1], "World");
    ASSERT_EQ(parts[2], "");

    const auto& single = string_views::split("abcdefg", ' ');
    ASSERT_EQ(single.size(), 1);
    ASSERT_EQ(single[0], "abcdefg");

    const auto& ip = string_views::split("127.0.0.1", '.');
    ASSERT_EQ(ip.size(), 4);
    ASSERT_TRUE(ranges::equal(ip, list<string>{ "127", "0", "0", "1" }));

    const auto& row = string_views::split("John,Doe,45,,555-1234", ',');
    ASSERT_EQ(row.size(), 5);
    ASSERT_TRUE(ranges::equal(row, list<string>{ "John", "Doe", "45", "", "555-1234" }));

    const auto& result = string_views::split("testing--123---hello----world", "--");
    ASSERT_EQ(result.size(), 5);
    ASSERT_TRUE(ranges::equal(result, list<string>{ "testing", "123", "-hello", "", "world" }));

    string str = "a,b,c";
    ASSERT_TRUE(ranges::equal(string_views::split(str, ','), list<string>{ "a", "b", "c" }));

    string_view view = "a,b,c";
    ASSERT_TRUE(ranges::equal(string_views::split(view, ','), list<string>{ "a", "b", "c" }));
}

TEST(StringViewsTest, Join)
{
    const auto& hello = string_views::join({ "hello", "world", "" }, ' ');
    ASSERT_EQ(hello, "hello world ");

    const auto& single = string_views::join({ "abcdefg" }, ' ');
    ASSERT_EQ(single, "abcdefg");

    const auto& ip = string_views::join({ "127", "0", "0", "1" }, '.');
    ASSERT_EQ(ip, "127.0.0.1");

    const auto& row = string_views::join({ "John", "Doe", "45", "", "555-1234" }, ',');
    ASSERT_EQ(row, "John,Doe,45,,555-1234");

    const auto& result = string_views::join({ "testing", "123", "-hello", "", "world" }, "--");
    ASSERT_EQ(result, "testing--123---hello----world");

    ASSERT_EQ(string_views::join({ "te", "st", "ing" }, ""), "testing");

    std::vector<std::string_view> views = { "a", "b", "c" };
    ASSERT_EQ(string_views::join(views, ','), "a,b,c");
}

int main(int argc, char * argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}