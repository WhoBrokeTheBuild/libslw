
#include <slw/file.hpp>

#include <gtest/gtest.h>

using namespace slw;

const char INPUT_TXT[] = {
#embed "input.txt"
};

const uint8_t IMAGE_BMP[] = {
#embed "image.bmp"
};

class ReadTest : public testing::Test
{
protected:

    void SetUp() override {
        // SetWorkingDirectory(path(__FILE__).parent_path());
        set_working_directory(path(__FILE__).parent_path());
    }
};

TEST_F(ReadTest, Text)
{
    const auto& data = file("input.txt").read<char>();
    ASSERT_EQ(string_view(INPUT_TXT), data.data());

    const auto& text = file("input.txt").read_string();
    ASSERT_EQ(text, string_view(INPUT_TXT));

    const auto& lines1 = string_views::split(text, '\n');
    const auto& lines2 = string_views::split(INPUT_TXT, '\n');

    for (const auto& [line1, line2] : views::zip(lines1, lines2)) {
        ASSERT_EQ(line1, line2);
    }
}

TEST_F(ReadTest, Binary)
{
    file image("image.bmp");

    uint8_t magic[2];

    image.rewind();
    ASSERT_EQ(image.read_one(), 0x42);
    ASSERT_EQ(image.read_one(), 0x4D);

    image.rewind();
    image.read_into(magic);
    ASSERT_TRUE(ranges::equal(magic, list<uint8_t>{ 0x42, 0x4D }));

    image.rewind();
    image.read_into(magic, sizeof(magic));
    ASSERT_TRUE(ranges::equal(magic, list<uint8_t>{ 0x42, 0x4D }));

    uint16_t size = image.read_one<uint16_t>();
    ASSERT_EQ(size, image.get_size());

    image.seek(14, seekdir::begin);

    struct BITMAPINFOHEADER {
        uint32_t size;
        uint32_t width;
        uint32_t height;
        // ...
    } __attribute__((__packed__));

    BITMAPINFOHEADER header;
    image.read_object(header);
    ASSERT_EQ(header.width, 16);
    ASSERT_EQ(header.height, 16);

    image.rewind();

    const auto& data = image.read();
    ASSERT_EQ(data.size(), sizeof(IMAGE_BMP));
    ASSERT_EQ(memcmp(IMAGE_BMP, data.data(), data.size()), 0);

    ASSERT_THROW(image.read(99999), std::system_error);
    ASSERT_THROW(file("doesnotexist"), std::system_error);
}

class WriteTest : public testing::Test
{
protected:

    path _temp_dir;
    path _temp_file;

    void SetUp() override {
        _temp_dir = make_temporary_directory("/tmp/libslw-");
    }

    void TearDown() override {
        remove_file(_temp_file);
        remove_directory(_temp_dir);
    }
};

TEST_F(WriteTest, Text)
{
    _temp_file = _temp_dir / "output.txt";

    file file(_temp_file, "wt");
    file.print("Test 123\n");
    file.println("{} {}", "Test", 123);

    const string_view view = "Test 123\n";
    file.write_from(view);

    const char array[] = { 'T', 'e', 's', 't', ' ', '1', '2', '3', '\n' };
    file.write_object(array);

    file.close();

    file.open(_temp_file, "rt");
    const auto& lines = file.read_lines();
    for (const auto& line : lines) {
        EXPECT_EQ(string_views::trim_right(line), "Test 123");
    }
    file.close();

    file.open(_temp_file, "wt");
    for (const auto& line : lines) {
        file.write_line(string_views::trim_right(line));
    }
    file.close();

    file.open(_temp_file, "rt");
    const auto& lines2 = file.read_lines();
    ASSERT_TRUE(ranges::equal(lines, lines2));
    file.close();

    file.open(_temp_file, "wt");
    file.write_lines({
        "Test 123", "Test 123", "Test 123",
    });
}

TEST_F(WriteTest, Binary)
{
    _temp_file = _temp_dir / "output.bin";

    file output(_temp_file, "wb");

    output.write_one(42);

    uint8_t data[100];
    output.write_from(data, sizeof(data));
    output.write_from(data);

    struct point {
        int x;
        int y;
    };

    output.write_object(point{ .x = 10, .y = 100 });
}

int main(int argc, char * argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}