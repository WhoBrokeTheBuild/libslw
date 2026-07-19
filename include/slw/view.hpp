#ifndef SLW_VIEW_HPP
#define SLW_VIEW_HPP

#include <slw/range.hpp>

#include <span>

namespace slw {

template <typename Type, size_t Extent = std::dynamic_extent>
using view = std::span<Type, Extent>;

using std::dynamic_extent;


template <typename T>
struct is_view : std::false_type { };

template <typename Type, size_t Extent>
struct is_view<view<Type, Extent>> : std::true_type { };


template <typename T>
constexpr bool is_view_v = is_view<T>::value;


namespace concepts {

    template <typename T>
    concept view = is_view_v<T>;

} // namespace concepts

} // namespace slw

#endif // SLW_VIEW_HPP