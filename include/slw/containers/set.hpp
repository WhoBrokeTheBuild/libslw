#ifndef SLW_SET_HPP
#define SLW_SET_HPP

#include <slw/format.hpp>

#include <set>
#include <unordered_set>

namespace slw {

template <typename Key,
    typename Hash = std::hash<Key>,
    typename Predicate = std::equal_to<Key>,
    typename Alloc = std::allocator<Key>>
using set = std::unordered_set<Key, Hash, Predicate, Alloc>;

template <typename Key,
    typename Compare = std::less<Key>,
    typename Alloc = std::allocator<Key>>
using ordered_set = std::set<Key, Compare, Alloc>;


template <typename T>
struct is_set : std::false_type { };

template <typename Key, typename Hash, typename Predicate, typename Alloc>
struct is_set<set<Key, Hash, Predicate, Alloc>> : std::true_type { };

template <typename Key, typename Compare, typename Alloc>
struct is_set<ordered_set<Key, Compare, Alloc>> : std::true_type { };


template <typename T>
constexpr bool is_set_v = is_set<T>::value;


namespace concepts {

    template <typename T>
    concept set = is_set_v<T>;

} // namespace concepts

} // namespace slw

#endif // SLW_SET_HPP