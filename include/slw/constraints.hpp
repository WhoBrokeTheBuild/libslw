#ifndef SLW_CONSTRAINTS_HPP
#define SLW_CONSTRAINTS_HPP

#include <type_traits>
#include <concepts>

namespace slw {

using std::is_same_v;

using std::is_base_of_v;

using std::is_convertible_v;

using std::is_function_v;

using std::is_void_v;

using std::is_fundamental_v;

using std::is_compound_v;

using std::is_union_v;

using std::is_enum_v;

using std::is_scoped_enum_v;

using std::is_reference_v;

using std::is_pointer_v;

using std::is_arithmetic_v;


namespace concepts {

    using std::same_as;

    using std::derived_from;

    using std::convertible_to;

    using std::invocable;

} // namespace concepts

} // namespace slw

#endif // SLW_CONSTRAINTS_HPP