#ifndef SLW_EXCEPTIONS_HPP
#define SLW_EXCEPTIONS_HPP

#include <system_error>

#include <cerrno>

namespace slw {

using std::exception;

using std::runtime_error;

using std::system_error;

inline std::system_error last_system_error() {
    return std::system_error(errno, std::generic_category());
}

inline std::system_error last_system_error(const char * what) {
    return std::system_error(errno, std::generic_category(), what);
}

} // namespace slw

#endif // SLW_EXCEPTIONS_HPP
