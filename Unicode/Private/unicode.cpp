#include <slw/unicode.hpp>
#include <cstdio>

namespace slw {
    // const char32_t MISSING = U'�';

size_t unicode::length(u8string_view string)
{
    size_t length = 0;
    for (size_t i = 0; i < string.size(); ) {
        auto c = string[i];
        if ((c & 0b10000000) == 0b00000000) { //expect 1 byte
            ++i;
        }
        else if ((c & 0b11100000) == 0b11000000) { //expect 2 bytes
            i += 2;
        }
        else if ((c & 0b11110000) == 0b11100000) { //expect 3 bytes
            i += 3;
        }
        else if ((c & 0b11111000) == 0b11110000) { //expect 4 bytes
            i += 4;
        }
        else {
            throw invalid_sequence();
        }
        
        ++length;
    }
    return length;
}

} // namespace slw