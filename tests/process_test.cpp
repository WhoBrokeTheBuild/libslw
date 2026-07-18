#include <slw/process.hpp>
#include <slw/file.hpp>

#include <gtest/gtest.h>

using namespace slw;

TEST(Process, Simple)
{
    auto result = process({ "/bin/echo", "hello world" }).wait();
    EXPECT_EQ(result.code, 0);
    EXPECT_EQ(result.output, "hello world\n");
    EXPECT_EQ(result.error, "");
}

TEST(Process, Failure)
{
    ASSERT_THROW(process({ "asdf" }), system_error);

    ASSERT_THROW(process().open({ "asdf" }), system_error);

    process proc({ "/bin/true" });
    proc.wait();
    ASSERT_THROW(proc.wait(), runtime_error);
}

TEST(Process, BackAndForth)
{
    string out, err;

    process proc(
        { "/bin/cat", "-" },
        {
            .input = process::PIPE,
            .output = process::PIPE,
        }
    );

    proc.write_input("hello\n");
    tie(out, err) = proc.read_output(100ms);

    proc.write_input("world\n");
    tie(out, err) = proc.read_output(100ms);

    proc.interrupt();

    auto result = proc.wait();
    EXPECT_EQ(result.code, 0);
    EXPECT_EQ(result.output, "");
    EXPECT_EQ(result.error, "");
}

TEST(Process, Timeout)
{
    string out, err;

    process proc(
        { "/bin/sh", "-c", "echo 'hello...'; sleep 2; echo 'goodbye'" },
        {
            .output = process::PIPE,
            .error = process::PIPE,
        }
    );

    tie(out, err) = proc.read_output(1s);
    ASSERT_EQ(out, "hello...\n");
    ASSERT_EQ(err, "");
    
    tie(out, err) = proc.read_output(3s);
    ASSERT_EQ(out, "goodbye\n");
    ASSERT_EQ(err, "");

    proc.wait();

    auto result = proc.result();
    EXPECT_EQ(result.code, 0);
    EXPECT_EQ(result.output, "");
    EXPECT_EQ(result.error, "");
}

TEST(Process, Redirect)
{
    string out, err;

    process proc(
        { "/bin/sh", "-c", "echo output; >&2 echo error" },
        {
            .output = process::PIPE,
            .error = process::STDOUT,
        }
    );

    auto result = proc.wait();
    EXPECT_EQ(result.code, 0);
    EXPECT_EQ(result.output, "output\nerror\n");
    EXPECT_EQ(result.error, "");
}

TEST(Process, FileOutput)
{
    auto log = file::create_temporary("libslw-", "wt+");
    process proc(
        { "/bin/echo", "testing" },
        {
            .output = log.fd(),
        }
    );

    auto result = proc.wait();
    EXPECT_EQ(result.code, 0);
    EXPECT_EQ(result.output, "");
    EXPECT_EQ(result.error, "");

    log.rewind();
    EXPECT_EQ(log.read_string(), "testing\n");
}

TEST(Process, FileInput)
{
    auto input = file::create_temporary("libslw-", "wt+");
    input.write_lines({
        "hello",
        "world"
    });

    input.rewind();

    process proc(
        { "/bin/cat", "-" },
        {
            .input = input.fd(),
            .output = process::PIPE,
            .error = process::PIPE,
        }
    );

    auto result = proc.wait();
    EXPECT_EQ(result.code, 0);
    EXPECT_EQ(result.output, "hello\nworld\n");
    EXPECT_EQ(result.error, "");
}

int main(int argc, char * argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
