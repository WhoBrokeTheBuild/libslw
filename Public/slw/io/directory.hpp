#ifndef SLW_DIRECTORY_HPP
#define SLW_DIRECTORY_HPP

#include <slw/io/paths.hpp>

#include <filesystem>

namespace slw {

namespace directory {

    static inline void current(const path& path) {
        std::filesystem::current_path(path);
    }

    static inline path current() {
        return std::filesystem::current_path();
    }

    static inline path temp() {
        return std::filesystem::temp_directory_path();
    }

    static std::filesystem::space_info space(const path& path) {
        return std::filesystem::space(path);
    }

    static inline bool exists(const path& path) {
        return std::filesystem::exists(path) && std::filesystem::is_directory(path);
    }

    using std::filesystem::rename;

    using std::filesystem::remove;

    using std::filesystem::remove_all;

    static inline bool create(const path& path, bool create_parents = true)
    {
        if (create_parents) {
            return std::filesystem::create_directories(path);
        }
        else {
            return std::filesystem::create_directory(path);
        }
    }

    static inline path create_temporary(std::string_view prefix = "", const path& directory = temp())
    {
        slw::path directory_template = directory / std::format("{}{}", prefix, "XXXXXX");
        const char * result = ::mkdtemp(const_cast<char *>(directory_template.c_str()));
        if (!result) {
            throw last_system_error();
        }

        return directory_template;
    }

}; // namespace directory

} // namespace slw

#endif // SLW_DIRECTORY_HPP
