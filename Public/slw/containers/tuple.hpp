#ifndef SLW_TUPLE_HPP
#define SLW_TUPLE_HPP

#include <slw/format.hpp>

#include <tuple>

namespace slw {

using std::tuple;

using std::apply;

} // namespace slw

// template <typename ...Args>
// struct std::formatter<slw::tuple<Args...>> : public slw::range_formatter
// {
//     using slw::range_formatter::parse;

//     inline auto format(const auto& tuple, auto& ctx) const {
//         std::format_to(ctx.out(), "{{ ");

//         slw::apply([&](const auto&... values) {
//             ((std::format_to(ctx.out(), "{}, ", values)), ...);
//         }, tuple);

//         return std::format_to(ctx.out(), "}}");
//     }
// };

namespace slw {

template <typename ...Args>
inline string to_string(const slw::tuple<Args...>& array) {
    return std::format("{}", array);
}

} // namespace slw

#endif // SLW_TUPLE_HPP