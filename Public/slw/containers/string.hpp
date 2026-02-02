#ifndef SLW_STRING_HPP
#define SLW_STRING_HPP

#include <string>
#include <type_traits>

namespace slw {

using std::string;

using std::string_view;

using namespace std::string_literals;

using namespace std::string_view_literals;


template <typename T>
struct is_string : std::false_type { };

template <typename CharT, typename Traits, typename Alloc>
struct is_string<std::basic_string<CharT, Traits, Alloc>> : std::true_type { };

template <typename T>
constexpr bool is_string_v = is_string<T>::value;


template <typename T>
struct is_string_view : std::false_type { };

template <typename CharT, typename Traits>
struct is_string_view<std::basic_string_view<CharT, Traits>> : std::true_type { };

template <typename T>
constexpr bool is_string_view_v = is_string_view<T>::value;


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

} // namespace slw

#endif // SLW_STRING_HPP