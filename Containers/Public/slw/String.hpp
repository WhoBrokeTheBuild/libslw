#ifndef SLW_BASE64_HPP
#define SLW_BASE64_HPP

#include <string>
#include <string_view>
#include <vector>

namespace slw {

namespace String {

inline bool Contains(std::string_view haystack, std::string_view needle) {
    return (haystack.find(needle) != std::string_view::npos);
}

inline bool Contains(std::string_view haystack, char needle) {
    return (haystack.find(needle) != std::string_view::npos);
}

std::vector<std::string_view> Split(std::string_view string, std::string_view separator);

inline std::vector<std::string_view> Split(std::string_view string, char separator) {
    return Split(string, { &separator, 1 });
}

std::string Join(const std::vector<std::string_view>& parts, std::string_view separator);

inline std::string Join(std::initializer_list<const char *> parts, std::string_view separator) {
    return Join(std::vector<std::string_view>(parts.begin(), parts.end()), separator);
}

inline std::string Join(std::initializer_list<const char *> parts, char separator) {
    return Join(std::vector<std::string_view>(parts.begin(), parts.end()), { &separator, 1 });
}

inline std::string Join(const std::vector<std::string_view>& parts, char separator) {
    return Join(parts, { &separator, 1 });
}

inline std::string Join(const std::vector<std::string>& parts, std::string_view separator) {
    std::vector<std::string_view> views = { parts.begin(), parts.end() };
    return Join(views, separator);
}

inline std::string Join(const std::vector<std::string>& parts, char separator) {
    std::vector<std::string_view> views = { parts.begin(), parts.end() };
    return Join(views, { &separator, 1 });
}

} // namespace String

} // namespace slw

#endif // SLW_BASE64_HPP