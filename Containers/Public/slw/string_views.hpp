#ifndef SLW_STRING_VIEWS_HPP
#define SLW_STRING_VIEWS_HPP

#include <slw/list.hpp>
#include <slw/view.hpp>
#include <slw/string_like.hpp>

#include <algorithm>

namespace slw {

using std::string;

using std::string_view;

using namespace std::string_view_literals;

namespace string_views {

// TODO: Improve
constexpr string_view WHITESPACE = " \t\n\r\f\v";

// TODO: Improve
template <typename T>
concept delimeter = is_string_like_v<T> || std::is_same_v<char, T>;

inline constexpr size_t length(char) {
    return 1;
}

inline constexpr size_t length(string_view str) {
    return std::size(str);
}

inline constexpr list<string_view> split(string_view str, delimeter auto delim)
{
    list<string_view> parts;

    size_t delimSize = length(delim);

    auto it = str.find(delim);
    while (it != string_view::npos) {
        parts.emplace_back(str.substr(0, it));
        str = str.substr(it + delimSize);
        it = str.find(delim);
    }

    parts.emplace_back(str);

    return parts;
}

inline constexpr string join(view<string_view> parts, delimeter auto delim)
{
    if (parts.empty()) {
        return {};
    }

    size_t size = (parts.size() - 1) * length(delim);
    for (const auto& part : parts) {
        size += length(part);
    }

    string result;
    result.reserve(size);

    for (size_t i = 0; i < parts.size() - 1; ++i) {
        result.append(parts[i]);
        result += delim;
    }

    result.append(parts.back());

    return result;
}

template <ranges::input_range R>
inline constexpr string join(R&& range, delimeter auto delim) {
    return join(view<string_view>(range), delim);
}

template <typename T>
    requires is_string_like_v<T>
inline constexpr string join(std::initializer_list<T> parts, delimeter auto delim) {
    return join(list<string_view>(parts.begin(), parts.end()), delim);
}

inline constexpr string_view trim_left(string_view str, string_view filter = WHITESPACE) {
    return str.substr(str.find_first_not_of(filter));
}

inline constexpr string_view trim_right(string_view str, string_view filter = WHITESPACE) {
    auto it = str.find_last_not_of(filter);
    if (it == string_view::npos) {
        return str;
    }
    return str.substr(0, it + 1);
}

inline constexpr string_view trim(string_view str, string_view filter = WHITESPACE) {
    return trim_left(trim_right(str), filter);
}

inline constexpr string quoted(char c)
{
    if (c == '\'') {
        return { '\'', '\\', c, '\'' };
    }
    return { '\'', c, '\''};
}

inline constexpr string quoted(string_view str)
{
    size_t quotes = std::count(std::begin(str), std::end(str), '"');

    string result;
    result.reserve(str.size() + quotes + 2);
    
    result += '"';

    if (quotes == 0) {
        result += str;
    }
    else {
        for (const auto& c : str) {
            if (c == '"') {
                result += '\\';
            }
            result += c;
        }
    }

    result += '"';

    return result;
}

} // namespace string_views

} // namespace slw

#endif // SLW_BASE64_HPP