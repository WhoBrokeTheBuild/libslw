#ifndef SLW_STRINGS_HPP
#define SLW_STRINGS_HPP

#include <slw/containers/list.hpp>
#include <slw/containers/string.hpp>
#include <slw/containers/view.hpp>

namespace slw {

using std::string;

using std::string_view;

using namespace std::string_literals;

namespace string_views {

    // TODO: Improve
    constexpr string_view WHITESPACE = " \t\n\r\f\v";

    // TODO: Improve
    template <typename T>
    concept delimeter = is_string_v<T> or is_cstring_v<T> or std::is_same_v<char, T>;

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
        requires is_string_v<T> or is_cstring_v<T>
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

} // namespace string_views

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
        requires is_string_v<T> or is_cstring_v<T>
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

} // namespace strings

} // namespace slw

#endif // SLW_STRINGS_HPP