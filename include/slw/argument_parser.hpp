#ifndef SLW_ARGUMENT_PARSER_HPP
#define SLW_ARGUMENT_PARSER_HPP

#include <slw/strings.hpp>
#include <slw/io/console.hpp>
#include <slw/exceptions.hpp>

#include <functional>

namespace slw {

using std::function;

class argument_parser
{
public:

    struct flag
    {
    public:

        enum class type 
        {
            UNDEFINED = -1,
            STRING,
            BOOL,
            INT,
            FLOAT,
            STRING_CALLBACK,
            BOOL_CALLBACK,
            INT_CALLBACK,
            FLOAT_CALLBACK,
        };

        char        short_name;
        string_view long_name;
        string_view description;

        type        type;
        unsigned    count;

        union {
            string * string_ptr;
            bool *   bool_ptr;
            int *    int_ptr;
            float *  float_ptr;
        };

        function<void(string)> string_callback;
        function<void(bool)>   bool_callback;
        function<void(int)>    int_callback;
        function<void(float)>  float_callback;

        inline void process(string value)
        {
            auto parse_bool = [](string_view str) {
                return !(
                    str == "false" ||
                    str == "FALSE" ||
                    str == "0"
                );
            };

            ++count;

            switch (type) {
            case type::STRING:
                if (string_ptr) {
                    *string_ptr = value;
                }
                break;
            case type::STRING_CALLBACK:
                if (string_callback) {
                    if (!value.empty()) {
                        string_callback(value);
                    }
                }
                break;
            case type::BOOL:
                if (bool_ptr) {
                    if (!value.empty()) {
                        *bool_ptr = parse_bool(value);
                    }
                    else {
                        *bool_ptr = true;
                    }
                }
                break;
            case type::BOOL_CALLBACK:
                if (bool_callback) {
                    if (!value.empty()) {
                        bool_callback(parse_bool(value));
                    }
                    else {
                        bool_callback(true);
                    }
                }
                break;
            case type::INT:
                if (int_ptr) {
                    if (!value.empty()) {
                        *int_ptr = strtol(value.c_str(), nullptr, 10);
                    }
                }
                break;
            case type::INT_CALLBACK:
                if (int_callback) {
                    if (!value.empty()) {
                        int_callback(strtol(value.c_str(), nullptr, 10));
                    }
                }
                break;
            case type::FLOAT:
                if (float_ptr) {
                    if (!value.empty()) {
                        *float_ptr = strtof(value.c_str(), nullptr);
                    }
                }
                break;
            case type::FLOAT_CALLBACK:
                if (float_callback) {
                    if (!value.empty()) {
                        float_callback(strtof(value.c_str(), nullptr));
                    }
                }
                break;
            default: ;
            }
        }

    }; // struct flag

    string program;

    list<string_view> args;

    int argc;
    char ** argv;

    argument_parser()
        : argc(0)
        , argv(nullptr)
    { }

    flag * add_flag(flag&& flag)
    {
        _flags.push_back(flag);
        return &_flags.back();
    }

    flag * add_string(char short_name, string_view long_name, string * value_ptr, string_view description)
    {
        flag flag = {
            .short_name = short_name,
            .long_name = long_name,
            .description = description,
            .type = flag::type::STRING,
            .string_ptr = value_ptr,
        };

        _flags.push_back(flag);
        return &_flags.back();
    }
    
    flag * add_bool(char short_name, string_view long_name, bool * value_ptr, string_view description)
    {
        flag flag = {
            .short_name = short_name,
            .long_name = long_name,
            .description = description,
            .type = flag::type::BOOL,
            .bool_ptr = value_ptr,
        };

        _flags.push_back(flag);
        return &_flags.back();
    }
    
    flag * add_int(char short_name, string_view long_name, int * value_ptr, string_view description)
    {
        flag flag = {
            .short_name = short_name,
            .long_name = long_name,
            .description = description,
            .type = flag::type::INT,
            .int_ptr = value_ptr,
        };

        _flags.push_back(flag);
        return &_flags.back();
    }
    
    flag * add_float(char short_name, string_view long_name, float * value_ptr, string_view description)
    {
        flag flag = {
            .short_name = short_name,
            .long_name = long_name,
            .description = description,
            .type = flag::type::FLOAT,
            .float_ptr = value_ptr,
        };

        _flags.push_back(flag);
        return &_flags.back();
    }

    flag * add_string_callback(char short_name, string_view long_name, function<void(string)> callback, string_view description)
    {
        flag flag = {
            .short_name = short_name,
            .long_name = long_name,
            .description = description,
            .type = flag::type::STRING_CALLBACK,
            .string_callback = callback,
        };

        _flags.push_back(flag);
        return &_flags.back();
    }

