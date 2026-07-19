#ifndef SLW_PATH_HPP
#define SLW_PATH_HPP

#include <slw/exception.hpp>
#include <slw/string.hpp>

#include <filesystem>

#include <unistd.h>

namespace slw {

using std::filesystem::path;

namespace paths {

    using std::filesystem::canonical;

    using std::filesystem::absolute;

    using std::filesystem::relative;

    using std::filesystem::proximate; // TODO: Investigate

    using std::filesystem::equivalent;

    using std::filesystem::exists;

    static inline bool is_file(const path& path) {
        return std::filesystem::is_regular_file(path);
    }

    using std::filesystem::is_directory;

    using std::filesystem::is_symlink;

    using std::filesystem::is_empty;

    static inline std::filesystem::file_status stat(const path& path) {
        return std::filesystem::status(path);
    }

} // namespace paths

} // namespace slw

#endif // SLW_PATH_HPP
