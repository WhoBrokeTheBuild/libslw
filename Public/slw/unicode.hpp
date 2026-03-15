#ifndef SLW_UNICODE_HPP
#define SLW_UNICODE_HPP

#include <slw/strings.hpp>
#include <slw/exceptions.hpp>

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

    class invalid_sequence : public runtime_error
    {
    public:

        invalid_sequence()
            : runtime_error("Invalid UTF-8 byte sequence")
        { }

    }; // class invalid_sequence

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

    //     using wide_string = std::wstring;

    //     using wide_string_view = std::wstring_view;

    //     wide_string to_wide_string(u8string_view string);

    //     u8string from_wide_string(wide_string_view string);

    // #endif

} // namespace utf8

} // namespace slw

#endif // SLW_UTF8_HPP