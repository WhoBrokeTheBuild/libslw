#include <slw/base64.hpp>

#include <cmath>
#include <cassert>

namespace slw {

constexpr char _ENCODE_MAP[64] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/',
};

constexpr uint8_t _DECODE_MAP[] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3E, 0xFF, 0xFF, 0xFF, 0x3F, // + /
    0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0xFF, 0xFF, 0xFF, 0xEE, 0xFF, 0xFF, // 0-9 =
    0xFF, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, // A-O
    0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // P-Z
    0xFF, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, // a-o
    0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // p-z
};

std::vector<uint8_t> base64::decode(std::string_view code)
{
    assert((code.size() % 4) == 0);

    size_t result_size = (code.size() * 0.75);

    std::vector<uint8_t> result;
    result.reserve(result_size);

    size_t index;
    for (index = 0; (index + 3) < code.size(); index += 4) {
        uint8_t a = _DECODE_MAP[code[index + 0]];
        uint8_t b = _DECODE_MAP[code[index + 1]];
        uint8_t c = _DECODE_MAP[code[index + 2]];
        uint8_t d = _DECODE_MAP[code[index + 3]];

        assert(a != 0xFF && b != 0xFF && c != 0xFF && d != 0xFF);

        unsigned work = (a << 18) | (b << 12) | (c <<  6) | d;

        if (c == 0xEE && d == 0xEE) {
            result.push_back((work & (0xFF << 16)) >> 16);
        }
        else if (d == 0xEE) {
            result.push_back((work & (0xFF << 16)) >> 16);
            result.push_back((work & (0xFF <<  8)) >>  8);
        }
        else {
            result.push_back((work & (0xFF << 16)) >> 16);
            result.push_back((work & (0xFF <<  8)) >>  8);
            result.push_back((work & (0xFF <<  0)) >>  0);
        }
    }

    return result;
}

std::string base64::encode(std::span<const uint8_t> data)
{
    size_t result_size = std::ceil((data.size() * 0.75) / 4.0) * 4;

    std::string result;
    result.reserve(result_size);

    size_t index;
    for (index = 0; (index + 2) < data.size(); index += 3) 
    {
        // AAAAAA AABBBB BBBBCC CCCCCC
        unsigned work = (data[index] << 16) | (data[index + 1] << 8) | data[index + 2];

        result.push_back(_ENCODE_MAP[(work & (0b111111 << 18)) >> 18]);
        result.push_back(_ENCODE_MAP[(work & (0b111111 << 12)) >> 12]);
        result.push_back(_ENCODE_MAP[(work & (0b111111 <<  6)) >>  6]);
        result.push_back(_ENCODE_MAP[(work & (0b111111 <<  0)) >>  0]);
    }

    int remaining = data.size() - index;
    if (remaining == 1) {
        // AAAAAA AA0000
        unsigned work = (data[index] << 4);

        result.push_back(_ENCODE_MAP[(work & (0b111111 <<  6)) >>  6]);
        result.push_back(_ENCODE_MAP[(work & (0b111111 <<  0)) >>  0]);
        result.push_back('=');
        result.push_back('=');
    }
    else if (remaining == 2) {
        // AAAAAA AABBBB BBBB00
        unsigned work = (data[index] << 10) | (data[index + 1] << 2);

        result.push_back(_ENCODE_MAP[(work & (0b111111 << 12)) >> 12]);
        result.push_back(_ENCODE_MAP[(work & (0b111111 <<  6)) >>  6]);
        result.push_back(_ENCODE_MAP[(work & (0b111111 <<  0)) >>  0]);
        result.push_back('=');
    }

    assert((result.size() % 4) == 0);

    return result;
}

} // namespace slw
