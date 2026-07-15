#ifndef SLW_JSON_HPP
#define SLW_JSON_HPP

#include <slw/containers.hpp>
#include <slw/integers.hpp>
#include <slw/strings.hpp>
#include <slw/unicode.hpp>

#include <slw/ext/nlohmann/json.hpp>

namespace slw {

using nlohmann::json_pointer;

using json = nlohmann::basic_json<
    ordered_dict, list, string,
    bool, int64_t, uint64_t, double,
    std::allocator, nlohmann::adl_serializer,
    list<uint8_t>
>;

using u8json = nlohmann::basic_json<
    ordered_dict, list, u8string,
    bool, int64_t, uint64_t, double,
    std::allocator, nlohmann::adl_serializer,
    list<uint8_t>>;

// TODO: Use a dict that preserves insertion order
using ordered_json = nlohmann::basic_json<
    nlohmann::ordered_map, list, string,
    bool, int64_t, uint64_t, double,
    std::allocator, nlohmann::adl_serializer,
    list<uint8_t>
>;

} // namespace slw

#endif // SLW_JSON_HPP
