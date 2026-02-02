#ifndef SLW_ARRAY_HPP
#define SLW_ARRAY_HPP

#include <slw/format.hpp>

#include <array>

namespace slw {

using std::array;

using std::is_array_v;

namespace concepts {

    template <typename T>
    concept array = is_array_v<T>;

} // namespace concepts

} // namespace slw

#endif // SLW_ARRAY_HPP
