#ifndef SLW_PATHS_HPP
#define SLW_PATHS_HPP

#include <slw/strings.hpp>
#include <slw/exceptions.hpp>

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

#endif // SLW_PATHS_HPP
