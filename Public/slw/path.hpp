#ifndef SLW_PATH_HPP
#define SLW_PATH_HPP

#include <slw/strings.hpp>
#include <slw/exceptions.hpp>

#include <filesystem>

#include <unistd.h>

namespace slw {

using std::filesystem::path;

inline path get_working_directory() {
    std::unique_ptr<char> cwd(::getcwd(nullptr, 0));
    return path(cwd.get());
}

inline void set_working_directory(const path& path) {
    int result = ::chdir(path.c_str());
    if (result != 0) {
        throw last_system_error("Failed to set change directory");
    }
}

inline path make_temporary_directory(string_view prefix) {
    string temp(prefix);
    temp += "XXXXXX";

    const char * result = ::mkdtemp(temp.data());
    if (temp.ends_with("XXXXXX")) {
        throw std::system_error(errno, std::generic_category(), "Failed to create temporary directory");
    }

    return result;
}

inline void remove_file(const path& path) {
    int result = std::remove(path.c_str());
    if (result != 0) {
        throw std::system_error(errno, std::generic_category(), "Failed to remove file");
    }
}

inline void remove_directory(const path& path) {
    int result = ::rmdir(path.c_str());
    if (result != 0) {
        throw std::system_error(errno, std::generic_category(), "Failed to remove directory");
    }
}

} // namespace slw

#endif // SLW_PATH_HPP
