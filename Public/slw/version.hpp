#ifndef SLW_VERSION_HPP
#define SLW_VERSION_HPP

#include <slw/format.hpp>
#include <slw/integers.hpp>
#include <slw/strings.hpp>

#include <slw/containers/tuple.hpp>

namespace slw {

class version
{
public:

    version(int major = 0, int minor = 0, int patch = 0)
        : _major(major)
        , _minor(minor)
        , _patch(patch)
    { }

    version(const string& str) {
        parse(str);
    }

    inline constexpr int major() const {
        return _major;
    }

    inline constexpr int minor() const {
        return _minor;
    }

    inline constexpr int patch() const {
        return _patch;
    }

    inline constexpr tuple<int, int, int> tuple() const {
        return { _major, _minor, _patch };
    }

    void parse(string_view str);

    constexpr int compare(const version& other) const;

private:

    int _major;
    int _minor;
    int _patch;

}; // class version

inline void version::parse(string_view str)
{
    const auto& parts = string_views::split(str, '.');
    if (parts.size() > 0) {
        _major = to_integer(parts[0]);
        if (parts.size() > 1) {
            _minor = to_integer(parts[1]);
            if (parts.size() > 2) {
                _patch = to_integer(parts[2]);
            }
        }
    }
}

inline constexpr int version::compare(const version& other) const
{
    auto cmp = [](int a, int b) {
        if (a == b) {
            return 0;
        }
        if (a > b) {
            return 1;
        }
        return -1;
    };

    int result = cmp(_major, other._major);
    if (result == 0) {
        result = cmp(_minor, other._minor);
        if (result == 0) {
            result = cmp(_patch, other._patch);
        }
    }

    return result;
}

inline bool operator==(const version& lhs, const version& rhs) {
    return (lhs.compare(rhs) == 0);
}

inline bool operator!=(const version& lhs, const version& rhs) {
    return (lhs.compare(rhs) != 0);
}

inline bool operator>(const version& lhs, const version& rhs) {
    return (lhs.compare(rhs) > 0);
}

inline bool operator>=(const version& lhs, const version& rhs) {
    return (lhs.compare(rhs) >= 0);
}

inline bool operator<(const version& lhs, const version& rhs) {
    return (lhs.compare(rhs) < 0);
}

inline bool operator<=(const version& lhs, const version& rhs) {
    return (lhs.compare(rhs) <= 0);
}

} // namespace slw

template <>
struct std::formatter<slw::version> : public std::formatter<std::string_view>
{
    using std::formatter<std::string_view>::parse;

    auto format(const auto& version, auto& ctx) const {
        return std::format_to(ctx.out(), "{}.{}.{}",
            version.major(),
            version.minor(),
            version.patch());
    }
};

namespace slw {

inline string to_string(const version& version) {
    return std::format("{}", version);
}

} // namespace slw

#endif // SLW_VERSION_HPP
