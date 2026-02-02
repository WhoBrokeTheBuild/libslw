#ifndef SLW_SYMLINK_HPP
#define SLW_SYMLINK_HPP

#include <slw/io/paths.hpp>

#include <filesystem>

namespace slw {

namespace symlink {

    static inline path follow(const path& link) {
        return std::filesystem::read_symlink(link);
    }

    static inline void create(const path& src, const path& dst) {
        std::filesystem::create_symlink(src, dst);
    }

    static inline void copy(const path& src, const path& dst) {
        return std::filesystem::copy_symlink(src, dst);
    }

} // namespace symlink

} // namespace slw

#endif // SLW_SYMLINK_HPP
