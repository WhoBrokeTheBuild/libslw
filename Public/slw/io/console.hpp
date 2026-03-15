#ifndef SLW_CONSOLE_HPP
#define SLW_CONSOLE_HPP

#include <slw/strings.hpp>

#include <print>

namespace slw {

namespace console {

template <typename ...Args>
inline void print(std::format_string<Args...> format, Args&&... args) {
    std::print<Args...>(format, std::forward<Args>(args)...);
}

inline void println() {
    std::println();
}

template <typename ...Args>
inline void println(std::format_string<Args...> format, Args&&... args) {
    std::println<Args...>(format, std::forward<Args>(args)...);
}

// TODO: Colors
    
} // namespace console

} // namespace slw

#endif // SLW_CONSOLE_HPP
