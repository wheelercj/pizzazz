#include <iostream>
#include "encoding/utf8conv.h"
#include "terminal.h"
#define ESC "\x1b"
#define LESC L"\x1b"

namespace ynot
{

	void print(std::string message)
	{
#ifdef _WIN32
		std::wstring wstr = win32::Utf8ToUtf16(message);
		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		DWORD n_written;
		WriteConsoleW(handle, wstr.c_str(), (DWORD)wstr.size(), &n_written, NULL);
#else
		std::cout << message;
#endif
	}

	void set_cursor_style(CursorStyle style)
	{
		std::cout << ret_set_cursor_style(style);
	}

	std::string ret_set_cursor_style(CursorStyle style)
	{
		if (style == CursorStyle::not_hidden)
			return ESC "[?25h";
		else if (style == CursorStyle::hidden)
			return ESC "[?25l";
		else if (style == CursorStyle::blinking_default)
			return ESC "[7 q" ESC "[?12h";
		else if (style == CursorStyle::steady_default)
			return ESC "[7 q" ESC "[?12l";
		else
			return ESC "[" + std::to_string(int(style)) + " q";
	}

	void reset_cursor_style()
	{
		std::cout << ret_reset_cursor_style();
	}

	std::string ret_reset_cursor_style()
	{
		return ESC "[" + std::to_string(int(CursorStyle::blinking_default)) + " q";
	}

	void set_window_title(std::string title)
	{
		print(ret_set_window_title(title));
	}

	std::string ret_set_window_title(std::string title)
	{
		if (title.size() >= 255)
			throw std::invalid_argument("The title must be < 255 characters.");
		return ESC "]0;" + title + ESC "\x5C";
	}

	void print_styled(std::string message, std::vector<Style> styles)
	{
		print(ret_print_styled(message, styles));
	}

	std::string ret_print_styled(std::string message, std::vector<Style> styles)
	{
		return ret_set_style(styles) + message + ret_reset_style();
	}

	void set_style(std::vector<Style> styles)
	{
		std::cout << ret_set_style(styles);
	}

	std::string ret_set_style(std::vector<Style> styles)
	{
		std::string str;
		for (Style style : styles)
			str += ESC "[" + std::to_string(int(style)) + "m";
		return str;
	}

	void reset_style()
	{
		std::cout << ret_reset_style();
	}

	std::string ret_reset_style()
	{
		return "\x1b[0m";
	}

	void print_rgb(unsigned red, unsigned green, unsigned blue, std::string message)
	{
		print(ret_print_rgb(red, green, blue, message));
	}

	std::string ret_print_rgb(unsigned red, unsigned green, unsigned blue, std::string message)
	{
		return ret_set_rgb(red, green, blue) + message + ret_reset_style();
	}

	void print_bg_rgb(unsigned red, unsigned green, unsigned blue, std::string message)
	{
		print(ret_print_bg_rgb(red, green, blue, message));
	}

	std::string ret_print_bg_rgb(unsigned red, unsigned green, unsigned blue, std::string message)
	{
		return ret_set_bg_rgb(red, green, blue) + message + ret_reset_style();
	}

	void set_rgb(unsigned red, unsigned green, unsigned blue)
	{
		std::cout << ret_set_rgb(red, green, blue);
	}

	std::string ret_set_rgb(unsigned red, unsigned green, unsigned blue)
	{
		if (red > 255 || green > 255 || blue > 255)
			throw std::invalid_argument("Error: the values for the colors must be within the range [0,255].");
		return ESC "[38;2;" + std::to_string(red) + ";" + std::to_string(green) + ";" + std::to_string(blue) + "m";
	}

	void set_bg_rgb(unsigned red, unsigned green, unsigned blue)
	{
		std::cout << ret_set_bg_rgb(red, green, blue);
	}

	std::string ret_set_bg_rgb(unsigned red, unsigned green, unsigned blue)
	{
		if (red > 255 || green > 255 || blue > 255)
			throw std::invalid_argument("Error: the values for the colors must be within the range [0,255].");
		return ESC "[48;2;" + std::to_string(red) + ";" + std::to_string(green) + ";" + std::to_string(blue) + "m";
	}

	void print_at(unsigned x, unsigned y, std::string message)
	{
		print(ret_print_at(x, y, message));
	}

