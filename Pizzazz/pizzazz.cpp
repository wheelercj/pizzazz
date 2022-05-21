#include "pizzazz.h"
#define ESC "\x1b"
#define LESC L"\x1b"

namespace pizzazz {

    void set_cursor_style(CursorStyle style) {
        if (style == CursorStyle::not_hidden)
            std::cout << ESC "[?25h";
        else if (style == CursorStyle::hidden)
            std::cout << ESC "[?25l";
        else if (style == CursorStyle::blinking_default)
            std::cout << ESC "[7 q" ESC "[?12h";
        else if (style == CursorStyle::steady_default)
            std::cout << ESC "[7 q" ESC "[?12l";
        else
            std::cout << "\x1b[" << int(style) << " q";
    }

    void set_window_title(std::string title) {
        std::cout << ESC "]0;" + title + ESC "[";
    }

    void set_window_title(std::wstring title) {
        wprint(LESC "]0;" + title + LESC "[");
    }

    void wprint(std::wstring message) {
#ifdef _WIN32
        fflush(stdout);
        int previous_mode = _setmode(_fileno(stdout), _O_U16TEXT);
        std::wcout << message;
        previous_mode = _setmode(_fileno(stdout), previous_mode);
#else
        std::cout << std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(message);
#endif
    }

    void print_styled(std::string message, std::vector<Style> styles) {
        set_style(styles);
        std::cout << message;
        reset_style();
    }

    void print_styled(std::wstring message, std::vector<Style> styles) {
        set_style(styles);
        wprint(message);
        reset_style();
    }

    void set_style(std::vector<Style> styles) {
        for (Style style : styles)
            std::cout << ESC "[" << int(style) << "m";
    }

    void reset_style() {
        std::cout << "\x1b[0m";
    }

    void print_rgb(unsigned red, unsigned green, unsigned blue, std::string message) {
        set_rgb(red, green, blue);
        std::cout << message;
        reset_style();
    }

    void print_rgb(unsigned red, unsigned green, unsigned blue, std::wstring message) {
        set_rgb(red, green, blue);
        wprint(message);
        reset_style();
    }

    void print_bg_rgb(unsigned red, unsigned green, unsigned blue, std::string message) {
        set_bg_rgb(red, green, blue);
        std::cout << message;
        reset_style();
    }

    void print_bg_rgb(unsigned red, unsigned green, unsigned blue, std::wstring message) {
        set_bg_rgb(red, green, blue);
        wprint(message);
        reset_style();
    }

    void set_rgb(unsigned red, unsigned green, unsigned blue) {
        if (red > 255 || green > 255 || blue > 255)
            throw std::invalid_argument("Error: the values for the colors must be within the range [0,255].");
        std::cout << ESC "[38;2;" << red << ";" << green << ";" << blue << "m";
    }

    void set_bg_rgb(unsigned red, unsigned green, unsigned blue) {
        if (red > 255 || green > 255 || blue > 255)
            throw std::invalid_argument("Error: the values for the colors must be within the range [0,255].");
        std::cout << ESC "[48;2;" << red << ";" << green << ";" << blue << "m";
    }

    void print_at(unsigned x, unsigned y, std::string message) {
        set_cursor_coords(x, y);
        std::cout << message;
    }

    void print_at(unsigned x, unsigned y, std::wstring message) {
        set_cursor_coords(x, y);
        wprint(message);
    }

    void set_cursor_coords(unsigned x, unsigned y) {
        std::cout << ESC "[" << y << ";" << x << "H";
    }

    void set_cursor_coords(Coord coord) {
        std::cout << ESC "[" << coord.y << ";" << coord.x << "H";
    }

    Coord get_cursor_coords() {
        std::cout << ESC "[6n";  // request coordinates in the format \x1b[y;xR
        char input;
        input = getch_();  // \x1b
        input = getch_();  // [
        std::string number;
        while (input != ';') {
            input = getch_();
            if (input >= '0' && input <= '9')
                number.push_back(input);
        }
        Coord coord;
        coord.y = std::stoi(number);
        number = "";
        while (input != 'R') {
            input = getch_();
            if (input >= '0' && input <= '9')
                number.push_back(input);
        }
        coord.x = std::stoi(number);
        return coord;
    }

    void save_cursor_location() {
        std::cout << ESC "[s";
    }

    void restore_cursor_location() {
        std::cout << ESC "[u";
    }

    void move_cursor_up(size_t lines) {
        std::cout << ESC "[" << lines << "A";
    }

