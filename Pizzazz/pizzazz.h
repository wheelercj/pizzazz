﻿/*
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
#include <stdexcept>
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

    /* Hides or shows the terminal's cursor. */
    void show_cursor(bool = true);
    
    /* Hides or shows the terminal cursor's blinking. */
    void show_cursor_blink(bool = true);
    
    /* Changes the terminal window's title.
       Doesn't work with Windows Terminal.
    */
    void set_window_title(std::string title);

    /* Changes the terminal window's title.
       The new title can include any emoji/Unicode characters.
       Doesn't work with Windows Terminal.
    */
    void set_window_title(std::wstring title);

    /*  Prints a string that can have emoji/Unicode characters.
        When calling this function, put an L in front of the string.
        For example, `wprint(L"Hi! ✨");` prints `Hi! ✨`.
        Microsoft's C++ compiler doesn't support printing Unicode
        characters with code points greater than U+FFFF.
    */
    void wprint(std::wstring message);
    
    /* Prints a message with style such as as color, italics, underline, etc. */
    void print_styled(Style style, std::string message);
    
    /* Prints a message with style such as as color, italics, underline, etc. */
    void print_styled(Style style, std::wstring message);
    
    /* Prints a message with styles such as as color, italics, underline, etc. */
    void print_styled(std::vector<Style> styles, std::string message);
    
    /* Prints a message with styles such as as color, italics, underline, etc. */
    void print_styled(std::vector<Style> styles, std::wstring message);
    
    /* Sets future output to have a style such as as color, italics, underline, etc. */
    void set_style(Style style);
    
    /* Sets future output to have styles such as as color, italics, underline, etc. */
    void set_style(std::vector<Style> styles);
    
    /* Sets future output to have styles such as as color, italics, underline, etc. */
    void set_style(std::string ansi_code_after_esc);
    
    /* Sets future output to NOT have any special styles. */
    void reset_style();

    /* Prints a message with a chosen color.
       Throws std::invalid_argument if a color value is greater than 255.
       The red, green, and blue variables must be in the range [0, 255].
    */
    void print_rgb(unsigned red, unsigned green, unsigned blue, std::string message);

    /* Prints a message with a chosen color.
       The message can have emoji/Unicode characters.
       Throws std::invalid_argument if a color value is greater than 255.
    */
    void print_rgb(unsigned red, unsigned green, unsigned blue, std::wstring message);

    /* Prints a message with a chosen background color.
       Throws std::invalid_argument if a color value is greater than 255.
    */
    void print_bg_rgb(unsigned red, unsigned green, unsigned blue, std::string message);

    /* Prints a message with a chosen background color.
       The message can have emoji/Unicode characters.
       Throws std::invalid_argument if a color value is greater than 255.
    */
    void print_bg_rgb(unsigned red, unsigned green, unsigned blue, std::wstring message);
    
    /* Sets future output to have a chosen color. */
    void set_rgb(unsigned red, unsigned green, unsigned blue);
    
    /* Sets future output to have a chosen background color. */
    void set_bg_rgb(unsigned red, unsigned green, unsigned blue);

    /* Prints a message at chosen coordinates. */
    void print_at(unsigned x, unsigned y, std::string message);

    /* Prints a message at chosen coordinates.
       The message can have emoji/Unicode characters.
    */
    void print_at(unsigned x, unsigned y, std::wstring message);

    /* Moves the terminal's cursor to chosen coordinates.
       In this coordinate system, x increases to the right
       and y increases downwards.
    */
    void set_cursor_coords(unsigned x, unsigned y);

    /* Moves the terminal's cursor to chosen coordinates.
       In this coordinate system, x increases to the right
       and y increases downwards.
    */
    void set_cursor_coords(Coord coord);
    
    /* Detects the current coordinates of the terminal's cursor. */
    Coord get_cursor_coords();
    
    /* Saves the terminal cursor's current location to be restored later. */
    void save_cursor_location();
    
    /* Restores the terminal cursor to a previously saved location. */
    void restore_cursor_location();
    
    /* Moves the terminal cursor up a chosen number of lines.
       If the cursor is at the top of the window, this function will
       have no effect.
    */
    void move_cursor_up(unsigned = 1);
    
    /* Moves the terminal cursor down a chosen number of lines.
       If the cursor is at the bottom of the window, this function will
       have no effect.
    */
    void move_cursor_down(unsigned = 1);
    
    /* Moves the terminal cursor to the right a chosen number of columns.
       If the cursor is at the right edge of the window, this function will
       have no effect.
    */
    void move_cursor_right(unsigned = 1);
    
    /* Moves the terminal cursor to the left a chosen number of columns.
       If the cursor is at the left edge of the window, this function will
       have no effect.
    */
    void move_cursor_left(unsigned = 1);
    
    /* Detects the terminal window's current size in columns and rows. */
    Coord get_window_size();

    /* Waits for and gets a keypress of input without requiring an Enter keypress.
       Ignores modifier keys. Some return values may be unexpected:
       Enter -> "Enter",
       Tab -> "Tab",
       Escape -> "Escape",
       Backspace -> "Backspace",
       Delete -> "Delete",
       up arrow -> "up arrow",
       down arrow -> "down arrow",
       left arrow -> "left arrow",
       right arrow -> "right arrow",
       Home -> "Home",
       End -> "End",
       Page Up -> "Page Up",
       Page Down -> "Page Down",
       Insert -> "Insert",
       F1 -> "F1",
       F2 -> "F2",
       F3 -> "F3",
       F4 -> "F4",
       F5 -> "F5",
       F6 -> "F6",
       F7 -> "F7",
       F8 -> "F8",
       F9 -> "F9",
       F10 -> "F10",
       F11 -> "F11",
       F12 -> "F12".
    */
    std::string read_key();

    /* Waits for and gets a character input without requiring an Enter keypress.
       Ignores modifier keys. Some keys return values that may be unexpected:
       Enter -> '\r',
       Tab -> '\t',
       Escape -> '\x1b',
       Backspace -> '\b',
       Delete -> 'à' and then 'S',
       up arrow -> 'à' and then 'H',
       down arrow -> 'à' and then 'P',
       left arrow -> 'à' and then 'K',
       right arrow -> 'à' and then 'M',
       Home -> 'à' and then 'G',
       End -> 'à' and then 'O',
       Page Up -> 'à' and then 'I',
       Page Down -> 'à' and then 'Q',
       Insert -> 'à' and then 'R',
       F1 -> '\0' and then ';',
       F2 -> '\0' and then '<',
       F3 -> '\0' and then '=',
       F4 -> '\0' and then '>',
       F5 -> '\0' and then '?',
       F6 -> '\0' and then the at symbol,
       F7 -> '\0' and then 'A',
       F8 -> '\0' and then 'B',
       F9 -> '\0' and then 'C',
       F10 -> '\0' and then 'D',
       F11 -> 'à' and then '…',
       F12 -> 'à' and then '†'.
    */
    char getch_();

    /* Detects whether a key was pressed.
       Ignores modifier keys.
    */
    bool kbhit__();

    /* Gets a character input without requiring an Enter keypress only if there is input.
       Returns 0 if there is no input.
       Ignores modifier keys.
    */
    char getch_if_kbhit();

    /* Pauses the program until the user presses a key.
       Modifier keys are ignored.
    */
    void pause();

