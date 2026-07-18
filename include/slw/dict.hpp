#ifndef SLW_DICT_HPP
#define SLW_DICT_HPP

#include <slw/format.hpp>

#include <map>
#include <type_traits>
#include <unordered_map>

namespace slw {

using std::pair;

template <typename Key, typename Value,
    typename Hash = std::hash<Key>,
    typename Predicate = std::equal_to<Key>,
    typename Alloc = std::allocator<std::pair<const Key, Value>>>
using dict = std::unordered_map<Key, Value, Hash, Predicate, Alloc>;


template <typename Key, typename Value,
    typename Compare = std::less<Key>,
    typename Alloc = std::allocator<std::pair<const Key, Value>>>
using ordered_dict = std::map<Key, Value, Compare, Alloc>;


template <typename T>
struct is_dict : std::false_type { };

template <typename Key, typename Value, typename Hash, typename Predicate, typename Alloc>
struct is_dict<dict<Key, Value, Hash, Predicate, Alloc>> : std::true_type { };

template <typename Key, typename Value, typename Compare, typename Alloc>
struct is_dict<ordered_dict<Key, Value, Compare, Alloc>> : std::true_type { };


template <typename T>
constexpr bool is_dict_v = is_dict<T>::value;


namespace concepts {

    template <typename T>
    concept dict = is_dict_v<T>;

} // namespace concepts

} // namespace slw

#endif // SLW_DICT_HPP