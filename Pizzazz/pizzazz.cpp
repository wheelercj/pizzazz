#include "pizzazz.h"
#define ESC "\x1b"
#define LESC L"\x1b"

namespace pizzazz {

    /* Hides or shows the terminal's cursor. */
    void show_cursor(bool choice) {
        if (choice)
            std::cout << ESC "[?25h";
        else
            std::cout << ESC "[?25l";
    }

    /* Hides or shows the terminal cursor's blinking. */
    void show_cursor_blink(bool choice) {
        if (choice)
            std::cout << ESC "[?12h";
        else
            std::cout << ESC "[?12l";
    }

    /* Changes the terminal window's title.
       Doesn't work with Windows Terminal.
    */
    void set_window_title(std::string title) {
        std::cout << ESC "]0;" + title + ESC "[";
    }

    /* Changes the terminal window's title.
       The new title can include any emoji/Unicode characters.
       Doesn't work with Windows Terminal.
    */
    void set_window_title(std::wstring title) {
        wprint(LESC "]0;" + title + LESC "[");
    }

    /*  Prints a string that can have emoji/Unicode characters.
        When calling this function, put an L in front of the string.
        For example, `wprint(L"Hi! ✨");` prints `Hi! ✨`.
        Microsoft's C++ compiler doesn't support printing Unicode
        characters with code points greater than U+FFFF.
    */
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

    /* Prints a message with style such as as color, italics, underline, etc. */
    void print_styled(Style style, std::string message) {
        set_style(style);
        std::cout << message;
        reset_style();
    }

    /* Prints a message with style such as as color, italics, underline, etc. */
    void print_styled(Style style, std::wstring message) {
        set_style(style);
        wprint(message);
        reset_style();
    }

    /* Prints a message with styles such as as color, italics, underline, etc. */
    void print_styled(std::vector<Style> styles, std::string message) {
        set_style(styles);
        std::cout << message;
        reset_style();
    }

    /* Prints a message with styles such as as color, italics, underline, etc. */
    void print_styled(std::vector<Style> styles, std::wstring message) {
        set_style(styles);
        wprint(message);
        reset_style();
    }

    /* Sets future output to have a style such as as color, italics, underline, etc. */
    void set_style(Style style) {
        std::cout << ESC "[" << int(style) << "m";
    }

    /* Sets future output to have styles such as as color, italics, underline, etc. */
    void set_style(std::vector<Style> styles) {
        for (Style style : styles)
            std::cout << ESC "[" << int(style) << "m";
    }

    /* Sets future output to have styles such as as color, italics, underline, etc. */
    void set_style(std::string ansi_code_after_esc) {
        std::cout << ESC + ansi_code_after_esc;
    }

    /* Sets future output to NOT have any special styles. */
    void reset_style() {
        std::cout << "\x1b[0m";
    }

    /* Prints a message with a chosen color.
       The red, green, and blue variables must be in the range [0, 255].
    */
    void print_rgb(unsigned red, unsigned green, unsigned blue, std::string message) {
        std::cout << ESC "[38;2;" << red << ";" << green << ";" << blue << "m";
        std::cout << message;
        reset_style();
    }

    /* Prints a message with a chosen color.
       The message can have emoji/Unicode characters.
       The red, green, and blue variables must be in the range [0, 255].
    */
    void print_rgb(unsigned red, unsigned green, unsigned blue, std::wstring message) {
        std::cout << ESC "[38;2;" << red << ";" << green << ";" << blue << "m";
        wprint(message);
        reset_style();
    }

    /* Prints a message with a chosen background color.
       The red, green, and blue variables must be in the range [0, 255].
    */
    void print_bg_rgb(unsigned red, unsigned green, unsigned blue, std::string message) {
        std::cout << ESC "[48;2;" << red << ";" << green << ";" << blue << "m";
        std::cout << message;
        reset_style();
    }

