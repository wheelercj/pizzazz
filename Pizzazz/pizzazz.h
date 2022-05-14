/*
    MIT License

    Copyright (c) 2022 Chris Wheeler

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#include <iostream>
#include <string>
#include <vector>
#include <wchar.h>
#ifdef _WIN32
#include <windows.h>
#include <conio.h>  // _getch
#include <io.h>  // _setmode
#include <fcntl.h>  // _O_U16TEXT
#else
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <locale>
#include <codecvt>  // wstring_convert, codecvt_utf8
#endif
#define ESC "\x1b"
#define LESC L"\x1b"

const std::string VERSION = "0.1.0";

// Here are some resources that were helpful for writing this file:
// https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences
// https://docs.microsoft.com/en-us/windows/console/reading-input-buffer-events
// https://docs.microsoft.com/en-us/windows/console/mouse-event-record-str
// https://en.wikipedia.org/wiki/ANSI_escape_code#SGR_(Select_Graphic_Rendition)_parameters
// https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#main

namespace pizzazz {

    struct Coord {
        int x;
        int y;
        Coord() {
            x = 0;
            y = 0;
        }
        Coord(int new_x, int new_y) {
            x = new_x;
            y = new_y;
        }
    };

    enum class Style {
        reset,
        bold,
        dim,
        italic,
        underlined,
        slow_blink,
        fast_blink,
        reverse,
        hidden,
        strikethrough,
        double_underlined = 21,
        black = 30,
        red,
        green,
        yellow,
        blue,
        magenta,
        cyan,
        reset_color = 39,
        bg_black,
        bg_red,
        bg_green,
        bg_yellow,
        bg_blue,
        bg_magenta,
        bg_cyan,
        bg_white,
        reset_bg_color = 49,
        overlined = 53,
        bright_gray = 90,
        bright_red,
        bright_green,
        bright_yellow,
        bright_blue,
        bright_magenta,
        bright_cyan,
        bright_white,
        bright_bg_gray = 100,
        bright_bg_red,
        bright_bg_green,
        bright_bg_yellow,
        bright_bg_blue,
        bright_bg_magenta,
        bright_bg_cyan,
        bright_bg_white,
    };

    void toggle_showing_cursor(bool choice);
    void toggle_showing_cursor_blink(bool choice);
    void set_window_title(std::string title);
    void set_window_title(std::wstring title);
    void wprint(std::wstring message);
    void print_styled(Style style, std::string message);
    void print_styled(Style style, std::wstring message);
    void print_styled(std::vector<Style> styles, std::string message);
    void print_styled(std::vector<Style> styles, std::wstring message);
    void set_style(Style style);
    void set_style(std::vector<Style> styles);
    void set_style(std::string ansi_code_after_esc);
    void reset_style();
    void print_rgb(int red, int green, int blue, std::string message);
    void print_rgb(int red, int green, int blue, std::wstring message);
    void print_bg_rgb(int red, int green, int blue, std::string message);
    void print_bg_rgb(int red, int green, int blue, std::wstring message);
    void print_at(unsigned x, unsigned y, std::string message);
    void print_at(unsigned x, unsigned y, std::wstring message);
    void set_cursor_coords(unsigned x, unsigned y);
    Coord get_cursor_coords();
    void save_cursor_position();
    void restore_cursor_position();
    void move_cursor_up(int lines);
    void move_cursor_down(int lines);
    void move_cursor_right(int columns);
    void move_cursor_left(int columns);
    Coord get_window_size();
    char getch_();
    void insert(std::string text);
    void insert(std::wstring text);
    void delete_chars(int count);
    void insert_lines(int count);
    void delete_lines(int count);
    void alternate_screen_buffer();
    void restore_screen_buffer();
    void sleep_(int milliseconds);
    void set_window_width_to_132();
    void set_window_width_to_80();
    void clear_screen();

    void toggle_showing_cursor(bool choice) {
        if (choice)
            std::cout << ESC "[?25h";
        else
            std::cout << ESC "[?25l";
    }

    void toggle_showing_cursor_blink(bool choice) {
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
        /*  Prints a string that can have emoji/Unicode characters.
            When calling this function, put an L in front of the string.
            For example, `wprint(L"Hi! ✨");` prints `Hi! ✨`.
        */
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

    void print_rgb(int red, int green, int blue, std::string message) {
        std::cout << ESC "[38;2;" << red << ";" << green << ";" << blue << "m";
        std::cout << message;
        reset_style();
    }

    void print_rgb(int red, int green, int blue, std::wstring message) {
        std::cout << ESC "[38;2;" << red << ";" << green << ";" << blue << "m";
        wprint(message);
        reset_style();
    }

    void print_bg_rgb(int red, int green, int blue, std::string message) {
        std::cout << ESC "[48;2;" << red << ";" << green << ";" << blue << "m";
        std::cout << message;
        reset_style();
    }

    void print_bg_rgb(int red, int green, int blue, std::wstring message) {
        std::cout << ESC "[48;2;" << red << ";" << green << ";" << blue << "m";
        wprint(message);
        reset_style();
    }

    void print_at(unsigned x, unsigned y, std::string message) {
        /* Prints a message at chosen coordinates. */
        set_cursor_coords(x, y);
        std::cout << message;
    }

    void print_at(unsigned x, unsigned y, std::wstring message) {
        set_cursor_coords(x, y);
        wprint(message);
    }

    void set_cursor_coords(unsigned x, unsigned y) {
        /* Moves the terminal's cursor to chosen coordinates.
           In this coordinate system, x increases to the right
           and y increases downwards.
        */
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

    void save_cursor_position() {
        std::cout << ESC "[s";
    }

    void restore_cursor_position() {
        std::cout << ESC "[u";
    }

    void move_cursor_up(int lines = 1) {
        std::cout << ESC "[" << lines << "A";
    }

    void move_cursor_down(int lines = 1) {
        std::cout << ESC "[" << lines << "B";
    }

    void move_cursor_right(int columns = 1) {
        std::cout << ESC "[" << columns << "C";
    }

    void move_cursor_left(int columns = 1) {
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
        /* Waits for and gets a character input without an Enter keypress. */
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
        /* Gets a character input without an Enter keypress only if there is input.
           Returns 0 if there is no input.
        */
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

    void delete_chars(int count) {
        std::cout << ESC "[" << count << "P";
    }

    void insert_lines(int count) {
        std::cout << ESC "[" << count << "L";
    }

    void delete_lines(int count) {
        std::cout << ESC "[" << count << "M";
    }

    void alternate_screen_buffer() {
        std::cout << ESC "[?1049h";
    }

    void restore_screen_buffer() {
        std::cout << ESC "[?1049l";
    }

    void sleep_(int milliseconds) {
        /* Pauses the program for a chosen amount of time. */
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

    void clear_screen() {
        std::cout << ESC "[2J";
    }
}
