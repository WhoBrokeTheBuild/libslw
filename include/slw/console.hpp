#ifndef SLW_CONSOLE_HPP
#define SLW_CONSOLE_HPP

#include <slw/strings.hpp>

#include <print>

namespace slw {

namespace console {

enum class Color
{
    Default = '*',

    Red     = 'r',
    Green   = 'g',
    Blue    = 'b',
    Cyan    = 'c',
    Magenta = 'm',
    Yellow  = 'y',
    Black   = 'k',
    White   = 'w',

    BrightRed     = 'R',
    BrightGreen   = 'G',
    BrightBlue    = 'B',
    BrightCyan    = 'C',
    BrightMagenta = 'M',
    BrightYellow  = 'Y',
    BrightBlack   = 'K',
    BrightWhite   = 'W',
};

enum class Style
{
    Default   = '*',
    Bold      = '!',
    Underline = '_',
};

constexpr string color(std::string_view format = "")
{
    if (format.empty()) {
        return "\e[0m";
    }

    string code = "\e[";

    Style style = Style(format.back());
    switch (style) {
    case Style::Bold:
        code += "1;";
        break;
    case Style::Underline:
        code += "4;";
        break;
    default:
        code += "0;";
        break;
    }

    Color foreground = Color(format.front());
    switch (foreground) {
    case Color::Black:
        code += "30;";
        break;
    case Color::Red:
        code += "31;";
        break;
    case Color::Green:
        code += "32;";
        break;
    case Color::Yellow:
        code += "33;";
        break;
    case Color::Blue:
        code += "34;";
        break;
    case Color::Magenta:
        code += "35;";
        break;
    case Color::Cyan:
        code += "36;";
        break;
    case Color::White:
        code += "37;";
        break;
    case Color::BrightBlack:
        code += "90;";
        break;
    case Color::BrightRed:
        code += "91;";
        break;
    case Color::BrightGreen:
        code += "92;";
        break;
    case Color::BrightYellow:
        code += "93;";
        break;
    case Color::BrightBlue:
        code += "94;";
        break;
    case Color::BrightMagenta:
        code += "95;";
        break;
    case Color::BrightCyan:
        code += "96;";
        break;
    case Color::BrightWhite:
        code += "97;";
        break;
    default:
        break;
    }

    Color background = Color(format.size() >= 2 ? format[1] : '*');
    switch (background) {
    case Color::Black:
        code += "40;";
        break;
    case Color::Red:
        code += "41;";
        break;
    case Color::Green:
        code += "42;";
        break;
    case Color::Yellow:
        code += "43;";
        break;
    case Color::Blue:
        code += "44;";
        break;
    case Color::Magenta:
        code += "45;";
        break;
    case Color::Cyan:
        code += "46;";
        break;
    case Color::White:
        code += "47;";
        break;
    case Color::BrightBlack:
        code += "100;";
        break;
    case Color::BrightRed:
        code += "101;";
        break;
    case Color::BrightGreen:
        code += "102;";
        break;
    case Color::BrightYellow:
        code += "103;";
        break;
    case Color::BrightBlue:
        code += "104;";
        break;
    case Color::BrightMagenta:
        code += "105;";
        break;
    case Color::BrightCyan:
        code += "106;";
        break;
    case Color::BrightWhite:
        code += "107;";
        break;
    default:
        break;
    }

    code.pop_back();
    code += "m";

    return code;
}

} // namespace console

using std::print;

using std::println;

} // namespace slw

#endif // SLW_CONSOLE_HPP
