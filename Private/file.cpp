#include <slw/io/file.hpp>

namespace slw {

string file::read_line()
{
    string line;

    for (;;) {
        int c = std::fgetc(_file);
        if (c == EOF) {
            if (line.empty()) {
                throw last_system_error();
            }

            break;
        }

        line.push_back(c);
        if (c == '\n') {
            break;
        }
    }

    return line;
}

list<string> file::read_lines()
{
    list<string> lines;

    const auto& text = read_string();
    string_view view(text);

    auto it = view.find('\n');
    while (!view.empty() && it != string::npos) {
        lines.emplace_back(view.substr(0, it + 1));
        view = view.substr(it + 1);
        it = view.find('\n');
    }

    if (!view.empty()) {
        lines.emplace_back(view);
    }

    return lines;
}

} // namespace slw
