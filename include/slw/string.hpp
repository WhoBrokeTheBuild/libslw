#ifndef SLW_STRING_HPP
#define SLW_STRING_HPP

#include <slw/constraint.hpp>
#include <slw/initializer_list.hpp>
#include <slw/list.hpp>
#include <slw/range.hpp>

#include <string>
#include <type_traits>

namespace slw {

using std::string;

using std::basic_string;

using std::string_view;

using std::basic_string_view;


using namespace std::string_literals;

using namespace std::string_view_literals;


template <typename T>
struct is_string : std::false_type { };

template <typename CharT, typename Traits, typename Alloc>
struct is_string<std::basic_string<CharT, Traits, Alloc>> : std::true_type { };


template <typename T>
struct is_string_view : std::false_type { };

template <typename CharT, typename Traits>
struct is_string_view<std::basic_string_view<CharT, Traits>> : std::true_type { };


template <typename T>
struct is_cstring : std::false_type { };

template<>
struct is_cstring<char *> : std::true_type { };

template<>
struct is_cstring<const char *> : std::true_type { };

template<size_t N>
struct is_cstring<char[N]> : std::true_type { };

template<size_t N>
struct is_cstring<const char[N]> : std::true_type { };


template <typename T>
constexpr bool is_string_v = is_string<T>::value;

template <typename T>
constexpr bool is_string_view_v = is_string_view<T>::value;

template <typename T>
constexpr bool is_cstring_v = is_cstring<T>::value;


namespace concepts {

    template <typename T>
    concept string = is_string_v<T>;

    template <typename T>
    concept string_view = is_string_view_v<T>;

    template <typename T>
    concept cstring = is_cstring_v<T>;

    template <typename T>
    concept characters = is_string_v<T> or is_string_view_v<T> or is_cstring_v<T>;

} // namespace concepts


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


static_assert(is_string_v<std::string>);
static_assert(not is_string_v<std::string_view>);

static_assert(is_string_view_v<std::string_view>);
static_assert(not is_string_view_v<std::string>);

static_assert(is_cstring_v<char *>);
static_assert(is_cstring_v<const char *>);
static_assert(is_cstring_v<char [1]>);
static_assert(is_cstring_v<const char [1]>);
static_assert(not is_cstring_v<std::string>);
static_assert(not is_cstring_v<std::string_view>);

// } // namespace strings

} // namespace slw

#endif // SLW_STRING_HPP