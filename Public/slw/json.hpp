#ifndef SLW_JSON_HPP
#define SLW_JSON_HPP

#include <slw/containers.hpp>
#include <slw/strings.hpp>
#include <slw/unicode.hpp>
#include <slw/constraints.hpp>
#include <slw/integers.hpp>
#include <slw/floats.hpp>

#include <slw/io/file.hpp>

namespace slw {

template <typename String>
class json_pointer
{
public:

    json_pointer(const String& string)
        : _parts(strings::split(string, '/'))
    { }

    inline bool empty() const {
        return _parts.empty();
    }

    String next_key() {
        auto key = _parts.back();
        _parts.pop_back();
        return key;
    }

    size_t next_index() {
        size_t index = to_integer<size_t>(_parts.back());
        _parts.pop_back();
        return index;
    }

private:

    list<String> _parts;

}; // class json_pointer

template <
    template<typename Key, typename Value, typename... Args>
    typename Object = slw::dict,

    template <typename Value, typename... Args>
    typename Array = slw::list,

    typename String = slw::string,

    typename Boolean = bool,

    typename NumberInteger = int64_t,

    typename NumberUnsigned = uint64_t,

    typename NumberFloat = double,

    template <typename Type>
    typename Allocator = std::allocator
>
class basic_json
{
public:

    typedef Object<String, basic_json> object_t;

    typedef Array<basic_json> array_t;

    typedef String string_t;

    typedef Boolean boolean_t;

    typedef NumberInteger number_integer_t;

    typedef NumberUnsigned number_unsigned_t;

    typedef NumberFloat number_float_t;

    typedef basic_string_view<typename string_t::value_type, typename string_t::traits_type> string_view_t;

    template <typename T>
    static constexpr bool is_value_t = (
        is_same_v<T, object_t> or is_same_v<T, array_t> or
        is_same_v<T, string_t> or is_same_v<T, boolean_t> or
        is_same_v<T, number_integer_t> or is_same_v<T, number_unsigned_t> or
        is_same_v<T, number_float_t>
    );

    typedef std::variant<object_t, array_t, string_t, boolean_t, number_integer_t, number_unsigned_t, number_float_t> value_t;

    static inline basic_json array() {
        return basic_json(array_t());
    }

    static inline basic_json object() {
        return basic_json(object_t());
    }

    template <typename T>
    static inline basic_json array(initializer_list<T> values) {
        array_t array;
        for (const auto& v : values) {
            array.emplace_back(v);
        }
        return std::move(array);
    }

    template <typename K, typename V>
    static inline basic_json object(initializer_list<tuple<K, V>> pairs) {
        array_t array;
        for (const auto& [k, v] : pairs) {
            array.emplace_back(k, v);
        }
        return std::move(array);
    }

    inline constexpr bool is_object() {
        return std::holds_alternative<object_t>(_value);
    }

    inline constexpr bool is_array() {
        return std::holds_alternative<array_t>(_value);
    }

    inline constexpr bool is_structured() {
        return is_object() || is_array();
    }

    inline constexpr bool is_primitive() {
        return is_string() || is_boolean() || is_number();
    }

    inline constexpr bool is_string() {
        return std::holds_alternative<string_t>(_value);
    }

    inline constexpr bool is_boolean() {
        return std::holds_alternative<boolean_t>(_value);
    }

    inline constexpr bool is_number() {
        return is_number_integer() || is_number_float();
    }

    inline constexpr bool is_number_integer() {
        return std::holds_alternative<number_integer_t>(_value) || is_number_unsigned();
    }

    inline constexpr bool is_number_unsigned() {
        return std::holds_alternative<number_unsigned_t>(_value);
    }

    inline constexpr bool is_number_float() {
        return std::holds_alternative<number_float_t>(_value);
    }

