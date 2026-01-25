
#include <slw/to_string.hpp>

#include <gtest/gtest.h>

using namespace slw;

TEST(ToStringTest, array)
{
    // ASSERT_EQ(to_string(array<int, 0>()), "[0]{ }");
    ASSERT_EQ(to_string(array<int, 0>()), "[ ]");

    array<int, 3> values = { 1, 2, 3 };
    // ASSERT_EQ(to_string(values), "[3]{ 1, 2, 3, }");
    ASSERT_EQ(to_string(values), "[ 1, 2, 3, ]");
}

TEST(ToStringTest, dict)
{
    // ASSERT_EQ(to_string(ordered_dict<int, int>()), "{ }");
    // ASSERT_EQ(to_string(dict<int, int>()), "{ }");
    ASSERT_EQ(to_string(ordered_dict<int, int>()), "[ ]");
    ASSERT_EQ(to_string(dict<int, int>()), "[ ]");

    ordered_dict<int, int> ordered = {
        { 42, 52 },
        { 56, 64 },
        { 64, 98 }
    };

    dict<int, int> unordered(ordered.begin(), ordered.end());

    // ASSERT_EQ(to_string(ordered), "{ 42: 52, 56: 64, 64: 98, }");
    ASSERT_EQ(to_string(ordered), "[ ( 42, 52 ), ( 56, 64 ), ( 64, 98 ), ]");
    ASSERT_EQ(to_string(unordered).size(), to_string(ordered).size());

    ordered_dict<char, string> alphabet = {
        { 'a', "addressof" },
        { 'b', "break" },
        { 'c', "continue" },
    };

    // ASSERT_EQ(to_string(alphabet), "{ 'a': \"addressof\", 'b': \"break\", 'c': \"continue\", }");
    ASSERT_EQ(to_string(alphabet), "[ ( 'a', \"addressof\" ), ( 'b', \"break\" ), ( 'c', \"continue\" ), ]");

    ASSERT_EQ(to_string(pair<int, int>(42, 45)), "( 42, 45 )");

    list<pair<char, string>> pairs(alphabet.begin(), alphabet.end());
    ASSERT_EQ(to_string(pairs), "[ ( 'a', \"addressof\" ), ( 'b', \"break\" ), ( 'c', \"continue\" ), ]");
}

TEST(ToStringList, initializer_list)
{
    ASSERT_EQ(to_string(initializer_list<int>{ }), "{ }");
    ASSERT_EQ(to_string(initializer_list<int>{ 1, 2, 3 }), "{ 1, 2, 3, }");
    ASSERT_EQ(to_string(initializer_list<char>{ 'a', 'b', 'c' }), "{ 'a', 'b', 'c', }");
    ASSERT_EQ(to_string(initializer_list<const char *>{ "hello", "world" }), "{ \"hello\", \"world\", }");
    ASSERT_EQ(to_string(initializer_list<string>{ "hello", "world" }), "{ \"hello\", \"world\", }");
    ASSERT_EQ(to_string(initializer_list<string_view>{ "hello", "world" }), "{ \"hello\", \"world\", }");
}

TEST(ToStringTest, list_view)
{
    ASSERT_EQ(to_string(list<int>()), "[ ]");
    ASSERT_EQ(to_string(view<int>()), "[ ]");

    list<int> ints = { 1, 2, 3 };
    ASSERT_EQ(to_string(ints), "[ 1, 2, 3, ]");
    ASSERT_EQ(to_string(view<int>(ints)), "[ 1, 2, 3, ]");

    list<char> chars = { 'a', 'b', 'c' };
    ASSERT_EQ(to_string(chars), "[ 'a', 'b', 'c', ]");
    ASSERT_EQ(to_string(view<char>(chars)), "[ 'a', 'b', 'c', ]");

    list<string> strings = { "hello", "world" };
    ASSERT_EQ(to_string(strings), "[ \"hello\", \"world\", ]");
    ASSERT_EQ(to_string(view<string>(strings)), "[ \"hello\", \"world\", ]");
}

// TEST(ToStringTest, ranges)
// {

// }

TEST(ToStringTest, set)
{
    // ASSERT_EQ(to_string(ordered_set<int>()), "{ }");
    // ASSERT_EQ(to_string(set<int>()), "{ }");
    ASSERT_EQ(to_string(ordered_set<int>()), "[ ]");
    ASSERT_EQ(to_string(set<int>()), "[ ]");

    ordered_set<int> ordered = { 1, 2, 3, 2, 1 };

    set<int> unordered(ordered.begin(), ordered.end());

    // ASSERT_EQ(to_string(ordered), "{ 1, 2, 3, }");
    ASSERT_EQ(to_string(ordered), "[ 1, 2, 3, ]");
    ASSERT_EQ(to_string(unordered).size(), to_string(ordered).size());
}

TEST(ToStringTest, tuple)
{
    ASSERT_EQ(to_string(tuple<>()), "( )");

    tuple<int, int, int> coord(1, 2, 3);
    ASSERT_EQ(to_string(coord), "( 1, 2, 3, )");

    tuple<string, int, float> person("John Doe", 45, 4.0);
    ASSERT_EQ(to_string(person), "( \"John Doe\", 45, 4.000000, )");
}

int main(int argc, char * argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}