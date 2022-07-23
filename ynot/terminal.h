#pragma once
#include <iostream>
#include <syncstream>  // C++20+
#include <string>
#include <vector>
#include <wchar.h>
#include <stdexcept>
#include <csignal>  // raise, SIGINT
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

// Here are some resources that were helpful for writing this file:
// https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences
// https://docs.microsoft.com/en-us/windows/console/reading-input-buffer-events
// https://docs.microsoft.com/en-us/windows/console/mouse-event-record-str
// https://en.wikipedia.org/wiki/ANSI_escape_code#SGR_(Select_Graphic_Rendition)_parameters
// https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#main
// http://xn--rpa.cc/irl/term.html

namespace ynot
{

    struct Coord
    {
        int x;
        int y;
        Coord()
        {
            x = 0;
            y = 0;
        }
        Coord(int new_x, int new_y)
        {
            x = new_x;
            y = new_y;
        }
    };

    /* Values to pass to a function to control the cursor's appearance. */
    enum class CursorStyle
    {
        blinking_block = 1,
        steady_block,
        blinking_underline,
        steady_underline,
        blinking_i_beam,
        steady_i_beam,
        blinking_default,
        steady_default,
        hidden,
        not_hidden
    };

    /* Values to pass to functions that control the terminal's text style.
       Some styles don't work in all terminals. */
    enum class Style
    {
        reset_all,
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
        normal_intensity,
        not_italic,
        not_underlined,
        not_blinking,
        not_reversed = 27,
        not_hidden,
        not_strikethrough,
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
        framed = 51,
        encircled,
        overlined,
        not_framed_or_encircled,
        not_overlined,
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

    /* Changes the terminal cursor's appearance. */
    void set_cursor_style(CursorStyle style, std::ostream& stream = std::cout);

    /* Changes the terminal cursor's appearance. */
    void w_set_cursor_style(CursorStyle style, std::wostream& wstream = std::wcout);

    /* Changes the terminal window's title.
       Doesn't work with Windows Terminal. */
    void set_window_title(std::string title, std::ostream& stream = std::cout);

    /* Changes the terminal window's title.
       The new title can include any emoji/Unicode characters.
       Doesn't work with Windows Terminal. */
    void w_set_window_title(std::wstring title, std::wostream& wstream = std::wcout);

    /*  Prints a string that can have emoji/Unicode characters.
        When calling this function, put an L in front of the string.
        For example, `wprint(L"Hi! ✨");` prints `Hi! ✨`.
        Microsoft's C++ compiler doesn't support printing Unicode
        characters with code points greater than U+FFFF. */
    void w_print(std::wstring message, std::wostream& wstream = std::wcout);

    /* Prints a message with style such as as color, italics, underline, etc. */
    void print_styled(std::string message, std::vector<Style> styles, std::ostream& stream = std::cout);

    /* Prints a message with style such as as color, italics, underline, etc. */
    void w_print_styled(std::wstring message, std::vector<Style> styles, std::wostream& wstream = std::wcout);

    /* Sets future output to have a style such as as color, italics, underline, etc. */
    void set_style(std::vector<Style> styles, std::ostream& stream = std::cout);

    /* Sets future output to have a style such as as color, italics, underline, etc. */
    void w_set_style(std::vector<Style> styles, std::wostream& wstream = std::wcout);

    /* Sets future output to NOT have any special styles. */
    void reset_style(std::ostream& stream = std::cout);

    /* Sets future output to NOT have any special styles. */
    void w_reset_style(std::wostream& wstream = std::wcout);

    /* Prints a message with a chosen color.
       Throws std::invalid_argument if a color value is greater than 255.
       The red, green, and blue variables must be in the range [0, 255]. */
    void print_rgb(unsigned red, unsigned green, unsigned blue, std::string message, std::ostream& stream = std::cout);

    /* Prints a message with a chosen color.
       The message can have emoji/Unicode characters.
       Throws std::invalid_argument if a color value is greater than 255. */
    void w_print_rgb(unsigned red, unsigned green, unsigned blue, std::wstring message, std::wostream& wstream = std::wcout);

    /* Prints a message with a chosen background color.
       Throws std::invalid_argument if a color value is greater than 255. */
    void print_bg_rgb(unsigned red, unsigned green, unsigned blue, std::string message, std::ostream& stream = std::cout);

    /* Prints a message with a chosen background color.
       The message can have emoji/Unicode characters.
       Throws std::invalid_argument if a color value is greater than 255. */
    void w_print_bg_rgb(unsigned red, unsigned green, unsigned blue, std::wstring message, std::wostream& wstream = std::wcout);

