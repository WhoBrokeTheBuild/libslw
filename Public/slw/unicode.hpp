#ifndef SLW_UNICODE_HPP
#define SLW_UNICODE_HPP

#include <string_view>
#include <stdexcept>

namespace slw {

namespace unicode {

class invalid_sequence : public std::runtime_error
{
public:

    invalid_sequence()
        : std::runtime_error("Invalid UTF-8 byte sequence")
    { }

}; // class InvalidSequence

using std::u8string;

using std::u8string_view;

using std::u32string;

using std::u32string_view;

// constexpr bool is_valid(u8string_view string);

size_t length(u8string_view string);

inline constexpr size_t length(u32string_view string) {
    return string.size();
}

// constexpr u8string case_fold(u8string_view string);

// constexpr u32string case_fold(u32string_view string);

// constexpr u32string to_utf32(u8string_view string);

// constexpr u8string to_utf8(u32string_view string);

// #if defined(WIN32)

// using wide_string = std::wstring;

// using wide_string_view = std::wstring_view;

// wide_string to_wide_string(u8string_view string);

// u8string from_wide_string(wide_string_view string);

// #endif

} // namespace utf8

} // namespace slw

#endif // SLW_UTF8_HPP