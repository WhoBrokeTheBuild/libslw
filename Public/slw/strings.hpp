#ifndef SLW_STRINGS_HPP
#define SLW_STRINGS_HPP

#include <slw/list.hpp>
#include <slw/view.hpp>
#include <slw/string_like.hpp>
#include <slw/string_views.hpp>

#include <algorithm>

namespace slw {

using std::string;

using std::string_view;

using namespace std::string_literals;

namespace strings {

using string_views::WHITESPACE;

using string_views::delimeter;

using string_views::length;

inline constexpr list<string> split(string_view str, delimeter auto delim)
{
    list<string> parts;
    for (const auto& part : string_views::split(str, delim)) {
        parts.emplace_back(part);
    }
    return parts;
}

inline constexpr string join(view<string> parts, delimeter auto delim)
{
    list<string_view> temp(parts.begin(), parts.end());
    return string_views::join(temp, delim);
}

template <ranges::input_range R>
inline constexpr string join(R&& range, delimeter auto delim) {
    return join(view<string>(range), delim);
}

template <typename T>
    requires is_string_like_v<T>
inline constexpr string join(std::initializer_list<T> parts, delimeter auto delim)
{
    list<string> temp(parts.begin(), parts.end());
    return join(temp, delim);
}

inline constexpr string trim_left(string_view str, string_view filter = WHITESPACE) {
    return string(string_views::trim_left(str, filter));
}

inline constexpr string trim_right(string_view str, string_view filter = WHITESPACE) {
    return string(string_views::trim_right(str, filter));
}

inline constexpr string trim(string_view str, string_view filter = WHITESPACE) {
    return string(string_views::trim(str, filter));
}

using string_views::quoted;

} // namespace strings

} // namespace slw

#endif // SLW_BASE64_HPP