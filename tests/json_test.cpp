#include <slw/json.hpp>
#include <slw/io.hpp>

#include <gtest/gtest.h>

TEST(JSON, Parse)
{
    slw::json data1 = slw::json::parse("{ \"hello\": \"world\" }");
    ASSERT_EQ(data1["hello"].get<slw::string>(), "world");
    slw::json data2 = slw::json::parse("{ \"numbers\": [1, 2, 3, 4] }");
    ASSERT_TRUE(slw::ranges::equal(data2["numbers"].get<slw::list<int>>(), slw::list<int>{1, 2, 3, 4}));

    slw::json data3 = slw::json::parse(slw::file("test.json"));
    ASSERT_EQ(data3["name"].get<slw::string>(), "France");
    ASSERT_EQ(data3["area"].get<int>(), 551695);
    ASSERT_EQ(data3["languages"][0].get<slw::string>(), "French");
}

TEST(JSON, Dump)
{
    slw::ordered_json data1;
    data1["hello"] = "world";
    ASSERT_EQ(data1.dump(), "{\"hello\":\"world\"}");
    ASSERT_EQ(data1.dump(2), "{\n  \"hello\": \"world\"\n}");

    slw::ordered_json data2;
    data2["strings"] = { "abc", "123" };
    data2["numbers"] = { 1, 2, 3 };
    data2["bools"] = { true, false, true };
    ASSERT_EQ(data2.dump(), "{\"strings\":[\"abc\",\"123\"],\"numbers\":[1,2,3],\"bools\":[true,false,true]}");
}

int main(int argc, char * argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
