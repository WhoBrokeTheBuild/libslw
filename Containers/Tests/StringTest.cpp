
#include <gtest/gtest.h>

#include <slw/String.hpp>
#include <slw/List.hpp>

using namespace slw;

TEST(String, Contains)
{
    ASSERT_TRUE(String::Contains("hello", 'e'));
    ASSERT_FALSE(String::Contains("hello", 'q'));

    ASSERT_TRUE(String::Contains("hello", "ll"));
    ASSERT_FALSE(String::Contains("hello", "eh"));

    ASSERT_TRUE(String::Contains("", ""));
    ASSERT_FALSE(String::Contains("", "abc"));

    std::string string = "hello";
    ASSERT_TRUE(String::Contains(string, "hello"));
    ASSERT_FALSE(String::Contains(string, "world"));

    std::string_view view = "hello";
    ASSERT_TRUE(String::Contains(view, "hello"));
    ASSERT_FALSE(String::Contains(view, "world"));
}

TEST(String, Split)
{
    const auto& parts = String::Split("Hello World ", ' ');
    ASSERT_EQ(parts.size(), 3);
    ASSERT_EQ(parts[0], "Hello");
    ASSERT_EQ(parts[1], "World");
    ASSERT_EQ(parts[2], "");

    const auto& single = String::Split("abcdefg", ' ');
    ASSERT_EQ(single.size(), 1);
    ASSERT_EQ(single[0], "abcdefg");

    const auto& ip = String::Split("127.0.0.1", '.');
    ASSERT_EQ(ip.size(), 4);
    ASSERT_TRUE(List::Equals(ip, { "127", "0", "0", "1" }));

    const auto& row = String::Split("John,Doe,45,,555-1234", ',');
    ASSERT_EQ(row.size(), 5);
    ASSERT_TRUE(List::Equals(row, { "John", "Doe", "45", "", "555-1234" }));

    const auto& result = String::Split("testing--123---hello----world", "--");
    ASSERT_EQ(result.size(), 5);
    ASSERT_TRUE(List::Equals(result, { "testing", "123", "-hello", "", "world" }));

    std::string string = "a,b,c";
    ASSERT_TRUE(List::Equals(String::Split(string, ','), { "a", "b", "c" }));

    std::string_view view = "a,b,c";
    ASSERT_TRUE(List::Equals(String::Split(view, ','), { "a", "b", "c" }));
}

TEST(String, Join)
{
    const auto& hello = String::Join({ "hello", "world", "" }, ' ');
    ASSERT_EQ(hello, "hello world ");

    const auto& single = String::Join({ "abcdefg" }, ' ');
    ASSERT_EQ(single, "abcdefg");

    const auto& ip = String::Join({ "127", "0", "0", "1" }, '.');
    ASSERT_EQ(ip, "127.0.0.1");

    const auto& row = String::Join({ "John", "Doe", "45", "", "555-1234" }, ',');
    ASSERT_EQ(row, "John,Doe,45,,555-1234");

    const auto& result = String::Join({ "testing", "123", "-hello", "", "world" }, "--");
    ASSERT_EQ(result, "testing--123---hello----world");

    ASSERT_EQ(String::Join({ "te", "st", "ing" }, ""), "testing");

    std::vector<std::string> strings = { "a", "b", "c" };
    ASSERT_EQ(String::Join(strings, ','), "a,b,c");

    std::vector<std::string_view> views = { "a", "b", "c" };
    ASSERT_EQ(String::Join(views, ','), "a,b,c");
}

int main(int argc, char * argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}