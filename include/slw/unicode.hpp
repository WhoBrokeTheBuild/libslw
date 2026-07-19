#ifndef SLW_UNICODE_HPP
#define SLW_UNICODE_HPP

#include <slw/exception.hpp>
#include <slw/string.hpp>

namespace slw {

using std::u8string;

typedef basic_strings<char8_t> u8strings;

using std::u8string_view;

typedef basic_string_views<char8_t> u8string_views;


using std::u32string;

typedef basic_strings<char32_t> u32strings;

using std::u32string_view;

typedef basic_string_views<char32_t> u32string_views;


namespace unicode {

    const char32_t Missing = U'�';

    class invalid_sequence : public runtime_error
    {
    public:

        invalid_sequence()
            : runtime_error("Invalid UTF-8 byte sequence")
        { }

    }; // class invalid_sequence

    // constexpr bool is_valid(u8string_view string);

    inline constexpr size_t length(u32string_view string) {
        return string.size();
    }

    inline constexpr size_t length(u8string_view string)
    {
        size_t length = 0;
        for (size_t i = 0; i < string.size(); ) {
            auto c = string[i];
            if ((c & 0b10000000) == 0b00000000) {
                ++i;
            }
            else if ((c & 0b11100000) == 0b11000000) {
                i += 2;
            }
            else if ((c & 0b11110000) == 0b11100000) {
                i += 3;
            }
            else if ((c & 0b11111000) == 0b11110000) {
                i += 4;
            }
            else {
                throw invalid_sequence();
            }
            
            ++length;
        }
        return length;
    }

    inline constexpr u32string to_utf32(u8string_view string)
    {
        u32string result;

        for (size_t i = 0; i < string.size(); ) {
            auto c = string[i];
            if ((c & 0b10000000) == 0b00000000) {
                result.push_back(char32_t(c));
                ++i;
            }
            else if ((c & 0b11100000) == 0b11000000) {
                char32_t code = (
                    ((string[i + 0] & 0b00011111) << 6) |
                    (string[i + 1] & 0b00111111)
                );
                result.push_back(code);
                i += 2;
            }
            else if ((c & 0b11110000) == 0b11100000) {
                char32_t code = (
                    ((string[i + 0] & 0b00001111) << 12) |
                    ((string[i + 1] & 0b00111111) << 6) |
                    (string[i + 2] & 0b00111111)
                );
                result.push_back(code);
                i += 3;
            }
            else if ((c & 0b11111000) == 0b11110000) {
                char32_t code = (
                    ((c & 0b00000111) << 18) |
                    ((string[i + 1] & 0b00111111) << 12) |
                    ((string[i + 2] & 0b00111111) << 6) |
                    (string[i + 3] & 0b00111111)
                );
                result.push_back(code);
                i += 4;
            }
            else {
                throw invalid_sequence();
            }
        }

        return result;
    }

    inline constexpr u8string to_utf8(u32string_view string)
    {
        u8string result;

        for (size_t i = 0; i < string.size(); ++i) {
            char32_t code = string[i];
            if (code <= 0x7F) {
                result.push_back(char(code));
            }
            else if (code <= 0x7FF) {
                result.push_back(0b11000000 | ((code >> 6) & 0x3F));
                result.push_back(0b10000000 | (code & 0x3F));
            }
            else if (code <= 0xFFFF) {
                result.push_back(0b11100000 | ((code >> 12) & 0x3F));
                result.push_back(0b10000000 | ((code >> 6) & 0x3F));
                result.push_back(0b10000000 | (code & 0x3F));
            }
            else if (code <= 0x10FFFF) {
                result.push_back(0b11110000 | ((code >> 18) & 0x3F));
                result.push_back(0b10000000 | ((code >> 12) & 0x3F));
                result.push_back(0b10000000 | ((code >> 6) & 0x3F));
                result.push_back(0b10000000 | (code & 0x3F));
            }
            else {
                throw invalid_sequence();
            }
        }

        return result;
    }

    inline constexpr char32_t case_fold(char32_t code)
    {
        struct CaseFold {
            char32_t code;
            char32_t mapping;
            // char32_t mapping[3];
        };
        
        CaseFold Mappings[] = {
        #include "unicode.inc.hpp"
        };

        for (size_t i = 0; i < sizeof(Mappings) / sizeof(Mappings[0]); ++i) {
            if (Mappings[i].code == code) {
                return Mappings[i].mapping;
            }
        }

        return code;
    }

    inline constexpr u32string case_fold(u32string_view string)
    {
        u32string result(string);
        for (auto& code : result) {
            code = case_fold(code);
        }
        return result;
    }

    inline constexpr u8string case_fold(u8string_view string)
    {
        return to_utf8(case_fold(to_utf32(string)));
    }

    // #if defined(WIN32)

    //     using wide_string = std::wstring;

    //     using wide_string_view = std::wstring_view;

    //     wide_string to_wide_string(u8string_view string);

    //     u8string from_wide_string(wide_string_view string);

    // #endif


} // namespace unicode

} // namespace slw

#endif // SLW_UTF8_HPP