    static basic_json parse(string_view_t string)
    {
        basic_json root;
        string_t key;
        stack<value_t *> stack;

        if (string[0] == '{') {
            root = object();
        }
        else if (string[0] == '[') {
            root = array();
        }
        else {
            throw std::runtime_error("JSON must start with object or array");
        }

        stack.push(&root._value);
        string = string.substr(1);

        auto insert = [&](value_t&& value) {
            value_t& top = *stack.top();
            
            bool is_container = (
                std::holds_alternative<array_t>(value) ||
                std::holds_alternative<object_t>(value)
            );

            if (std::holds_alternative<array_t>(top)) {
                array_t& array = std::get<array_t>(top);
                array.emplace_back(std::move(value));
                if (is_container) {
                    stack.push(&array.back()._value);
                }
            }
            else if (std::holds_alternative<object_t>(top)) {
                object_t& object = std::get<object_t>(top);
                object.emplace(key, std::move(value));
                if (is_container) {
                    stack.push(&object.at(key)._value);
                }
                key.clear();
            }
        };

        while (!stack.empty()) {
            string = string_views::trim_left(string);

            if (string.front() == ',') {
                string = string.substr(1);
                string = string_views::trim_left(string);
            }

            if (string.front() == '{') {
                string = string.substr(1);
                insert(object_t());
            }
            else if (string.front() == '}') {
                string = string.substr(1);
                stack.pop();
            }
            else if (string.front() == '[') {
                string = string.substr(1);
                insert(array_t());
            }
            else if (string.front() == ']') {
                string = string.substr(1);
                stack.pop();                
            }
            else if (string.front() == '"') {
                string_view value;

                for (size_t i = 1; i < string.size(); ++i) {
                    if (string[i] == '\\') {
                        ++i;
                    }
                    else if (string[i] == '"') {
                        value = string.substr(1, i - 1);
                        string = string.substr(i + 1);
                        break;
                    }
                }

                if (string.front() == ':') {
                    string = string.substr(1);
                    key = value;
                }
                else {
                    insert(string_t(value));
                }
            }
            else if (string.starts_with("true")) {
                string = string.substr(4);
                insert(true);
            }
            else if (string.starts_with("false")) {
                insert(false);
                string = string.substr(5);
            }
            else {
                string_view value;

                bool is_signed = false;
                bool is_integer = true;

                for (size_t i = 0; i < string.size(); ++i) {
                    if (i == 0 && string[i] == '-') {
                        is_signed = true;
                        continue;
                    }

                    bool digit = ::isdigit(string[i]);
                    bool point = (string[i] == '.');

                    if (is_integer && point) {
                        is_integer = false;
                    }
                    if (!digit && !point) {
                        value = string.substr(0, i);
                        string = string.substr(i);
                        break;
                    }
                }

                if (is_integer) {
                    if (is_signed) {
                        insert(to_integer<number_integer_t>(value));
                    }
                    else {
                        insert(to_integer<number_unsigned_t>(value));
                    }
                }
                else {
                    insert(to_float<number_float_t>(value));
                }
            }
        }

        return std::move(root);
    }

    // TODO: Improve
    static inline basic_json parse(file& file) {
        return parse(file.read_string());
    }

    static inline basic_json parse(file&& file) {
        return parse(file.read_string());
    }

    // static basic_json parse(std::fstream& stream);

    basic_json() = default;

    explicit basic_json(value_t&& value)
        : _value(std::move(value))
    { }

    explicit basic_json(const object_t& value)
        : _value(value)
    { }

    explicit basic_json(const array_t& value)
        : _value(value)
    { }

    explicit basic_json(const string_t& string)
        : _value(string)
    { }
    
    explicit basic_json(const boolean_t& value)
        : _value(value)
    { }

    explicit basic_json(const number_integer_t& value)
        : _value(value)
    { }

    explicit basic_json(const number_unsigned_t& value)
        : _value(value)
    { }

    explicit basic_json(const number_float_t& value)
        : _value(value)
    { }

    template <typename T>
    basic_json(const T& value)
    {
        if constexpr (is_dict_v<T>) {
            _value = object_t(value);
        }
        else if constexpr (is_list_v<T> or is_view_v<T>) {
            _value = array_t(value);
        }
        else if constexpr (is_string_v<T> or is_string_view_v<T> or is_cstring_v<T>) {
            _value = string_t(value);
        }
        else if constexpr (is_same_v<T, bool>) {
            _value = boolean_t(value);
        }
        else if constexpr (is_signed_v<T>) {
            _value = number_integer_t(value);
        }
        else if constexpr (is_unsigned_v<T>) {
            _value = number_unsigned_t(value);
        }
        else if constexpr (is_floating_point_v<T>) {
            _value = number_float_t(value);
        }
    }

    // basic_json(file& file);
    
    inline constexpr void operator=(const basic_json& other) {
        _value = other._value;
    }
    
    inline constexpr void operator=(const number_integer_t& value) {
        _value = value;
    }
    
    template <typename T>
    inline constexpr void operator=(const initializer_list<T>& values) {
        array_t array;
        for (const auto& v : values) {
            array.emplace_back(v);
        }
        _value = std::move(array);
    }

    // TODO: =boolean

    // // TODO: Investigate
    // template <typename ...Types>
    // struct overloads : Types... { using Types::operator()...; };

