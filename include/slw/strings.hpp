#ifndef SLW_STRINGS_HPP
#define SLW_STRINGS_HPP

#include <slw/containers.hpp>
#include <slw/constraints.hpp>

namespace slw {

template <typename CharT, typename Traits = std::char_traits<CharT>>
struct basic_string_views
{
    typedef CharT value_t;
    typedef basic_string<CharT, Traits> string_t;
    typedef basic_string_view<CharT, Traits> string_view_t;


    static inline constexpr string_t whitespace() {
        return { ' ', '\n', '\r', '\f', '\v' };
    }


    static inline constexpr list<string_view_t> split(string_view_t str, string_view_t delim)
    {
        auto result = views::split(str, delim);
        return { std::begin(result), std::end(result) };
    }

    static inline constexpr list<string_view_t> split(string_view_t str, value_t delim) {
        return split(str, string_view_t(&delim, 1));
    }


    template <ranges::viewable_range R>
        requires is_string_view_v<ranges::range_value_t<R>>
    static inline constexpr string_t join(R&& range, string_view_t delim) {
        auto result = views::join_with(range, delim);
        return string_t(std::begin(result), std::end(result));
    }

    template <ranges::viewable_range R>
    static inline constexpr string_t join(R&& range, value_t delim) {
        return join(std::move(range), string_view_t(&delim, 1));
    }

    template <typename T>
        requires concepts::characters<T>
    static inline constexpr string_t join(initializer_list<T> values, auto delim) {
        return join(list<string_view_t>(std::begin(values), std::end(values)), delim);
    }


    static inline constexpr string_view_t trim_left(string_view_t str, string_view_t filter = whitespace()) {
        return str.substr(str.find_first_not_of(filter));
    }


    static inline constexpr string_view_t trim_right(string_view_t str, string_view_t filter = whitespace()) {
        auto it = str.find_last_not_of(filter);
        if (it == string_view::npos) {
            return str;
        }
        return str.substr(0, it + 1);
    }


    static inline constexpr string_view_t trim(string_view_t str, string_view_t filter = whitespace()) {
        return trim_left(trim_right(str, filter), filter);
    }

}; // basic_string_views

typedef basic_string_views<char> string_views;

template <typename CharT, typename Traits = std::char_traits<CharT>>
struct basic_strings
{
    typedef CharT value_t;
    typedef basic_string<CharT, Traits> string_t;
    typedef basic_string_view<CharT, Traits> string_view_t;
    typedef basic_string_views<CharT, Traits> string_views_t;


    static inline constexpr string_t whitespace() {
        return string_views_t::whitespace();
    }
    

    static inline constexpr list<string_t> split(string_view_t str, string_view_t delim)
    {
        const auto& views = string_views_t::split(str, delim);
        return list<string_t>(std::begin(views), std::end(views));
    }

    static inline constexpr list<string_t> split(string_view_t str, value_t delim) {
        return split(str, string_view_t(&delim, 1));
    }


    template <ranges::viewable_range R>
        requires is_string_v<ranges::range_value_t<R>>
    static inline constexpr string_t join(R&& range, string_view_t delim) {
        auto result = views::join_with(range, delim);
        return string_t(std::begin(result), std::end(result));
    }

    template <ranges::viewable_range R>
    static inline constexpr string_t join(R&& range, value_t delim) {
        return join(std::move(range), string_view_t(&delim, 1));
    }

    template <typename T>
        requires concepts::characters<T>
    static inline constexpr string_t join(initializer_list<T> values, auto delim) {
        return join(list<string_t>(std::begin(values), std::end(values)), delim);
    }


    static inline constexpr string_t trim_left(string_view_t str, string_view_t filter = whitespace()) {
        return string_views_t::trim_left(str, filter);
    }


    static inline constexpr string_t trim_right(string_view_t str, string_view_t filter = whitespace()) {
        return string_views_t::trim_right(str, filter);
    }


    static inline constexpr string_t trim(string_view_t str, string_view_t filter = whitespace()) {
        return string_views_t::trim(str, filter);
    }

}; // basic_strings

typedef basic_strings<char> strings;

// } // namespace strings

} // namespace slw

#endif // SLW_STRINGS_HPP