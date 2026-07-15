#ifndef SLW_INTEGERS_HPP
#define SLW_INTEGERS_HPP

#include <slw/strings.hpp>
#include <slw/exceptions.hpp>

#include <charconv>
#include <limits>
#include <type_traits>

#include <cstdint>

namespace slw {

using std::int8_t;

using std::int16_t;

using std::int32_t;

using std::int64_t;

using std::uint8_t;

using std::uint16_t;

using std::uint32_t;

using std::uint64_t;

using std::intmax_t;

using std::uintmax_t;


using std::numeric_limits;

using std::is_integral_v;

using std::is_signed_v;

using std::is_unsigned_v;


template <typename T = int>
    requires is_integral_v<T>
constexpr inline T to_integer(string_view str, int base = 10)
{
    T value = {};
    const auto& [ptr, ec] = std::from_chars(str.begin(), str.end(), value, base);
    if (ec == std::errc()) {
        return value;
    }
    throw system_error(make_error_code(ec));
}


namespace concepts {

    using std::integral;

    using std::signed_integral;

    using std::unsigned_integral;

} // namespace concepts


} // namespace slw

#endif // SLW_INTEGERS_HPP
