#ifndef SLW_LIST_HPP
#define SLW_LIST_HPP

#include <slw/format.hpp>

#include <slw/containers/view.hpp>

#include <vector>

namespace slw {

template <typename Type,
    typename Alloc = std::allocator<Type>>
using list = std::vector<Type, Alloc>;


template <typename T>
struct is_list : std::false_type { };

template <typename Type, typename Alloc>
struct is_list<list<Type, Alloc>> : std::true_type { };


template <typename T>
constexpr bool is_list_v = is_list<T>::value;


namespace concepts {

    template <typename T>
    concept list = is_list_v<T>;

} // namespace concepts

} // namespace slw

#endif // SLW_LIST_HPP