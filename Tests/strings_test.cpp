
#include <slw/strings.hpp>
#include <slw/ranges.hpp>

#include <slw/containers/list.hpp>

#include <gtest/gtest.h>

using namespace slw;

TEST(StringsTest, Split)
{
    const auto& parts = strings::split("Hello World ", ' ');
    ASSERT_EQ(parts.size(), 3);
    ASSERT_EQ(parts[0], "Hello");
    ASSERT_EQ(parts[1], "World");
    ASSERT_EQ(parts[2], "");

    const auto& single = strings::split("abcdefg", ' ');
    ASSERT_EQ(single.size(), 1);
    ASSERT_EQ(single[0], "abcdefg");

    const auto& ip = strings::split("127.0.0.1", '.');
    ASSERT_EQ(ip.size(), 4);
    ASSERT_TRUE(ranges::equal(ip, list<string>{ "127", "0", "0", "1" }));

    const auto& row = strings::split("John,Doe,45,,555-1234", ',');
    ASSERT_EQ(row.size(), 5);
    ASSERT_TRUE(ranges::equal(row, list<string>{ "John", "Doe", "45", "", "555-1234" }));

    const auto& result = strings::split("testing--123---hello----world", "--");
    ASSERT_EQ(result.size(), 5);
    ASSERT_TRUE(ranges::equal(result, list<string>{ "testing", "123", "-hello", "", "world" }));

    string str = "a,b,c";
    ASSERT_TRUE(ranges::equal(strings::split(str, ','), list<string>{ "a", "b", "c" }));

    string_view view = "a,b,c";
    ASSERT_TRUE(ranges::equal(strings::split(view, ','), list<string>{ "a", "b", "c" }));
}

TEST(StringsTest, Join)
{
    // const auto& hello = strings::join({ "hello", "world", "" }, ' ');
    // ASSERT_EQ(hello, "hello world ");

    // const auto& single = strings::join({ "abcdefg" }, ' ');
    // ASSERT_EQ(single, "abcdefg");

    // const auto& ip = strings::join({ "127", "0", "0", "1" }, '.');
    // ASSERT_EQ(ip, "127.0.0.1");

    // const auto& row = strings::join({ "John", "Doe", "45", "", "555-1234" }, ',');
    // ASSERT_EQ(row, "John,Doe,45,,555-1234");

    // const auto& result = strings::join({ "testing", "123", "-hello", "", "world" }, "--");
    // ASSERT_EQ(result, "testing--123---hello----world");

    // ASSERT_EQ(strings::join({ "te", "st", "ing" }, ""), "testing");

    // std::vector<std::string> strings = { "a", "b", "c" };
    // ASSERT_EQ(strings::join(strings, ','), "a,b,c");
}

int main(int argc, char * argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}