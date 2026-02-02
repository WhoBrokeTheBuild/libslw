#ifndef SLW_FLOATS_HPP
#define SLW_FLOATS_HPP

#include <slw/exceptions.hpp>
#include <slw/strings.hpp>

#include <charconv>
#include <limits>
#include <type_traits>

namespace slw {

typedef float float32;

typedef double float64;

typedef long double float128;


using std::numeric_limits;

using std::chars_format;

using std::is_floating_point_v;


template <typename T = float>
    requires is_floating_point_v<T>
constexpr inline T to_float(string_view str, chars_format fmt = chars_format::general)
{
    T value = {};
    const auto& [ptr, ec] = std::from_chars(str.begin(), str.end(), value, fmt);
    if (ec == std::errc()) {
        return value;
    }
    throw system_error(make_error_code(ec));
}


namespace concepts {

    using std::floating_point;

} // namespace concepts


static_assert(sizeof(float32) == 4);
static_assert(sizeof(float64) == 8);
static_assert(sizeof(float128) == 16);
// static_assert(to_float("3.14") == 3.14);

} // namespace slw

#endif // SLW_FLOATS_HPP
