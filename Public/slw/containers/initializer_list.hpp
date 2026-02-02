#ifndef SLW_INITIALIZER_LIST_HPP
#define SLW_INITIALIZER_LIST_HPP

#include <initializer_list>
#include <type_traits>

namespace slw {

using std::initializer_list;


template <typename T>
struct is_initializer_list : std::false_type { };

template <typename Type>
struct is_initializer_list<initializer_list<Type>> : std::true_type { };


template <typename T>
constexpr bool is_initializer_list_v = is_initializer_list<T>::value;


namespace concepts {

    template <typename T>
    concept initializer_list = is_initializer_list_v<T>;

} // namespace concepts

} // namespace slw

#endif // SLW_INITIALIZER_LIST_HPP
