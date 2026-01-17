#ifndef SLW_LIST_HPP
#define SLW_LIST_HPP

#include <vector>
#include <algorithm>

namespace slw {

namespace List {

template <typename T>
inline bool Contains(const std::vector<T>& haystack, const T& needle) {
    return (std::find(haystack.begin(), haystack.end(), needle) != haystack.end());
}

template <typename T>
inline bool Equals(const std::vector<T>& left, const std::vector<T>& right) {
    return std::equal(left.begin(), left.end(), right.begin());
}

} // namespace List

} // namespace slw

#endif // SLW_LIST_HPP