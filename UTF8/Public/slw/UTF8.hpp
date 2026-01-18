#ifndef SLW_UTF8_HPP
#define SLW_UTF8_HPP

#include <string_view>
#include <stdexcept>

namespace slw {

namespace UTF8 {

class InvalidSequence : public std::runtime_error
{
public:

    InvalidSequence()
        : std::runtime_error("Invalid UTF-8 byte sequence")
    { }

}; // class InvalidSequence

size_t Length(std::u8string_view string);

} // namespace UTF8

} // namespace slw

#endif // SLW_UTF8_HPP