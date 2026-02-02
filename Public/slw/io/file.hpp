#ifndef SLW_FILE_HPP
#define SLW_FILE_HPP

#include <slw/ranges.hpp>
#include <slw/strings.hpp>
#include <slw/containers.hpp>

#include <slw/io/paths.hpp>
#include <slw/io/directory.hpp>

#include <cstdio>
#include <print>
#include <type_traits>

namespace slw {

enum class seekdir
{
    beg = SEEK_SET,
    end = SEEK_END,
    cur = SEEK_CUR,

    set = beg,
    begin = beg,
    current = cur,

}; // enum class seekdir

class file
{
public:

    static inline file create_temporary(std::string_view prefix = "", const char * mode = "wb", const path& directory = directory::temp()) {
        slw::path file_template = directory / std::format("{}{}", prefix, "XXXXXX");
        const char * result = ::mktemp(const_cast<char *>(file_template.c_str()));
        if (!result) {
            throw last_system_error();
        }
        
        return file(file_template, mode);
    }

    static inline bool copy(const path& src, const path& dst) {
        return std::filesystem::copy_file(src, dst);
    }

    static inline bool exists(const path& path) {
        return std::filesystem::exists(path) && std::filesystem::is_regular_file(path);
    }

    static inline bool empty(const path& path) {
        return std::filesystem::is_empty(path);
    }

    static inline void rename(const path& src, const path& dst) {
        std::filesystem::rename(src, dst);
    }

    static inline bool remove(const path& path) {
        return std::filesystem::remove(path);
    }

    static inline size_t size(const path& path) {
        return std::filesystem::file_size(path);
    }

    file() = default;

    file(FILE * fp, const path& path = {})
        : _file(fp)
        , _path(path)
    { }

    file(const path& path, const char * mode = "rb")
    {
        open(path, mode);
    }

    virtual ~file() = default;

    void open(const path& path, const char * mode = "rb")
    {
        close();

        _file = std::fopen(path.c_str(), mode);
        if (_file == nullptr) {
            throw last_system_error();
        }
    }

    void reopen(const char * mode = "rb")
    {
        _file = std::freopen(_path.c_str(), mode, _file);
        if (_file == nullptr) {
            throw last_system_error();
        }
    }

    inline void close()
    {
        if (_file) {
            std::fclose(_file);
            _file = nullptr;
        }
    }

    FILE * c_file() const {
        return _file;
    }

    inline operator FILE *() const {
        return _file;
    }

    inline path path() const {
        return _path;
    }

    inline bool open() const {
        return (_file != nullptr);
    }

    inline bool eof() const {
        return std::feof(_file);
    }

    inline bool error() const {
        return std::ferror(_file);
    }

    inline operator bool() const {
        return (open() && ! error() && ! eof());
    }

    inline auto tell() {
        return std::ftell(_file);
    }

    inline bool seek(ssize_t offset, seekdir dir = seekdir::current)
    {
        int result = std::fseek(_file, offset, int(dir));
        return (result == 0);
    }

    inline bool rewind() {
        return seek(0, seekdir::begin);
    }

    size_t size()
    {
        size_t start = std::ftell(_file);
        std::fseek(_file, 0, SEEK_END);
        size_t size = std::ftell(_file);
        std::fseek(_file, start, SEEK_SET);
        return size;
    }

    size_t remaining_size() {
        return size() - tell();
    }

    inline void read_into(void * buffer, size_t size)
    {
        size_t count = std::fread(buffer, 1, size, _file);
        if (count != size) {
            throw last_system_error();
        }
    }

    inline void write_from(const void * buffer, size_t size)
    {
        size_t count = std::fwrite(buffer, 1, size, _file);
        if (count != size) {
            throw last_system_error();
        }
    }

    template <ranges::viewable_range R>
    inline void read_into(R&& range)
    {
        size_t size_bytes = ranges::size(range) * sizeof(std::ranges::range_value_t<R>);
        read_into(ranges::data(range), size_bytes);
    }

    template <ranges::viewable_range R>
    inline void write_from(R&& range)
    {
        size_t size_bytes = ranges::size(range) * sizeof(std::ranges::range_value_t<R>);
        write_from(ranges::cdata(range), size_bytes);
    }

    template <typename T = uint8_t>
        requires std::is_fundamental_v<T>
    inline T read_one()
    {
        if constexpr (std::is_integral_v<T> && sizeof(T) == 1) {
            int c = std::fgetc(_file);
            if (c == EOF) {
                throw last_system_error();
            }
            return c;
        }
        else {
            T value = {};
            read_into(&value, sizeof(value));
            return value;
        }
    }

    template <typename T = uint8_t>
        requires std::is_fundamental_v<T>
    inline void write_one(T value)
    {
        if constexpr (std::is_integral_v<T> && sizeof(T) == 1) {
            int c = std::fputc(value, _file);
            if (c == EOF) {
                throw last_system_error();
            }
        }
        else {
            write_from(&value, sizeof(value));
        }
    }

    template <typename T = uint8_t>
    inline list<T> read(size_t count)
    {
        list<T> data(count);
        read_into(data);
        return data;
    }

    template <typename T = uint8_t>
    inline list<T> read() {
        return read<T>(remaining_size() / sizeof(T));
    }

    template <typename T>
    inline void read_object(T& object) {
        read_into(&object, sizeof(T));
    }

    template <typename T>
    inline void write_object(const T& object) {
        write_from(&object, sizeof(T));
    }

    inline string read_string(size_t size)
    {
        string result(size, '\0');
        read_into(result.data(), result.size());
        return result;
    }

    inline string read_string() {
        return read_string(remaining_size());
    }

    inline void write_string(string_view text) {
        write_from(text.data(), text.size());
    }

    string read_line();

    list<string> read_lines();

    inline void write_line(string_view line)
    {
        write_string(line);
        write_one('\n');
    }

    template <ranges::viewable_range R>
        requires is_string_v<ranges::range_value_t<R>> or is_cstring_v<ranges::range_value_t<R>>
    inline void write_lines(R&& range)
    {
        for (const auto& line : range) {
            write_line(line);
        }
    }

    template <typename T>
        requires is_string_v<T> or is_cstring_v<T>
    inline void write_lines(initializer_list<T> lines) {
        write_lines(list<T>(lines));
    }

    template <typename ...Args>    
    inline void print(std::format_string<Args...> format, Args&&... args) {
        std::print<Args...>(_file, format, std::forward<Args>(args)...);
    }

    inline void println() {
        std::println(_file);
    }

    template <typename ...Args>
    inline void println(std::format_string<Args...> format, Args&&... args) {
        std::println<Args...>(_file, format, std::forward<Args>(args)...);
    }

private:

    std::FILE * _file = nullptr;

    slw::path _path;

}; // class file

} // namespace slw

#endif // SLW_FILE_HPP