    void move_cursor_down(size_t lines) {
        std::cout << ESC "[" << lines << "B";
    }

    void move_cursor_right(size_t columns) {
        std::cout << ESC "[" << columns << "C";
    }

    void move_cursor_left(size_t columns) {
        std::cout << ESC "[" << columns << "D";
    }

    Coord get_window_size() {
#ifdef _WIN32
        HANDLE hStdIn = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hStdIn == INVALID_HANDLE_VALUE)
            throw "Error: Invalid handle value.";
        CONSOLE_SCREEN_BUFFER_INFO buffer_info;
        GetConsoleScreenBufferInfo(hStdIn, &buffer_info);
        int columns = buffer_info.srWindow.Right - buffer_info.srWindow.Left + 1;
        int rows = buffer_info.srWindow.Bottom - buffer_info.srWindow.Top + 1;
        return Coord(columns, rows);
#else
        struct winsize w;
        ioctl(0, TIOCGWINSZ, &w);
        return Coord(w.ws_col, w.ws_row);
#endif
    }

    std::string to_lower(std::string str) {
        for (char& ch : str)
            ch = tolower(ch);
        return str;
    }

    std::string read_key() {
        char input = getch_();
        switch (input) {
        case '\r': return "Enter";
        case '\t': return "Tab";
        case '\x1b': return "Escape";
        case '\b': return "Backspace";
        case '\n': return "Ctrl+Enter";
        case '\x1': return "Ctrl+a";
        case '\x2': return "Ctrl+b";
        case '\x4': return "Ctrl+d";
        case '\x5': return "Ctrl+e";
        case '\x6': return "Ctrl+f";
        case '\a': return "Ctrl+g";
        case '\v': return "Ctrl+k";
        case '\f': return "Ctrl+l";
        case '\xe': return "Ctrl+n";
        case '\xf': return "Ctrl+o";
        case '\x10': return "Ctrl+p";
        case '\x11': return "Ctrl+q";
        case '\x12': return "Ctrl+r";
        case '\x13': return "Ctrl+s";
        case '\x14': return "Ctrl+t";
        case '\x15': return "Ctrl+u";
        case '\x16': return "Ctrl+v";
        case '\x17': return "Ctrl+w";
        case '\x18': return "Ctrl+x";
        case '\x19': return "Ctrl+y";
        case '\x1a': return "Ctrl+z";
        case '\x1c': return "Ctrl+\\";
        case '\x1d': return "Ctrl+]";
        case '': return "Ctrl+Backspace";
        case 'à':
            if (!kbhit__())
                return "à";
            input = getch_();
            switch (input) {
            case 'S': return "Delete";
            case 'H': return "up arrow";
            case 'P': return "down arrow";
            case 'K': return "left arrow";
            case 'M': return "right arrow";
            case 'G': return "Home";
            case 'O': return "End";
            case 'I': return "Page Up";
            case 'Q': return "Page Down";
            case 'R': return "Insert";
            case 's': return "Ctrl+left arrow";
            case 't': return "Ctrl+right arrow";
            case '': return "Ctrl+up arrow";
            case '‘': return "Ctrl+down arrow";
            //case '†': return "Ctrl+Page Up";  // conflicts with F12
            case 'v': return "Ctrl+Page Down";
            case 'w': return "Ctrl+Home";
            case 'u': return "Ctrl+Home";
            case '“': return "Ctrl+Delete";
            case '…': return "F11";
            case '†': return "F12";
            case '‰': return "Ctrl+F11";
            case 'Š': return "Ctrl+F12";
            case '‡': return "Shift+F11";
            case 'ˆ': return "Shift+F12";
            case '‹': return "Alt+F11";
            case 'Œ': return "Alt+F12";
            default:
                throw std::exception("Key not recognized.");
            }
        case '\0':
            if (!kbhit__())
                return "\0";
            input = getch_();
            switch (input) {
            case ';': return "F1";
            case '<': return "F2";
            case '=': return "F3";
            case '>': return "F4";
            case '?': return "F5";
            case '@': return "F6";
            case 'A': return "F7";
            case 'B': return "F8";
            case 'C': return "F9";
            case 'D': return "F10";
            case '”': return "Ctrl+Tab";
            case '^': return "Ctrl+F1";
            case '_': return "Ctrl+F2";
            case '`': return "Ctrl+F3";
            case 'a': return "Ctrl+F4";
            case 'b': return "Ctrl+F5";
            case 'c': return "Ctrl+F6";
            case 'd': return "Ctrl+F7";
            case 'e': return "Ctrl+F8";
            case 'f': return "Ctrl+F9";
            case 'g': return "Ctrl+F10";
            case 'T': return "Shift+F1";
            case 'U': return "Shift+F2";
            case 'V': return "Shift+F3";
            case 'W': return "Shift+F4";
            case 'X': return "Shift+F5";
            case 'Y': return "Shift+F6";
            case 'Z': return "Shift+F7";
            case '[': return "Shift+F8";
            case '\\': return "Shift+F9";
            case ']': return "Shift+F10";
            case '£': return "Alt+Delete";
            case '—': return "Alt+Home";
            case 'Ÿ': return "Alt+End";
            case '™': return "Alt+Page Up";
            case '¡': return "Alt+Page Down";
            case '›': return "Alt+left arrow";
            case '': return "Alt+right arrow";
            case '˜': return "Alt+up arrow";
            case ' ': return "Alt+down arrow";
            case '\x1e': return "Ctrl+Alt+a";
            case '0': return "Ctrl+Alt+b";
            case '.': return "Ctrl+Alt+c";
            case ' ': return "Ctrl+Alt+d";
            case '\x12': return "Ctrl+Alt+e";
            case '!': return "Ctrl+Alt+f";
            case '"': return "Ctrl+Alt+g";
            case '#': return "Ctrl+Alt+h";
            case '\x17': return "Ctrl+Alt+i";
            case '$': return "Ctrl+Alt+j";
            case '%': return "Ctrl+Alt+k";
            case '&': return "Ctrl+Alt+l";
            case '2': return "Ctrl+Alt+m";
            case '1': return "Ctrl+Alt+n";
            case '\x18': return "Ctrl+Alt+o";
            case '\x19': return "Ctrl+Alt+p";
            case '\x10': return "Ctrl+Alt+q";
            case '\x13': return "Ctrl+Alt+r";
            case '\x1f': return "Ctrl+Alt+s";
            case '\x14': return "Ctrl+Alt+t";
            case '\x16': return "Ctrl+Alt+u";
            case '/': return "Ctrl+Alt+v";
            case '\x11': return "Ctrl+Alt+w";
            case '-': return "Ctrl+Alt+x";
            case '\x15': return "Ctrl+Alt+y";
            case ',': return "Ctrl+Alt+z";
            case 'x': return "Ctrl+Alt+1";
            case 'y': return "Ctrl+Alt+2";
            case 'z': return "Ctrl+Alt+3";
            case '{': return "Ctrl+Alt+4";
            case '|': return "Ctrl+Alt+5";
            case '}': return "Ctrl+Alt+6";
            case '~': return "Ctrl+Alt+7";
            case '': return "Ctrl+Alt+8";
            case '€': return "Ctrl+Alt+9";
            case '': return "Ctrl+Alt+0";
            case '‚': return "Ctrl+Alt+-";
            case '(': return "Ctrl+Alt+'";
            case '\'': return "Ctrl+Alt+;";
            case '5': return "Ctrl+Alt+/";
            case '4': return "Ctrl+Alt+.";
            case '3': return "Ctrl+Alt+,";
            case '\x1a': return "Ctrl+Alt+[";
            case '\x1b': return "Ctrl+Alt+]";
            case 'ƒ': return "Ctrl+Alt+=";
            case '\x1c': return "Ctrl+Alt+Enter";
            case '\xe': return "Ctrl+Alt+Backspace";
            case ')': return "Ctrl+Alt+`";
            case 'h': return "Alt+F1";
            case 'i': return "Alt+F2";
            case 'j': return "Alt+F3";
            case 'k': return "Alt+F4";
            case 'l': return "Alt+F5";
            case 'm': return "Alt+F6";
            case 'n': return "Alt+F7";
            case 'o': return "Alt+F8";
            case 'p': return "Alt+F9";
            case 'q': return "Alt+F10";
            default:
                throw std::exception("Key not recognized.");
            }
        default:
            std::string result;
            result.push_back(input);
            return result;
        }
    }

    std::string read_key_if_kbhit() {
        if (kbhit__())
            return read_key();
        return "";
    }

    char getch_() {
#ifdef _WIN32
        char input = _getch();
        if (input == '\x3')
            raise(SIGINT);
        return input;
#else
        struct termios old_attr, new_attr;
        tcgetattr(STDIN_FILENO, &old_attr);
        new_attr = oldattr;
        new_attr.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &new_attr);
        char input = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &old_attr);
        if (input == '\x3')
            raise(SIGINT);
        return input;
