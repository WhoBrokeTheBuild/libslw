#ifndef SLW_FORMAT_HPP
#define SLW_FORMAT_HPP

#include <slw/ranges.hpp>

#include <slw/containers/string.hpp>

#include <format>
#include <string>

namespace slw {

using std::format;

using std::format_to;

using std::to_string;


inline std::string format_human_readable(size_t size_bytes)
{
    constexpr std::array<const char *, 7> SUFFIX_LIST = {
        "B", "KiB", "MiB", "GiB", "TiB", "PiB", "EiB"
    };

    double fraction = size_bytes;
    std::size_t suffixIndex = 0;
    while (fraction >= 1024.0) {
        ++suffixIndex;
        fraction /= 1024.0;
    }

    if (suffixIndex >= SUFFIX_LIST.size()) {
        return std::format("{} B", size_bytes);
    }

    return std::format("{:.3f} {}", fraction, SUFFIX_LIST[suffixIndex]);
}

} // namespace slw

#endif // SLW_FORMAT_HPP