    /* Sets future output to have a chosen color. */
    void set_rgb(unsigned red, unsigned green, unsigned blue, std::ostream& stream = std::cout);

    /* Sets future output to have a chosen color. */
    void w_set_rgb(unsigned red, unsigned green, unsigned blue, std::wostream& wstream = std::wcout);

    /* Sets future output to have a chosen background color. */
    void set_bg_rgb(unsigned red, unsigned green, unsigned blue, std::ostream& stream = std::cout);

    /* Sets future output to have a chosen background color. */
    void w_set_bg_rgb(unsigned red, unsigned green, unsigned blue, std::wostream& wstream = std::wcout);

    /* Prints a message at chosen coordinates. */
    void print_at(unsigned x, unsigned y, std::string message, std::ostream& stream = std::cout);

    /* Prints a message at chosen coordinates.
       The message can have emoji/Unicode characters. */
    void w_print_at(unsigned x, unsigned y, std::wstring message, std::wostream& wstream = std::wcout);

    /* Moves the terminal's cursor to chosen coordinates.
       In this coordinate system, x increases to the right
       and y increases downwards. */
    void set_cursor_coords(unsigned x, unsigned y, std::ostream& stream = std::cout);

    /* Moves the terminal's cursor to chosen coordinates.
       In this coordinate system, x increases to the right
       and y increases downwards. */
    void w_set_cursor_coords(unsigned x, unsigned y, std::wostream& wstream = std::wcout);

    /* Moves the terminal's cursor to chosen coordinates.
       In this coordinate system, x increases to the right
       and y increases downwards. */
    void set_cursor_coords(Coord coord, std::ostream& stream = std::cout);

    /* Moves the terminal's cursor to chosen coordinates.
       In this coordinate system, x increases to the right
       and y increases downwards. */
    void w_set_cursor_coords(Coord coord, std::wostream& wstream = std::wcout);

    /* Detects the current coordinates of the terminal's cursor. */
    Coord get_cursor_coords(std::ostream& stream = std::cout);

    /* Detects the current coordinates of the terminal's cursor. */
    Coord w_get_cursor_coords(std::wostream& wstream = std::wcout);

    /* Saves the terminal cursor's current location to be restored later. */
    void save_cursor_location(std::ostream& stream = std::cout);

    /* Saves the terminal cursor's current location to be restored later. */
    void w_save_cursor_location(std::wostream& wstream = std::wcout);

    /* Restores the terminal cursor to a previously saved location. */
    void restore_cursor_location(std::ostream& stream = std::cout);

    /* Restores the terminal cursor to a previously saved location. */
    void w_restore_cursor_location_w(std::wostream& wstream = std::wcout);

    /* Moves the terminal cursor up a chosen number of lines.
       If the cursor is at the top of the window, this function will
       have no effect. */
    void move_cursor_up(size_t lines = 1, std::ostream& stream = std::cout);

    /* Moves the terminal cursor up a chosen number of lines.
       If the cursor is at the top of the window, this function will
       have no effect. */
    void w_move_cursor_up(size_t lines = 1, std::wostream& wstream = std::wcout);

    /* Moves the terminal cursor down a chosen number of lines.
       If the cursor is at the bottom of the window, this function will
       have no effect. */
    void move_cursor_down(size_t lines = 1, std::ostream& stream = std::cout);
    
    /* Moves the terminal cursor down a chosen number of lines.
       If the cursor is at the bottom of the window, this function will
       have no effect. */
    void w_move_cursor_down(size_t lines = 1, std::wostream& wstream = std::wcout);

    /* Moves the terminal cursor to the right a chosen number of columns.
       If the cursor is at the right edge of the window, this function will
       have no effect. */
    void move_cursor_right(size_t columns = 1, std::ostream& stream = std::cout);

    /* Moves the terminal cursor to the right a chosen number of columns.
       If the cursor is at the right edge of the window, this function will
       have no effect. */
    void w_move_cursor_right(size_t columns = 1, std::wostream& wstream = std::wcout);

    /* Moves the terminal cursor to the left a chosen number of columns.
       If the cursor is at the left edge of the window, this function will
       have no effect. */
    void move_cursor_left(size_t columns = 1, std::ostream& stream = std::cout);

    /* Moves the terminal cursor to the left a chosen number of columns.
       If the cursor is at the left edge of the window, this function will
       have no effect. */
    void w_move_cursor_left(size_t columns = 1, std::wostream& wstream = std::wcout);

    /* Detects the terminal window's current size in columns and rows. */
    Coord get_window_size();

