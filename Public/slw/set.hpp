#ifndef SLW_SET_HPP
#define SLW_SET_HPP

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

} // namespace slw

#endif // SLW_SET_HPP