    flag * add_bool_callback(char short_name, string_view long_name, function<void(bool)> callback, string_view description)
    {
        flag flag = {
            .short_name = short_name,
            .long_name = long_name,
            .description = description,
            .type = flag::type::BOOL_CALLBACK,
            .bool_callback = callback,
        };

        _flags.push_back(flag);
        return &_flags.back();
    }

    flag * add_int_callback(char short_name, string_view long_name, function<void(int)> callback, string_view description)
    {
        flag flag = {
            .short_name = short_name,
            .long_name = long_name,
            .description = description,
            .type = flag::type::INT_CALLBACK,
            .int_callback = callback,
        };

        _flags.push_back(flag);
        return &_flags.back();
    }

    flag * add_float_callback(char short_name, string_view long_name, function<void(float)> callback, string_view description)
    {
        flag flag = {
            .short_name = short_name,
            .long_name = long_name,
            .description = description,
            .type = flag::type::FLOAT_CALLBACK,
            .float_callback = callback,
        };

        _flags.push_back(flag);
        return &_flags.back();
    }

    inline void parse(int main_argc, char * main_argv[])
    {
        argc = main_argc;
        argv = main_argv;

        program = argv[0];

        bool passthrough = false;
        for (int i = 1; i < argc; ++i) {
            char * pch = argv[i];
            if (!passthrough && *pch == '-') {
                ++pch;
                if (*pch == '-') {
                    ++pch;

                    if (*pch == '\0') {
                        // All following flags are not to be processed
                        passthrough = true;
                        continue;
                    }

                    // Long
                    string_view key = pch;
                    string_view value;

                    char * divider = strchr(pch, '=');
                    if (divider) {
                        key = key.substr(0, divider - pch);
                        value = divider + 1;
                    }

                    bool next_arg_is_value = (i + 1 < argc && argv[i + 1][0] != '-');

                    bool found = false;
                    for (auto& flag : _flags) {
                        if (flag.long_name == key) {
                            found = true;

                            if (!value.empty()) {
                                flag.process(string(value));
                            }
                            else if (flag.type == flag::type::BOOL || flag.type == flag::type::BOOL_CALLBACK) {
                                flag.process(string());
                            }
                            else if (next_arg_is_value) {
                                flag.process(string(argv[i + 1]));
                                ++i;
                            }
                            else {
                                throw invalid_argument(format("option '-{}' requires a value", key));
                            }

                            break;
                        }
                    }

                    if (!found) {
                        throw invalid_argument(format("unrecognized option '--{}'", key));
                    }
                }
                else {
                    // Short
                    while (*pch) {
                        bool is_last_short_flag = (*(pch + 1) == '\0');
                        bool next_arg_is_value = (i + 1 < argc && argv[i + 1][0] != '-');

                        bool found = false;
                        for (flag& flag : _flags) {
                            if (flag.short_name == *pch) {
                                found = true;

                                if (is_last_short_flag && next_arg_is_value) {
                                    flag.process(string(argv[i + 1]));
                                    ++i;
                                }
                                else if (flag.type == flag::type::BOOL || flag.type == flag::type::BOOL_CALLBACK) {
                                    flag.process(string());
                                }
                                else {
                                    throw invalid_argument(format("option '-{}' requires a value", *pch));
                                }

                                break;
                            }
                        }

                        if (!found) {
                            throw invalid_argument(format("unrecognized option '-{}'", *pch));
                        }

                        ++pch;
                    }
                }
            }
            else {
                args.push_back(pch);
                _argv.push_back(pch);
            }
        }

        argc = static_cast<int>(_argv.size());
        argv = _argv.data();
    }

    string get_usage(string_view usage, string_view above, string_view below)
    {
        string output = format("Usage: {} {}\n", program, usage);
        output += format("{}\n\n", above);

        for (auto& flag : _flags) {
            output += "  ";
            if (flag.short_name != '\0') {
                output += format("-{}, ", flag.short_name);
            }
            else {
                output += "    ";
            }

            if (!flag.long_name.empty()) {
                output += format("--{}", flag.long_name);
            }
            if (flag.long_name.size() > 20) {
                output += "\n                            ";
            }
            else {
                for (size_t i = 0; i < 20 - flag.long_name.size(); ++i) {
                    output += " ";
                }
            }
            output += format("{}\n", flag.description);
        }

        output += format("\n{}\n", below);
        return output;
    }

    inline void print_usage(const string& usage, const string& above, const string& below)
    {
        print("{}", get_usage(usage, above, below));
    }

private:

    list<char *> _argv;
    
    list<flag> _flags;

}; // class argument_parser

} // namespace slw

#endif // SLW_ARGUMENT_PARSER_HPP