    template <typename T>
        requires is_arithmetic_v<T>
    inline constexpr T get() const
    {
        struct visitor
        {
            inline T operator() (const object_t& value) {
                throw runtime_error("no");
            }

            inline T operator() (const array_t& value) {
                throw runtime_error("no");
            }

            inline T operator() (const string_t& value) {
                throw runtime_error("no");
            }

            inline T operator() (const boolean_t& value) {
                return static_cast<T>(value);
            }

            inline T operator() (const number_integer_t& value) {
                return static_cast<T>(value);
            }

            inline T operator() (const number_unsigned_t& value) {
                return static_cast<T>(value);
            }

            inline T operator() (const number_float_t& value) {
                return static_cast<T>(value);
            }
        };

        return _value.visit(visitor{});
    }

    template <typename T>
        requires is_list_v<T>
    inline constexpr T get() const
    {
        T result = {};

        if (std::holds_alternative<array_t>(_value)) {
            for (const auto& v : std::get<array_t>(_value)) {
                result.push_back(v.template get<typename T::value_type>());
            }
        }
        else {
            throw runtime_error("no");
        }

        return result;
    }

    template <typename T>
        requires (not is_arithmetic_v<T> and not is_list_v<T>)
    inline constexpr T get() const {
        return std::get<T>(_value);
    }

    constexpr string_t dump(int indent = -1, char indent_char = ' ') const
    {
        struct visitor
        {
            int current_indent = 0;
            int indent_step;
            char indent_char;

            string_t newline;
            string_t indent_string;

            visitor(const visitor&) = delete;
            visitor& operator=(const visitor&) = delete;

            visitor(char indent_step, char indent_char)
                : indent_step(indent_step)
                , indent_char(indent_char)
            {
                if (pretty()) {
                    newline.push_back('\n');
                }
            }

            inline bool pretty() {
                return (indent_step >= 0);
            }

            inline void indent() {
                if (pretty()) {
                    current_indent += indent_step;
                    indent_string.resize(current_indent, indent_char);
                }
            }

            inline void deindent() {
                if (pretty()) {
                    current_indent -= indent_step;
                    indent_string.resize(current_indent, indent_char);
                }
            }

            string_t operator() (const object_t& values)
            {
                std::string result = format("{{{}", newline);

                indent();

                bool first = true;
                for (const auto& [key, value] : values) {
                    if (!first) {
                        result += format(",{}", newline);
                    }

                    result += indent_string;
                    result += format("{:?}:",key);
                    if (pretty()) {
                        result.push_back(' ');
                    }
                    result += value._value.visit(*this);
                    first = false;
                }

                deindent();

                result += format("{}{}}}", newline, indent_string);

                // Special case
                if (current_indent == 0) {
                    result += newline;
                }

                return result;
            }

            string_t operator() (const array_t& values)
            {
                std::string result = format("[{}", newline);

                indent();

                bool first = true;
                for (const auto& value : values) {
                    if (!first) {
                        result += format(",{}", newline);
                    }

                    result += indent_string;
                    result += value._value.visit(*this);
                    first = false;
                }

                deindent();

                result += format("{}{}]", newline, indent_string);
                return result;
            }

            inline string_t operator() (const string_t& value) {
                return format("{:?}", value);
            }

            inline string_t operator() (const boolean_t& value) {
                return format("{}", value);
            }

            inline string_t operator() (const number_integer_t& value) {
                return format("{}", value);
            }

            inline string_t operator() (const number_unsigned_t& value) {
                return format("{}", value);
            }

            inline string_t operator() (const number_float_t& value) {
                return format("{}", value);
            }
        };

        return _value.visit(visitor(indent, indent_char));
    }

    // pointer functions

    inline constexpr basic_json& at(json_pointer<String> ptr) {
        throw runtime_error("unimplemented");
    }

    inline constexpr basic_json& operator[](json_pointer<String> ptr) {
        throw runtime_error("unimplemented");
    }


    // object functions

    inline constexpr basic_json& at(string_t key) {
        return std::get<object_t>(_value).at(key);
    }

    inline constexpr basic_json& operator[](const string_t& key) {
        return std::get<object_t>(_value)[key];
    }

    // array functions

    inline constexpr basic_json& at(size_t index) {
        return std::get<array_t>(_value).at(index);
    }
    
    inline constexpr basic_json& operator[](size_t index) {
        return std::get<array_t>(_value)[index];
    }

    void push_back(basic_json&& value) {
        std::get<array_t>(_value).push_back(std::move(value));
    }

    void push_back(const basic_json& value) {
        std::get<array_t>(_value).push_back(value);
    }

    // void push_back(basic_json&& value) {
    //     std::get<array_t>(_value).push_back(std::move(value));
    // }


// private:

    value_t _value = object_t();

};

using json = basic_json<>;

using u8json = basic_json<dict, list, u8string>;

// TODO: Use a dict that preserves insertion order
using ordered_json = basic_json<ordered_dict>;

} // namespace slw

#endif // SLW_JSON_HPP
