#include <slw/String.hpp>

namespace slw {

std::vector<std::string_view> String::Split(std::string_view string, std::string_view separator)
{
    std::vector<std::string_view> parts;

    auto it = string.find(separator);
    while (it != std::string_view::npos) {
        parts.emplace_back(string.substr(0, it));
        string = string.substr(it + separator.size());
        it = string.find(separator);
    }

    parts.push_back(string);
    return parts;
}

std::string String::Join(const std::vector<std::string_view>& parts, std::string_view separator)
{
    if (parts.empty()) {
        return {};
    }

    size_t size = (parts.size() - 1) * separator.size();
    for (const auto& part : parts) {
        size += part.size();
    }

    std::string result;
    result.reserve(size);

    for (size_t i = 0; i < parts.size() - 1; ++i) {
        result.append(parts[i]);
        result.append(separator);
    }

    result.append(parts.back());

    return result;
}

} // namespace slw