#ifndef _WIN32
    
    void enable_raw_mode();
    
    void disable_raw_mode();
    
    /* Detects whether a key has been pressed. */
    bool kbhit_();
#endif
    
    /* Inserts text at the terminal cursor's current location. */
    void insert(std::string text);

    /* Inserts text at the terminal cursor's current location.
       The text can have emoji/Unicode characters.
    */
    void insert(std::wstring text);

    /* Deletes a chosen number of characters at the terminal cursor's current location.
       All characters to the right of what is deleted move to the left.
    */
    void delete_chars(unsigned count);
    
    /* Inserts empty lines at the terminal cursor's current location. */
    void insert_lines(unsigned count);

    /* Deletes a chosen number of lines at the terminal cursor's current location.
       All characters below what is deleted move up.
    */
    void delete_lines(unsigned count);

    /* Deletes all characters in the terminal window.
       Use of this function is not recommended without using an alternate screen buffer.
    */
    void clear_screen();
    
    /* Switches the terminal app to a different "window" in the terminal. */
    void alternate_screen_buffer();
    
    /* Restores the terminal app's original "window". */
    void restore_screen_buffer();

    /* Pauses the program for a chosen amount of time.
       1 second is 1000 milliseconds.
    */
    void sleep_(unsigned milliseconds);
    
    void set_window_width_to_132();
    
    void set_window_width_to_80();
}
