#include <slw/argument_parser.hpp>

#include <gtest/gtest.h>

using namespace slw;

TEST(ArgumentParser, Simple)
{
    argument_parser parser;

    bool help = false;
    parser.add_bool('h', "help", &help, "display this help and exit");

    int count = 0;
    parser.add_int('c', "count", &count, "enter a number");

    float amount = 0.f;
    parser.add_float('a', "amount", &amount, "enter a float");

    string name;
    parser.add_string('\0', "name", &name, "enter a name");

    bool long_name = false;
    parser.add_bool('q', "really-long-argument-name", &long_name, "testing really long argument names");

    list<const char *> argv({
        "test",
        "-c", "4",
        "--amount=3.14",
        "--name=Test McTest",
        "--really-long-argument-name",
        "one", "two", "three"
    });
    parser.parse(argv.size(), const_cast<char **>(argv.data()));

    EXPECT_FALSE(help);
    EXPECT_EQ(count, 4);
    EXPECT_EQ(amount, 3.14f);
    EXPECT_EQ(name, "Test McTest");
    EXPECT_TRUE(long_name);

    EXPECT_PRED2(ranges::equal, parser.args, list<string>({ "one", "two", "three" }));
    EXPECT_EQ(parser.argc, 3);
    EXPECT_EQ(parser.argv[0], "one");
    EXPECT_EQ(parser.argv[1], "two");
    EXPECT_EQ(parser.argv[2], "three");
}

TEST(ArgumentParser, Repeat)
{
    argument_parser parser;

    auto verbose = parser.add_bool('v', "verbose", nullptr, "enables verbose output, repeat up to 4 times for more verbosity");

    list<const char *> argv({ "test", "-vvv" });
    parser.parse(argv.size(), const_cast<char **>(argv.data()));

    EXPECT_EQ(verbose->count, 3);
}

TEST(ArgumentParser, Escape)
{
    argument_parser parser;

    bool help = false;
    parser.add_bool('h', "help", &help, "display this help and exit");

    list<const char *> argv({ "test", "--", "-h" });
    parser.parse(argv.size(), const_cast<char **>(argv.data()));

    EXPECT_FALSE(help);
}

TEST(ArgumentParser, Callbacks)
{
    argument_parser parser;

    list<string> strings;
    parser.add_string_callback(
        's', "string", 
        [&](string value) {
            strings.push_back(value);
        },
        "add a string"
    );

    list<bool> bools;
    parser.add_bool_callback(
        'b', "bool",
        [&](bool value) {
            bools.push_back(value);
        },
        "add a boolean"
    );

    list<int> ints;
    parser.add_int_callback(
        'i', "int",
        [&](int value) {
            ints.push_back(value);
        },
        "add an int"
    );

    list<float> floats;
    parser.add_float_callback(
        'f', "float",
        [&](float value) {
            floats.push_back(value);
        },
        "add a float"
    );

    list<const char *> argv({
        "test",
        "-s", "input.txt",
        "--string", "Input File.docx",
        "-b",
        "--bool=false",
        "--bool", "1",
        "-i", "42",
        "--int=123",
        "-f", "3.14",
        "--float", "0.0001",
    });
    parser.parse(argv.size(), const_cast<char **>(argv.data()));

    EXPECT_PRED2(ranges::equal, strings, list<string>({ "input.txt", "Input File.docx" }));
    EXPECT_PRED2(ranges::equal, bools, list<bool>({ true, false, true }));
    EXPECT_PRED2(ranges::equal, ints, list<int>({ 42, 123 }));
    EXPECT_PRED2(ranges::equal, floats, list<float>({ 3.14f, 0.0001f }));
}

TEST(ArgumentParser, Usage)
{
    argument_parser parser;

    bool help = false;
    parser.add_bool('h', "help", &help, "display this help and exit");

    int count = 0;
    parser.add_int('c', "count", &count, "enter a number");

    float amount = 0.f;
    parser.add_float('a', "amount", &amount, "enter a float");

    string name;
    parser.add_string('\0', "name", &name, "enter a name");

    bool long_name = false;
    parser.add_bool('q', "really-long-argument-name", &long_name, "testing really long argument names");

    parser.program = "argument_parser_test";
    string usage = parser.get_usage(
            "[OPTION]... [ARG]...", 
            "Tests the argument_parser in libslw.", 
            "Additional information about this library can be found by contacting:\n"
            "  sdl.slane@gmail.com");

    string compare = "Usage: argument_parser_test [OPTION]... [ARG]...\n"
        "Tests the argument_parser in libslw.\n"
        "\n"
        "  -h, --help                display this help and exit\n"
        "  -c, --count               enter a number\n"
        "  -a, --amount              enter a float\n"
        "      --name                enter a name\n"
        "  -q, --really-long-argument-name\n"
        "                            testing really long argument names\n"
        "\n"
        "Additional information about this library can be found by contacting:\n"
        "  sdl.slane@gmail.com\n";

    EXPECT_EQ(usage, compare);
}

int main(int argc, char * argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}