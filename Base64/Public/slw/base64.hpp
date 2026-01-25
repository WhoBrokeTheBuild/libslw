#ifndef SLW_BASE64_HPP
#define SLW_BASE64_HPP

#include <span>
#include <string>
#include <vector>

#include <cstdint>

namespace slw {

namespace base64 {

std::string encode(std::span<const uint8_t> data);

std::vector<uint8_t> decode(std::string_view code);

inline std::string encode(std::span<const char> data) {
    return encode({ reinterpret_cast<const uint8_t *>(data.data()), data.size() });
}

inline std::string encode_string(std::string_view data) {
    return encode({ reinterpret_cast<const uint8_t *>(data.data()), data.size() });
}

inline std::string decode_string(std::string_view code) {
    const auto& data = decode(code);
    return { data.begin(), data.end() };
}

} // namespace base64

} // namespace slw

#endif // SLW_BASE64_HPP