    /* Gets a keypress of input without requiring an enter keypress.
       Optional parameters:
       * whether to wait for the keypress.
       
       Some possible return values are entire words or multiple words, such as "enter"
       or "left arrow". To see all possible return values, you can look through the
       function definition or run the terminal tests (the first test is for the get_key function).
       Here are some potentially unexpected return values:
       ctrl+c -> (ends the program immediately)
       ctrl+2 -> (ends the program immediately)
       ctrl+h -> "backspace"
       ctrl+i -> "tab"
       ctrl+j -> "ctrl+enter"
       ctrl+m -> "enter" */
    std::string get_key(bool wait = true);

    /* Waits for and gets a character input without requiring an Enter keypress.
       See a list of many possible return values here: https://gist.github.com/wheelercj/3e6dd4f9c8b267145cbd746d8daccf80
       Cross-platform, unlike _getch from conio.h. */
    char getch_();

    /* Detects whether a key was pressed.
       Ignores modifier keys.
       Cross-platform, unlike _kbhit from conio.h. */
    bool kbhit__();

    /* Pauses the program until the user presses a key.
       Modifier keys are ignored. */
    void pause();

    /* Inserts text at the terminal cursor's current location.
       Any text that is pushed out of the window to the right
       is deleted. */
    void insert(std::string text, std::ostream& stream = std::cout);

    /* Inserts text at the terminal cursor's current location.
       The text can have emoji/Unicode characters. Any text that
       is pushed out of the window to the right is deleted. */
    void w_insert(std::wstring text, std::wostream& wstream = std::wcout);

    /* Deletes a chosen number of characters at the terminal cursor's current location.
       All characters to the right of what is deleted move to the left. */
    void delete_chars(size_t count, std::ostream& stream = std::cout);

    /* Deletes a chosen number of wide characters at the terminal cursor's current location.
       All characters to the right of what is deleted move to the left. */
    void w_delete_chars(size_t count, std::wostream& wstream = std::wcout);

    /* Backspaces a chosen number of characters at the terminal cursor's current location. */
    void backspace_chars(size_t count, std::ostream& stream = std::cout);

    /* Backspaces a chosen number of wide characters at the terminal cursor's current location. */
    void w_backspace_chars(size_t count, std::wostream& wstream = std::wcout);

    /* Inserts empty lines at the terminal cursor's current location.
       Any text that gets pushed down off the window is deleted. */
    void insert_lines(size_t count, std::ostream& stream = std::cout);

    /* Inserts empty lines at the terminal cursor's current location.
       Any text that gets pushed down off the window is deleted. */
    void w_insert_lines(size_t count, std::wostream& wstream = std::wcout);

    /* Deletes a chosen number of lines at the terminal cursor's current location.
       All characters below what is deleted move up. */
    void delete_lines(size_t count, std::ostream& stream = std::cout);

    /* Deletes a chosen number of lines at the terminal cursor's current location.
       All characters below what is deleted move up. */
    void w_delete_lines(size_t count, std::wostream& wstream = std::wcout);

    /* Deletes all characters in the terminal window.
       Use of this function is not recommended without using an alternate screen buffer. */
    void clear_screen(std::ostream& stream = std::cout);

    /* Deletes all characters in the terminal window.
       Use of this function is not recommended without using an alternate screen buffer. */
    void w_clear_screen(std::wostream& wstream = std::wcout);

    /* Switches the terminal app to a different "window" in the terminal.
       Some terminal apps move the cursor to the top when the new "window" opens, and some
       do not. This can be called multiple times consecutively, but only the original
       buffer can be restored. */
    void alternate_screen_buffer(std::ostream& stream = std::cout);

    /* Switches the terminal app to a different "window" in the terminal.
       Some terminal apps move the cursor to the top when the new "window" opens, and some
       do not. This can be called multiple times consecutively, but only the original
       buffer can be restored. */
    void w_alternate_screen_buffer(std::wostream& wstream = std::wcout);

    /* Restores the terminal app's original "window". Does nothing if already on the
    original buffer. */
    void restore_screen_buffer(std::ostream& stream = std::cout);

    /* Restores the terminal app's original "window". Does nothing if already on the
    original buffer. */
    void w_restore_screen_buffer(std::wostream& wstream = std::wcout);

    void set_window_width_to_132(std::ostream& stream = std::cout);

    void w_set_window_width_to_132(std::wostream& wstream = std::wcout);

    void set_window_width_to_80(std::ostream& stream = std::cout);

    void w_set_window_width_to_80(std::wostream& wstream = std::wcout);
}
