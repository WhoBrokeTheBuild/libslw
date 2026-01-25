#ifndef SLW_DICT_HPP
#define SLW_DICT_HPP

#include <map>
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


} // namespace slw

#endif // SLW_DICT_HPP