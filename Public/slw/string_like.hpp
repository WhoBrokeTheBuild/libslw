#ifndef SLW_STRING_LIKE_HPP
#define SLW_STRING_LIKE_HPP

#include <string>

namespace slw {

template <typename T>
struct is_string_like {
    static constexpr bool value = false;
};

template<>
struct is_string_like<std::string> {
    static constexpr bool value = true;
};

template<>
struct is_string_like<std::string_view> {
    static constexpr bool value = true;
};

template<>
struct is_string_like<char *> {
    static constexpr bool value = true;
};

template<>
struct is_string_like<const char *> {
    static constexpr bool value = true;
};

template<size_t N>
struct is_string_like<char[N]> {
    static constexpr bool value = true;
};

template<size_t N>
struct is_string_like<const char[N]> {
    static constexpr bool value = true;
};

template <typename T>
constexpr bool is_string_like_v = is_string_like<T>::value;

template <typename T>
concept string_like = is_string_like_v<T>;

} // namespace slw

#endif // SLW_STRING_LIKE_HPP