	std::string ret_print_at(unsigned x, unsigned y, std::string message)
	{
		return ret_set_cursor_coords(x, y) + message;
	}

	void set_cursor_coords(unsigned x, unsigned y)
	{
		std::cout << ret_set_cursor_coords(x, y);
	}

	std::string ret_set_cursor_coords(unsigned x, unsigned y)
	{
		return ESC "[" + std::to_string(y) + ";" + std::to_string(x) + "H";
	}

	void set_cursor_coords(Coord coord)
	{
		std::cout << ret_set_cursor_coords(coord);
	}

	std::string ret_set_cursor_coords(Coord coord)
	{
		return ESC "[" + std::to_string(coord.y) + ";" + std::to_string(coord.x) + "H";
	}

	Coord __get_cursor_coords()
	{
		char input;
		input = getch_();  // \x1b
		input = getch_();  // [
		std::string number;
		while (input != ';')
		{
			input = getch_();
			if (input >= '0' && input <= '9')
				number.push_back(input);
		}
		Coord coord;
		coord.y = std::stoi(number);
		number = "";
		while (input != 'R')
		{
			input = getch_();
			if (input >= '0' && input <= '9')
				number.push_back(input);
		}
		coord.x = std::stoi(number);
		return coord;
	}

	Coord get_cursor_coords()
	{
		std::cout << ESC "[6n";  // request coordinates in the format \x1b[y;xR
		return __get_cursor_coords();
	}

	void save_cursor_location()
	{
		std::cout << ret_save_cursor_location();
	}

	std::string ret_save_cursor_location()
	{
		return ESC "[s";
	}

	void restore_cursor_location()
	{
		std::cout << ret_restore_cursor_location();
	}

	std::string ret_restore_cursor_location()
	{
		return ESC "[u";
	}

	void move_cursor_up(size_t lines)
	{
		std::cout << ret_move_cursor_up(lines);
	}

	std::string ret_move_cursor_up(size_t lines)
	{
		return ESC "[" + std::to_string(lines) + "A";
	}

	void move_cursor_down(size_t lines)
	{
		std::cout << ret_move_cursor_down(lines);
	}

	std::string ret_move_cursor_down(size_t lines)
	{
		return ESC "[" + std::to_string(lines) + "B";
	}

	void move_cursor_right(size_t columns)
	{
		std::cout << ret_move_cursor_right(columns);
	}

	std::string ret_move_cursor_right(size_t columns)
	{
		return ESC "[" + std::to_string(columns) + "C";
	}

	void move_cursor_left(size_t columns)
	{
		std::cout << ret_move_cursor_left(columns);
	}

	std::string ret_move_cursor_left(size_t columns)
	{
		return ESC "[" + std::to_string(columns) + "D";
	}

