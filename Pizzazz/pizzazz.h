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

const std::string __version = "0.2.0";

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

    /* Values to pass to functions that control the terminal's style.
       Some styles don't work in all terminals.
    */
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

    void show_cursor(bool = true);
    void show_cursor_blink(bool = true);
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
    void print_rgb(unsigned red, unsigned green, unsigned blue, std::string message);
    void print_rgb(unsigned red, unsigned green, unsigned blue, std::wstring message);
    void print_bg_rgb(unsigned red, unsigned green, unsigned blue, std::string message);
    void print_bg_rgb(unsigned red, unsigned green, unsigned blue, std::wstring message);
    void print_at(unsigned x, unsigned y, std::string message);
    void print_at(unsigned x, unsigned y, std::wstring message);
    void set_cursor_coords(unsigned x, unsigned y);
    Coord get_cursor_coords();
    void save_cursor_location();
    void restore_cursor_location();
    void move_cursor_up(unsigned = 1);
    void move_cursor_down(unsigned = 1);
    void move_cursor_right(unsigned = 1);
    void move_cursor_left(unsigned = 1);
    Coord get_window_size();
    char getch_();
    char getch_if_kbhit();
#ifndef _WIN32
    void enable_raw_mode();
    void disable_raw_mode();
    bool kbhit_();
#endif
    void insert(std::string text);
    void insert(std::wstring text);
    void delete_chars(unsigned count);
    void insert_lines(unsigned count);
    void delete_lines(unsigned count);
    void clear_screen();
    void alternate_screen_buffer();
    void restore_screen_buffer();
    void sleep_(unsigned milliseconds);
    void set_window_width_to_132();
    void set_window_width_to_80();
}
