#ifndef SLW_TO_STRING_HPP
#define SLW_TO_STRING_HPP

#include <slw/strings.hpp>

#include <slw/array.hpp>
#include <slw/dict.hpp>
#include <slw/initializer_list.hpp>
#include <slw/list.hpp>
#include <slw/ranges.hpp>
#include <slw/set.hpp>
#include <slw/tuple.hpp>
#include <slw/view.hpp>

#include <sstream>

namespace slw {

using std::to_string;

template <typename T>
constexpr string to_display_string(const T& value);

// template <typename Type, size_t Size>
// inline constexpr string to_string(const array<Type, Size>& array)
// {
//     std::stringstream ss;
//     ss << "[" << Size << "]{ ";
//     for (const auto& value : array) {
//         ss << to_display_string(value) << ", ";
//     }
//     ss << "}";
//     return ss.str();
// }

// template <typename Key, typename Value, typename Hash, typename Predicate, typename Alloc>
// inline constexpr string to_string(const dict<Key, Value, Hash, Predicate, Alloc>& dict)
// {
//     std::stringstream ss;

//     ss << "{ ";
//     for (const auto& [key, value] : dict) {
//         ss << to_display_string(key) << ": " << to_display_string(value) << ", ";
//     }
//     ss << "}";

//     return ss.str();
// }

// template <typename Key, typename Value, typename Compare, typename Alloc>
// inline constexpr string to_string(const ordered_dict<Key, Value, Compare, Alloc>& dict)
// {
//     std::stringstream ss;

//     ss << "{ ";
//     for (const auto& [key, value] : dict) {
//         ss << to_display_string(key) << ": " << to_display_string(value) << ", ";
//     }
//     ss << "}";
    
//     return ss.str();
// }

template <typename T>
inline constexpr string to_string(const initializer_list<T>& list)
{
    std::stringstream ss;
    ss << "{ ";
    for (const auto& value : list) {
        ss << to_display_string(value) << ", ";
    }
    ss << "}";
    return ss.str();
}

// template <typename Key, typename Hash, typename Predicate, typename Alloc>
// inline constexpr string to_string(const set<Key, Hash, Predicate, Alloc>& set)
// {
//     std::stringstream ss;

//     ss << "{ ";
//     for (const auto& value : set) {
//         ss << to_display_string(value) << ", ";
//     }
//     ss << "}";
    
//     return ss.str();
// }

template <typename Key, typename Compare, typename Alloc>
inline constexpr string to_string(const ordered_set<Key, Compare, Alloc>& set)
{
    std::stringstream ss;

    ss << "{ ";
    for (const auto& value : set) {
        ss << to_display_string(value) << ", ";
    }
    ss << "}";
    
    return ss.str();
}

template <typename ...Elements>
inline constexpr string to_string(const tuple<Elements...>& tuple)
{
    std::stringstream ss;

    ss << "( ";

    apply([&ss](const auto&... args) {
        ((ss << to_display_string(args) << ", "), ...);
    }, tuple);

    ss << ")";
    
    return ss.str();
}

// template <typename Type, size_t Extent>
// inline constexpr string to_string(const view<Type, Extent>& view)
// inline constexpr string to_string(ranges::view auto view)
// {
//     std::stringstream ss;
//     ss << "[ ";
//     for (const auto& value : view) {
//         ss << to_display_string(value) << ", ";
//     }
//     ss << "]";
//     return ss.str();
// }

// template <typename Type, typename Alloc>
// inline constexpr std::string to_string(const list<Type, Alloc>& list)
// {
//     std::stringstream ss;
//     ss << "[ ";
//     for (const auto& value : list) {
//         ss << to_display_string(value) << ", ";
//     }
//     ss << "]";
//     return ss.str();
// }

template <typename T1, typename T2>
inline constexpr string to_string(const pair<T1, T2>& pair)
{
    std::stringstream ss;

    ss << "( " << to_display_string(pair.first) << ", " << to_display_string(pair.second) << " )";

    return ss.str();
}

template <ranges::viewable_range R>
inline constexpr string to_string(R&& range) {
    std::stringstream ss;
    ss << "[ ";
    for (const auto& value : range) {
        ss << to_display_string(value) << ", ";
    }
    ss << "]";
    return ss.str();
}

template <typename T>
inline constexpr string to_display_string(const T& value)
{
    if constexpr (is_string_like_v<T> or std::is_same_v<T, char>) {
        return strings::quoted(value);
    }
    else {
        return to_string(value);
    }
}

} // namespace slw

#endif // SLW_TO_STRING_HPP