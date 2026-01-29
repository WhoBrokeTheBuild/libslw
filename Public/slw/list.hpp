#ifndef SLW_LIST_HPP
#define SLW_LIST_HPP

#include <slw/view.hpp>

#include <vector>

namespace slw {

template <typename Type,
    typename Alloc = std::allocator<Type>>
using list = std::vector<Type, Alloc>;

} // namespace slw

#endif // SLW_LIST_HPP