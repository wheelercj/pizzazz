#include "pizzazz.h"
#define ESC "\x1b"
#define LESC L"\x1b"

namespace pizzazz {

    void show_cursor(bool choice) {
        if (choice)
            std::cout << ESC "[?25h";
        else
            std::cout << ESC "[?25l";
    }

    void show_cursor_blink(bool choice) {
        if (choice)
            std::cout << ESC "[?12h";
        else
            std::cout << ESC "[?12l";
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

    void print_styled(Style style, std::string message) {
        set_style(style);
        std::cout << message;
        reset_style();
    }

    void print_styled(Style style, std::wstring message) {
        set_style(style);
        wprint(message);
        reset_style();
    }

    void print_styled(std::vector<Style> styles, std::string message) {
        set_style(styles);
        std::cout << message;
        reset_style();
    }

    void print_styled(std::vector<Style> styles, std::wstring message) {
        set_style(styles);
        wprint(message);
        reset_style();
    }

    void set_style(Style style) {
        std::cout << ESC "[" << int(style) << "m";
    }

    void set_style(std::vector<Style> styles) {
        for (Style style : styles)
            std::cout << ESC "[" << int(style) << "m";
    }

    void set_style(std::string ansi_code_after_esc) {
        std::cout << ESC + ansi_code_after_esc;
    }

    void reset_style() {
        std::cout << "\x1b[0m";
    }

    void print_rgb(unsigned red, unsigned green, unsigned blue, std::string message) {
        std::cout << ESC "[38;2;" << red << ";" << green << ";" << blue << "m";
        std::cout << message;
        reset_style();
    }

    void print_rgb(unsigned red, unsigned green, unsigned blue, std::wstring message) {
        std::cout << ESC "[38;2;" << red << ";" << green << ";" << blue << "m";
        wprint(message);
        reset_style();
    }

    void print_bg_rgb(unsigned red, unsigned green, unsigned blue, std::string message) {
        std::cout << ESC "[48;2;" << red << ";" << green << ";" << blue << "m";
        std::cout << message;
        reset_style();
    }

    void print_bg_rgb(unsigned red, unsigned green, unsigned blue, std::wstring message) {
        std::cout << ESC "[48;2;" << red << ";" << green << ";" << blue << "m";
        wprint(message);
        reset_style();
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

    void move_cursor_up(unsigned lines) {
        std::cout << ESC "[" << lines << "A";
    }

    void move_cursor_down(unsigned lines) {
        std::cout << ESC "[" << lines << "B";
    }

    void move_cursor_right(unsigned columns) {
        std::cout << ESC "[" << columns << "C";
    }

    void move_cursor_left(unsigned columns) {
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

    char getch_() {
#ifdef _WIN32
        return _getch();
#else
        struct termios old_attr, new_attr;
        tcgetattr(STDIN_FILENO, &old_attr);
        new_attr = oldattr;
        new_attr.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &new_attr);
        char ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &old_attr);
        return ch;
#endif
    }

    char getch_if_kbhit() {
#ifdef _WIN32
        if (_kbhit())
#else
        if (kbhit_())
#endif
            return getch_();
        return 0;
    }

#ifndef _WIN32
    void enable_raw_mode() {
        termios term;
        tcgetattr(0, &term);
        term.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(0, TCSANOW, &term);
    }

    void disable_raw_mode() {
        termios term;
        tcgetattr(0, &term);
        term.c_lflag |= ICANON | ECHO;
        tcsetattr(0, TCSANOW, &term);
    }

    bool kbhit_() {
        int bytes_waiting;
        ioctl(0, FIONREAD, &bytes_waiting);
        return bytes_waiting > 0;
    }
#endif

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

    void delete_chars(unsigned count) {
        std::cout << ESC "[" << count << "P";
    }

    void insert_lines(unsigned count) {
        std::cout << ESC "[" << count << "L";
    }

    void delete_lines(unsigned count) {
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
