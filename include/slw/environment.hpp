#ifndef SLW_ENVIRONMENT_HPP
#define SLW_ENVIRONMENT_HPP

#include <slw/strings.hpp>
#include <slw/exceptions.hpp>
#include <slw/dict.hpp>

#include <unistd.h>

namespace slw {

namespace environ {

    inline void set(const string& name, const string& value, bool overwrite = true) {
        ::setenv(name.c_str(), value.c_str(), overwrite);
    }

    inline void unset(const string& name) {
        ::unsetenv(name.c_str());
    }

    inline string get(const string& name) {
        const char * value = ::getenv(name.c_str());
        if (value) {
            return value;
        }

        throw slw::out_of_range(name);
    }

    inline string get(const string& name, const string& default_value) {
        const char * value = ::getenv(name.c_str());
        if (value) {
            return value;
        }

        return default_value;
    }

    inline bool contains(const string& name) {
        const char * value = ::getenv(name.c_str());
        return (value != nullptr);
    }

    inline list<string> get_list()
    {
        list<string> result;

        char ** ptr = ::environ;
        while (*ptr) {
            result.push_back(*ptr);
            ++ptr;
        }

        return result;
    }

    inline dict<string, string> get_dict()
    {
        dict<string, string> result;

        char ** ptr = ::environ;
        while (*ptr) {
            string_view pair = *ptr;
            ++ptr;

            size_t pivot = pair.find_first_of('=');
            result[string(pair.substr(0, pivot))] = pair.substr(pivot + 1);
        }

        return result;
    }

}; // namespace environ

} // namespace slw

#endif // SLW_ENVIRONMENT_HPP
