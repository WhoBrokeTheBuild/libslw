#ifndef SLW_CHRONO_HPP
#define SLW_CHRONO_HPP

#include <chrono>

namespace slw {

using namespace std::chrono;

using namespace std::chrono_literals;

inline struct timeval to_timeval(nanoseconds duration)
{
    auto secs = duration_cast<seconds>(duration);
    duration -= secs;

    auto msec = duration_cast<microseconds>(duration);

    return {
        .tv_sec = secs.count(),
        .tv_usec = msec.count(),
    };
}

inline struct timespec to_timespec(nanoseconds duration)
{
    auto secs = duration_cast<seconds>(duration);
    duration -= secs;

    return {
        .tv_sec = secs.count(),
        .tv_nsec = duration.count(),
    };
}

} // namespace slw

#endif // SLW_CHRONO_HPP