	Coord get_window_size()
	{
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

	std::string get_key(bool wait)
	{
		if (!wait && !kbhit__())
			return "";
		char input = getch_();
		switch (input)
		{
		case '\r': return "enter";
		case '\t': return "tab";
		case '\x1b': return "escape";
		case '\b': return "backspace";
		case '\n': return "ctrl+enter";
		case '\x1': return "ctrl+a";
		case '\x2': return "ctrl+b";
		case '\x4': return "ctrl+d";
		case '\x5': return "ctrl+e";
		case '\x6': return "ctrl+f";
		case '\a': return "ctrl+g";
		case '\v': return "ctrl+k";
		case '\f': return "ctrl+l";
		case '\xe': return "ctrl+n";
		case '\xf': return "ctrl+o";
		case '\x10': return "ctrl+p";
		case '\x11': return "ctrl+q";
		case '\x12': return "ctrl+r";
		case '\x13': return "ctrl+s";
		case '\x14': return "ctrl+t";
		case '\x15': return "ctrl+u";
		case '\x16': return "ctrl+v";
		case '\x17': return "ctrl+w";
		case '\x18': return "ctrl+x";
		case '\x19': return "ctrl+y";
		case '\x1a': return "ctrl+z";
		case '\x1c': return "ctrl+\\";
		case '\x1d': return "ctrl+]";
		case '': return "ctrl+backspace";
		case 'à':
			if (!kbhit__())
				return "à";
			input = getch_();
			switch (input)
			{
			case 'S': return "delete";
			case 'H': return "up arrow";
			case 'P': return "down arrow";
			case 'K': return "left arrow";
			case 'M': return "right arrow";
			case 'G': return "home";
			case 'O': return "end";
			case 'I': return "page up";
			case 'Q': return "page down";
			case 'R': return "insert";
			case 's': return "ctrl+left arrow";
			case 't': return "ctrl+right arrow";
			case '': return "ctrl+up arrow";
			case '‘': return "ctrl+down arrow";
				//case '†': return "ctrl+page up";  // conflicts with F12
			case 'v': return "ctrl+page down";
			case 'w': return "ctrl+home";
			case 'u': return "ctrl+end";
			case '“': return "ctrl+delete";
			case '…': return "f11";
			case '†': return "f12";
			case '‰': return "ctrl+f11";
			case 'Š': return "ctrl+f12";
			case '‡': return "shift+f11";
			case 'ˆ': return "shift+f12";
			case '‹': return "alt+f11";
			case 'Œ': return "alt+f12";
			default:
				std::cerr << "Key not recognized." << std::endl;
				return "";
			}
		case '\0':
			if (!kbhit__())
				return "\0";
			input = getch_();
			switch (input)
			{
			case ';': return "f1";
			case '<': return "f2";
			case '=': return "f3";
			case '>': return "f4";
			case '?': return "f5";
			case '@': return "f6";
			case 'A': return "f7";
			case 'B': return "f8";
			case 'C': return "f9";
			case 'D': return "f10";
			case '”': return "ctrl+tab";
			case '^': return "ctrl+f1";
			case '_': return "ctrl+f2";
			case '`': return "ctrl+f3";
			case 'a': return "ctrl+f4";
			case 'b': return "ctrl+f5";
			case 'c': return "ctrl+f6";
			case 'd': return "ctrl+f7";
			case 'e': return "ctrl+f8";
			case 'f': return "ctrl+f9";
			case 'g': return "ctrl+f10";
			case 'T': return "shift+f1";
			case 'U': return "shift+f2";
			case 'V': return "shift+f3";
			case 'W': return "shift+f4";
			case 'X': return "shift+f5";
			case 'Y': return "shift+f6";
			case 'Z': return "shift+f7";
			case '[': return "shift+f8";
			case '\\': return "shift+f9";
			case ']': return "shift+f10";
			case '£': return "alt+delete";
			case '—': return "alt+home";
			case 'Ÿ': return "alt+end";
			case '™': return "alt+page up";
			case '¡': return "alt+page down";
			case '›': return "alt+left arrow";
			case '': return "alt+right arrow";
			case '˜': return "alt+up arrow";
			case ' ': return "alt+down arrow";
			case '\x1e': return "ctrl+alt+a";
			case '0': return "ctrl+alt+b";
			case '.': return "ctrl+alt+c";
			case ' ': return "ctrl+alt+d";
			case '\x12': return "ctrl+alt+e";
			case '!': return "ctrl+alt+f";
			case '"': return "ctrl+alt+g";
			case '#': return "ctrl+alt+h";
			case '\x17': return "ctrl+alt+i";
			case '$': return "ctrl+alt+j";
			case '%': return "ctrl+alt+k";
			case '&': return "ctrl+alt+l";
			case '2': return "ctrl+alt+m";
			case '1': return "ctrl+alt+n";
			case '\x18': return "ctrl+alt+o";
			case '\x19': return "ctrl+alt+p";
			case '\x10': return "ctrl+alt+q";
			case '\x13': return "ctrl+alt+r";
			case '\x1f': return "ctrl+alt+s";
			case '\x14': return "ctrl+alt+t";
			case '\x16': return "ctrl+alt+u";
			case '/': return "ctrl+alt+v";
			case '\x11': return "ctrl+alt+w";
			case '-': return "ctrl+alt+x";
			case '\x15': return "ctrl+alt+y";
			case ',': return "ctrl+alt+z";
			case 'x': return "ctrl+alt+1";
			case 'y': return "ctrl+alt+2";
			case 'z': return "ctrl+alt+3";
			case '{': return "ctrl+alt+4";
			case '|': return "ctrl+alt+5";
			case '}': return "ctrl+alt+6";
			case '~': return "ctrl+alt+7";
			case '': return "ctrl+alt+8";
			case '€': return "ctrl+alt+9";
			case '': return "ctrl+alt+0";
			case '‚': return "ctrl+alt+-";
			case '(': return "ctrl+alt+'";
			case '\'': return "ctrl+alt+;";
			case '5': return "ctrl+alt+/";
			case '4': return "ctrl+alt+.";
			case '3': return "ctrl+alt+,";
			case '\x1a': return "ctrl+alt+[";
			case '\x1b': return "ctrl+alt+]";
			case 'ƒ': return "ctrl+alt+=";
			case '\x1c': return "ctrl+alt+enter";
			case '\xe': return "ctrl+alt+backspace";
			case ')': return "ctrl+alt+`";
			case 'h': return "alt+f1";
			case 'i': return "alt+f2";
			case 'j': return "alt+f3";
			case 'k': return "alt+f4";
			case 'l': return "alt+f5";
			case 'm': return "alt+f6";
			case 'n': return "alt+f7";
			case 'o': return "alt+f8";
			case 'p': return "alt+f9";
			case 'q': return "alt+f10";
			default:
				std::cerr << "Key not recognized." << std::endl;
				return "";
			}
		default:
			std::string result;
			result.push_back(input);
			return result;
		}
	}

	char getch_()
	{
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

#ifndef _WIN32
	bool _POSIX_kbhit()
	{
		int bytes_waiting;
		ioctl(0, FIONREAD, &bytes_waiting);
		return bytes_waiting > 0;
	}

	void _enable_raw_mode()
	{
		termios term;
		tcgetattr(0, &term);
		term.c_lflag &= ~(ICANON | ECHO);
		tcsetattr(0, TCSANOW, &term);
	}

	void _disable_raw_mode()
	{
		termios term;
		tcgetattr(0, &term);
		term.c_lflag |= ICANON | ECHO;
		tcsetattr(0, TCSANOW, &term);
	}
#endif

	bool kbhit__()
	{
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

	void pause()
	{
		char _ = getch_();
		while (kbhit__())  // some keys are multiple characters
			_ = getch_();
	}

	void insert(std::string text)
	{
		print(ret_insert(text));
	}

	std::string ret_insert(std::string text)
	{
		return ESC "[" + std::to_string(text.size()) + "@" + text;
	}

	void delete_chars(size_t count)
	{
		std::cout << ret_delete_chars(count);
	}

	std::string ret_delete_chars(size_t count)
	{
		return ESC "[" + std::to_string(count) + "P";
	}

	void backspace_chars(size_t count)
	{
		std::cout << ret_backspace_chars(count);
	}

	std::string ret_backspace_chars(size_t count)
	{
		return ret_move_cursor_left(count) + ret_delete_chars(count);
	}

	void insert_lines(size_t count)
	{
		std::cout << ret_insert_lines(count);
	}

	std::string ret_insert_lines(size_t count)
	{
		return ESC "[" + std::to_string(count) + "L";
	}

	void delete_lines(size_t count)
	{
		std::cout << ESC "[" << count << "M";
	}

	std::string ret_delete_lines(size_t count)
	{
		return ESC "[" + std::to_string(count) + "M";
	}

	void clear_screen()
	{
		std::cout << ret_clear_screen();
	}

	std::string ret_clear_screen()
	{
		return ESC "[2J";
	}

	void alternate_screen_buffer()
	{
		std::cout << ret_alternate_screen_buffer();
	}

	std::string ret_alternate_screen_buffer()
	{
		return ESC "[?1049h";
	}

	void restore_screen_buffer()
	{
		std::cout << ret_restore_screen_buffer();
	}

	std::string ret_restore_screen_buffer()
	{
		return ESC "[?1049l";
	}

	void reset_callback(int signal_number)
	{
		reset_style();
		reset_cursor_style();
		restore_screen_buffer();
		exit(signal_number);
	}

	void reset_on_keyboard_interrupt()
	{
		signal(SIGINT, reset_callback);
	}

	void set_window_width_to_132()
	{
		std::cout << ret_set_window_width_to_132();
	}

	std::string ret_set_window_width_to_132()
	{
		return ESC "[?3h";
	}

	void set_window_width_to_80()
	{
		std::cout << ret_set_window_width_to_80();
	}

	std::string ret_set_window_width_to_80()
	{
		return ESC "[?3l";
	}
}
