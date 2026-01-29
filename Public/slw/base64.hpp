#ifndef SLW_BASE64_HPP
#define SLW_BASE64_HPP

#include <slw/strings.hpp>
#include <slw/list.hpp>
#include <slw/view.hpp>

#include <cstdint>

namespace slw {

namespace base64 {

string encode(view<const uint8_t> data);

list<uint8_t> decode(string_view code);

inline string encode(view<const char> data) {
    return encode({ reinterpret_cast<const uint8_t *>(data.data()), data.size() });
}

inline string encode_string(string_view data) {
    return encode({ reinterpret_cast<const uint8_t *>(data.data()), data.size() });
}

inline string decode_string(string_view code) {
    const auto& data = decode(code);
    return { data.begin(), data.end() };
}

} // namespace base64

} // namespace slw

#endif // SLW_BASE64_HPP