#endif
    }

    void _clear_suggestion(
            Coord input_end,
            Coord& suggestion_end,
            std::string& latest_suggestion) {
        set_cursor_coords(input_end);
        delete_chars(suggestion_end.x - input_end.x);
        suggestion_end = input_end;
        latest_suggestion = "";
    }

    void _red_flash_text(std::string text, Coord start, Coord current) {
        set_cursor_coords(start);
        print_styled(text, { Style::red });
        sleep_(300);
        set_cursor_coords(start);
        std::cout << text;
        set_cursor_coords(current);
    }
    
    void _print_suggestion(
            std::string suggestion,
            Coord& suggestion_end,
            size_t input_size) {
        std::string suffix = suggestion.substr(input_size);
        print_styled(suffix, { Style::bright_gray });
        suggestion_end = get_cursor_coords();
    }

    void _find_and_print_suggestion(
            std::string input,
            const std::vector<std::string>& suggestions,
            std::string& latest_suggestion,
            Coord current,
            Coord input_end,
            Coord& suggestion_end,
            bool case_sensitive,
            std::string default_message = "") {
        _clear_suggestion(input_end, suggestion_end, latest_suggestion);
        if (input.empty()) {
            if (default_message.size())
                _print_suggestion(default_message, suggestion_end, input.size());
            set_cursor_coords(current);
            return;
        }
        for (auto it = suggestions.begin(); it != suggestions.end(); it++) {
            std::string suggestion = *it;
            std::string lc_suggestion = suggestion;
            if (!case_sensitive) {
                input = to_lower(input);
                lc_suggestion = to_lower(suggestion);
            }
            if (lc_suggestion.find(input) == 0) {
                latest_suggestion = suggestion;
                _print_suggestion(suggestion, suggestion_end, input.size());
                break;
            }
        }
        set_cursor_coords(current);
    }

    std::string getline_autocompleted(
            const std::vector<std::string>& suggestions,
            std::string default_message,
            bool must_use_suggestion,
            bool case_sensitive) {
        std::string input;
        size_t input_index = 0;
        std::string key = "";
        std::string latest_suggestion = "";
        Coord start = get_cursor_coords();
        Coord current = start;
        Coord input_end = current;
        Coord suggestion_end = current;
        if (default_message.size()) {
            _find_and_print_suggestion(
                input,
                suggestions,
                latest_suggestion,
                current,
                input_end,
                suggestion_end,
                case_sensitive,
                default_message);
            latest_suggestion = default_message;
        }
        while (true) {
            key = read_key();
            if (key == "Enter") {
                if (must_use_suggestion) {
                    if (case_sensitive) {
                        if (latest_suggestion != input)
                            _red_flash_text(input, start, current);
                        else {
                            _clear_suggestion(input_end, suggestion_end, latest_suggestion);
                            return latest_suggestion;
                        }
                    }
                    else if (to_lower(latest_suggestion) != to_lower(input))
                        _red_flash_text(input, start, current);
                    else {
                        _clear_suggestion(input_end, suggestion_end, latest_suggestion);
                        return latest_suggestion;
                    }
                }
                else {
                    _clear_suggestion(input_end, suggestion_end, latest_suggestion);
                    return input;
                }
            }
            else if (key == "Tab") {
                if (latest_suggestion.empty())
                    return input;
                set_cursor_coords(start);
                std::cout << latest_suggestion;
                return latest_suggestion;
            }
            else if (key.size() == 1) {
                if (input_index < input.size())
                    input[input_index] = key[0];
                else {
                    input.append(key);
                    input_end.x += 1;
                }
                std::cout << key;
                current.x += 1;
                input_index += 1;
                _find_and_print_suggestion(
                    input,
                    suggestions,
                    latest_suggestion,
                    current,
                    input_end,
                    suggestion_end,
                    case_sensitive,
                    default_message);
            }
            else if (key == "Backspace" && current.x > start.x && input.size()) {
                input.erase(input_index - 1, 1);
                backspace_chars(1);
                current.x -= 1;
                input_end.x -= 1;
                input_index -= 1;
                _find_and_print_suggestion(
                    input,
                    suggestions,
                    latest_suggestion,
                    current,
                    input_end,
                    suggestion_end,
                    case_sensitive,
                    default_message);
            }
            else if (key == "Ctrl+Backspace" && current.x > start.x && input.size()) {
                backspace_chars(input_index);
                input = input.substr(input_index);
                if (latest_suggestion.size())
                    suggestion_end.x -= int(input_index);
                current = start;
                set_cursor_coords(current);
                input_end = start;
                input_index = 0;
                input = "";
                _find_and_print_suggestion(
                    input,
                    suggestions,
                    latest_suggestion,
                    current,
                    input_end,
                    suggestion_end,
                    case_sensitive,
                    default_message);
            }
            else if (key == "Delete" && current.x < input_end.x) {
                delete_chars(1);
                input_end.x -= 1;
                input.erase(input_index, 1);
                _find_and_print_suggestion(
                    input,
                    suggestions,
                    latest_suggestion,
                    current,
                    input_end,
                    suggestion_end,
                    case_sensitive,
                    default_message);
            }
            else if (key == "Ctrl+Delete") {
                delete_chars(suggestion_end.x - current.x);
                input = input.substr(0, input_index);
                input_end.x = current.x;
                _find_and_print_suggestion(
                    input,
                    suggestions,
                    latest_suggestion,
                    current,
                    input_end,
                    suggestion_end,
                    case_sensitive,
                    default_message);
            }
            else if (key == "left arrow" && current.x > start.x) {
                current.x -= 1;
                input_index -= 1;
                set_cursor_coords(current);
            }
            else if (key == "right arrow" && current.x < input_end.x) {
                current.x += 1;
                input_index += 1;
                set_cursor_coords(current);
            }
            //else if (key == "up arrow") {
            //    // TODO
            //}
            //else if (key == "down arrow") {
            //    // TODO
            //}
            else if (key == "Home") {
                current = start;
                input_index = 0;
                set_cursor_coords(current);
            }
            else if (key == "End") {
                current = input_end;
                input_index = input.size() - 1;
                set_cursor_coords(current);
            }
            //else if (key == "Ctrl+left arrow") {
            //    // TODO
            //}
            //else if (key == "Ctrl+right arrow") {
            //    // TODO
            //}
        }
    }

