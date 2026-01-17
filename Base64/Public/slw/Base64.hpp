#ifndef SLW_BASE64_HPP
#define SLW_BASE64_HPP

#include <span>
#include <string>
#include <vector>
#include <cstdint>

namespace slw {

namespace Base64 {

std::string Encode(std::span<const uint8_t> data);

std::vector<uint8_t> Decode(std::string_view code);

inline std::string Encode(std::span<const char> data) {
    return Encode({ reinterpret_cast<const uint8_t *>(data.data()), data.size() });
}

inline std::string EncodeString(std::string_view data) {
    return Encode({ reinterpret_cast<const uint8_t *>(data.data()), data.size() });
}

inline std::string DecodeString(std::string_view code) {
    const auto& data = Decode(code);
    return { data.begin(), data.end() };
}

} // namespace Base64

} // namespace slw

#endif // SLW_BASE64_HPP