    /* Prints a message with a chosen background color.
       The message can have emoji/Unicode characters.
       The red, green, and blue variables must be in the range [0, 255].
    */
    void print_bg_rgb(unsigned red, unsigned green, unsigned blue, std::wstring message) {
        std::cout << ESC "[48;2;" << red << ";" << green << ";" << blue << "m";
        wprint(message);
        reset_style();
    }

    /* Prints a message at chosen coordinates. */
    void print_at(unsigned x, unsigned y, std::string message) {
        set_cursor_coords(x, y);
        std::cout << message;
    }

    /* Prints a message at chosen coordinates.
       The message can have emoji/Unicode characters.
    */
    void print_at(unsigned x, unsigned y, std::wstring message) {
        set_cursor_coords(x, y);
        wprint(message);
    }

    /* Moves the terminal's cursor to chosen coordinates.
       In this coordinate system, x increases to the right
       and y increases downwards.
    */
    void set_cursor_coords(unsigned x, unsigned y) {
        std::cout << ESC "[" << y << ";" << x << "H";
    }

    /* Detects the current coordinates of the terminal's cursor. */
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

    /* Saves the terminal cursor's current location to be restored later. */
    void save_cursor_location() {
        std::cout << ESC "[s";
    }

    /* Restores the terminal cursor to a previously saved location. */
    void restore_cursor_location() {
        std::cout << ESC "[u";
    }

    /* Moves the terminal cursor up one line. */
    void move_cursor_up(unsigned lines) {
        std::cout << ESC "[" << lines << "A";
    }

    /* Moves the terminal cursor down one line. */
    void move_cursor_down(unsigned lines) {
        std::cout << ESC "[" << lines << "B";
    }

    /* Moves the terminal cursor to the right one column. */
    void move_cursor_right(unsigned columns) {
        std::cout << ESC "[" << columns << "C";
    }

    /* Moves the terminal cursor to the left one column. */
    void move_cursor_left(unsigned columns) {
        std::cout << ESC "[" << columns << "D";
    }

    /* Detects the terminal window's current size in columns and rows. */
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

    /* Waits for and gets a character input without an Enter keypress. */
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

    /* Gets a character input without an Enter keypress only if there is input.
       Returns 0 if there is no input.
    */
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

    /* Detects whether a key has been pressed. */
    bool kbhit_() {
        int bytes_waiting;
        ioctl(0, FIONREAD, &bytes_waiting);
        return bytes_waiting > 0;
    }
#endif

    /* Inserts text at the terminal cursor's current location. */
    void insert(std::string text) {
        std::cout << ESC "[" << text.size() << "@";
        std::cout << ESC "[" << text.size() << "D";
        std::cout << text;
    }

    /* Inserts text at the terminal cursor's current location.
       The text can have emoji/Unicode characters.
    */
    void insert(std::wstring text) {
        std::cout << ESC "[" << text.size() << "@";
        std::cout << ESC "[" << text.size() << "D";
        wprint(text);
    }

    /* Deletes a chosen number of characters at the terminal cursor's current location.
       All characters to the right of what is deleted move to the left.
    */
    void delete_chars(unsigned count) {
        std::cout << ESC "[" << count << "P";
    }

    /* Inserts empty lines at the terminal cursor's current location. */
    void insert_lines(unsigned count) {
        std::cout << ESC "[" << count << "L";
    }

    /* Deletes a chosen number of lines at the terminal cursor's current location.
       All characters below what is deleted move up.
    */
    void delete_lines(unsigned count) {
        std::cout << ESC "[" << count << "M";
    }

    /* Deletes all characters in the terminal window.
       Use of this function is not recommended without using an alternate screen buffer.
    */
    void clear_screen() {
        std::cout << ESC "[2J";
    }

    /* Switches the terminal app to a different "window" in the terminal. */
    void alternate_screen_buffer() {
        std::cout << ESC "[?1049h";
    }

    /* Restores the terminal app's original "window". */
    void restore_screen_buffer() {
        std::cout << ESC "[?1049l";
    }

    /* Pauses the program for a chosen amount of time. */
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