#ifndef _WIN32
    bool _POSIX_kbhit() {
        int bytes_waiting;
        ioctl(0, FIONREAD, &bytes_waiting);
        return bytes_waiting > 0;
    }

    void _enable_raw_mode() {
        termios term;
        tcgetattr(0, &term);
        term.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(0, TCSANOW, &term);
    }

    void _disable_raw_mode() {
        termios term;
        tcgetattr(0, &term);
        term.c_lflag |= ICANON | ECHO;
        tcsetattr(0, TCSANOW, &term);
    }
#endif

    bool kbhit__() {
#ifdef _WIN32
        return _kbhit();
#else
        _enable_raw_mode();
        bool hit = _POSIX_kbhit();
        _disable_raw_mode();
        tcflush(0, TCIFLUSH);
        return hit;
#endif
    }

    char getch_if_kbhit() {
        if (kbhit__())
            return getch_();
        return 0;
    }

    void pause() {
        char _ = getch_();
        while (kbhit__())  // some keys are multiple characters
            _ = getch_();
    }

    void insert(std::string text) {
        std::cout << ESC "[" << text.size() << "@";
        std::cout << ESC "[" << text.size() << "D";
        std::cout << text;
    }

    void insert(std::wstring text) {
        std::cout << ESC "[" << text.size() << "@";
        std::cout << ESC "[" << text.size() << "D";
        wprint(text);
    }

    void delete_chars(size_t count) {
        std::cout << ESC "[" << count << "P";
    }

    void backspace_chars(size_t count) {
        move_cursor_left(count);
        delete_chars(count);
    }

    void insert_lines(size_t count) {
        std::cout << ESC "[" << count << "L";
    }

    void delete_lines(size_t count) {
        std::cout << ESC "[" << count << "M";
    }

    void clear_screen() {
        std::cout << ESC "[2J";
    }

    void alternate_screen_buffer() {
        std::cout << ESC "[?1049h";
    }

    void restore_screen_buffer() {
        std::cout << ESC "[?1049l";
    }

    void sleep_(unsigned milliseconds) {
#ifdef _WIN32
        Sleep(milliseconds);
#else
        usleep(milliseconds * 1000);
#endif
    }

    void set_window_width_to_132() {
        std::cout << ESC "[?3h";
    }

    void set_window_width_to_80() {
        std::cout << ESC "[?3l";
    }
}
