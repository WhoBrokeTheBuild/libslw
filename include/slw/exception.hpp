#ifndef SLW_EXCEPTION_HPP
#define SLW_EXCEPTION_HPP

#include <cerrno>
#include <stdexcept>
#include <system_error>

namespace slw {

using std::exception;

using std::runtime_error;

using std::out_of_range;

using std::invalid_argument;

using std::system_error;

using std::make_error_code;

inline std::system_error last_system_error() {
    return std::system_error(errno, std::generic_category());
}

inline std::system_error last_system_error(const char * what) {
    return std::system_error(errno, std::generic_category(), what);
}

} // namespace slw

#endif // SLW_EXCEPTION_HPP
