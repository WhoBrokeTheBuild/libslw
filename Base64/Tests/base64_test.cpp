
#include <slw/base64.hpp>

#include <gtest/gtest.h>

using namespace slw;

TEST(Base64Test, Lipsum)
{
    const char * DATA = 
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Mauris porta "
        "dignissim mauris. Integer viverra faucibus dolor. Cras congue metus orci, "
        "quis ullamcorper erat imperdiet ac. Curabitur sodales leo ut diam rhoncus, "
        "id mattis sapien consectetur.";

    const char * CODE = 
        "TG9yZW0gaXBzdW0gZG9sb3Igc2l0IGFtZXQsIGNvbnNlY3RldHVyIGFkaXBpc2NpbmcgZWxpdC4g"
        "TWF1cmlzIHBvcnRhIGRpZ25pc3NpbSBtYXVyaXMuIEludGVnZXIgdml2ZXJyYSBmYXVjaWJ1cyBk"
        "b2xvci4gQ3JhcyBjb25ndWUgbWV0dXMgb3JjaSwgcXVpcyB1bGxhbWNvcnBlciBlcmF0IGltcGVy"
        "ZGlldCBhYy4gQ3VyYWJpdHVyIHNvZGFsZXMgbGVvIHV0IGRpYW0gcmhvbmN1cywgaWQgbWF0dGlz"
        "IHNhcGllbiBjb25zZWN0ZXR1ci4=";

    const auto& code = base64::encode_string(DATA);

    ASSERT_EQ(code, CODE);

    const auto& lipsum = base64::decode_string(CODE);

    ASSERT_EQ(lipsum, DATA);
}

TEST(Base64Test, Zeros)
{
    const uint8_t DATA[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    const char * CODE = "AAAAAAAAAAAAAA==";

    const auto& result = base64::encode(DATA);

    ASSERT_EQ(result, CODE);
}

TEST(Base64Test, AllValues)
{
    const char * CODE = 
        "AAECAwQFBgcICQoLDA0ODxAREhMUFRYXGBkaGxwdHh8gISIjJCUmJygpKissLS4vMDEyMzQ1Njc4"
        "OTo7PD0+P0BBQkNERUZHSElKS0xNTk9QUVJTVFVWV1hZWltcXV5fYGFiY2RlZmdoaWprbG1ub3Bx"
        "cnN0dXZ3eHl6e3x9fn+AgYKDhIWGh4iJiouMjY6PkJGSk5SVlpeYmZqbnJ2en6ChoqOkpaanqKmq"
        "q6ytrq+wsbKztLW2t7i5uru8vb6/wMHCw8TFxsfIycrLzM3Oz9DR0tPU1dbX2Nna29zd3t/g4eLj"
        "5OXm5+jp6uvs7e7v8PHy8/T19vf4+fr7/P3+/w==";

    std::vector<uint8_t> data; // { 0 .. 255 }
    for (int i = 0; i <= UINT8_MAX; ++i) {
        data.push_back(i);
    }

    auto code = base64::encode(data);

    const auto& result = base64::decode(code);

    ASSERT_EQ(data.size(), result.size());
    
    ASSERT_TRUE(std::equal(data.begin(), data.end(), result.begin()));
}

TEST(Base64Test, Random)
{
    srand(time(0));

    size_t length = rand() % 1000;

    std::vector<uint8_t> data;
    for (int i = 0; i < length; ++i) {
        data.push_back(rand() % 256);
    }

    auto code = base64::encode(data);

    const auto& result = base64::decode(code);

    ASSERT_EQ(data.size(), result.size());
    
    ASSERT_TRUE(std::equal(data.begin(), data.end(), result.begin()));
}

int main(int argc, char * argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}