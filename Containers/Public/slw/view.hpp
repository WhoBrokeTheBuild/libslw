#ifndef SLW_VIEW_HPP
#define SLW_VIEW_HPP

#include <slw/ranges.hpp>

#include <span>

namespace slw {

template <typename Type, size_t Extent = std::dynamic_extent>
using view = std::span<Type, Extent>;

} // namespace slw

#endif // SLW_VIEW_HPP