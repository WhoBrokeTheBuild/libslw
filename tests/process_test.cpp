#include <slw/process.hpp>

#include <gtest/gtest.h>

using namespace slw;

TEST(Process, Run)
{
    auto result = process::run("/bin/echo", { "hello world" });
    ASSERT_EQ(result.code, 0);
    ASSERT_EQ(result.out, "hello world\n");
    ASSERT_EQ(result.err, "");
}

TEST(Process, BackAndForth)
{
    process echo("/bin/cat", { "-" });

    echo.communicate("hello\n");
    std::this_thread::sleep_for(100ms);
    ASSERT_EQ(echo.result().out, "");

    echo.communicate("world\n");
    std::this_thread::sleep_for(100ms);
    ASSERT_EQ(echo.result().out, "hello\n");

    echo.send_signal(SIGINT);
    echo.wait();

    auto result = echo.result();
    ASSERT_EQ(result.code, 0);
    ASSERT_EQ(result.out, "hello\nworld\n");
    ASSERT_EQ(result.err, "");
}

int main(int argc, char * argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
