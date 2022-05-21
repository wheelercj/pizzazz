#include "pizzazz.h"
#include "tests.h"
#include <signal.h>
using namespace std;
namespace paz = pizzazz;  // shorter alias for easier use
using paz::Style;
using paz::CursorStyle;
using paz::Coord;

void run_tests() {
	test_read_key();
	test_getline_autocompleted();
	test_getline_autocompleted_menu();
	test_show_cursor();
	test_show_cursor_blink();
	test_set_cursor_style();
	test_set_window_title();
	test_wide_set_window_title();
	test_wide_print();
	test_print_styled();
	test_wide_print_styled();
	test_print_multi_styled();
	test_wide_print_multi_styled();
	test_set_style();
	test_set_multi_style();
	test_reset_style();
	test_print_rgb();
	test_print_invalid_rgb();
	test_wide_print_rgb();
	test_print_background_rgb();
	test_wide_print_background_rgb();
	test_save_cursor_coords();
	test_print_at();
	test_wide_print_at();
	test_set_cursor_coordinates();
	test_get_cursor_location();
	test_restore_cursor_location();
	test_move_cursor_up();
	test_move_cursor_down();
	test_move_cursor_right();
	test_move_cursor_left();
	test_get_window_size();
	test_getch_();
	test_kbhit__();
	test_getch_if_kbhit();
	test_insert();
	test_wide_insert();
	test_delete_chars();
	test_insert_lines();
	test_delete_lines();
	test_alternate_screen_buffer();
	test_sleep_();
	test_read_key_with_cursor_movements();
}

void signal_callback_handler(int signal_number) {
	cout << "\r                           \rYou pressed Ctrl+c or Ctrl+2 which ends the program early.";
	exit(signal_number);
}

void test_read_key() {
	signal(SIGINT, signal_callback_handler);
	cout << "Press any key and its name will be shown on the next line. Press escape to stop.\n";
	string input = "";
	while (input != "Escape") {
		input = paz::read_key();
		cout << "\r                           \r" << input;
	}
	cout << endl;
}

void test_getline_autocompleted() {
	cout << "Autocomplete suggestions example:\n";
	vector<string> large_city_names = { "Ahmedabad", "Alexandria", "Atlanta", "Baghdad", "Bangalore", "Bangkok", "Barcelona", "Beijing", "Belo Horizonte", "Bogotá", "Buenos Aires", "Cairo", "Chengdu", "Chennai", "Chicago", "Chongqing", "Dalian", "Dallas", "Dar es Salaam", "Delhi", "Dhaka", "Dongguan", "Foshan", "Fukuoka", "Guadalajara", "Guangzhou", "Hangzhou", "Harbin", "Ho Chi Minh City", "Hong Kong", "Houston", "Hyderabad", "Istanbul", "Jakarta", "Jinan", "Karachi", "Khartoum", "Kinshasa", "Kolkata", "Kuala Lumpur", "Lagos", "Lahore", "Lima", "London", "Los Angeles", "Luanda", "Madrid", "Manila", "Mexico City", "Miami", "Moscow", "Mumbai", "Nagoya", "Nanjing", "New York", "Osaka", "Paris", "Philadelphia", "Pune", "Qingdao", "Rio de Janeiro", "Riyadh", "Saint Petersburg", "Santiago", "Seoul", "Shanghai", "Shenyang", "Shenzhen", "Singapore", "Surat", "Suzhou", "São Paulo", "Tehran", "Tianjin", "Tokyo", "Toronto", "Washington", "Wuhan", "Xi'an", "Yangon" };
	string name = paz::getline_autocompleted(large_city_names, "type a large city's name");
	cout << "\nYou chose " << name << endl;
}

void test_getline_autocompleted_menu() {
	cout << "\nSample menu:\n"
		" * Create\n"
		" * Read\n"
		" * Update\n"
		" * Delete\n"
		"> ";
	string choice = paz::getline_autocompleted(
		{ "Create", "Read", "Update", "Delete" },
		"type an option");
	cout << "\nYou chose " << choice << "\n\n";
}

void test_show_cursor() {
	cout << "Cursor visible.\n";
	paz::pause();
	paz::show_cursor(false);
	cout << "Cursor not visible.\n";
	paz::pause();
	paz::show_cursor();
	cout << "Cursor visible again.\n";
	paz::pause();
}

void test_show_cursor_blink() {
	cout << "Cursor blinking.\n";
	paz::pause();
	paz::show_cursor_blink(false);
	cout << "Cursor not blinking.\n";
	paz::pause();
	paz::show_cursor_blink();
	cout << "Cursor blinking again.\n";
	paz::pause();
}

void test_set_cursor_style() {
	cout << "\nDefault cursor";
	paz::pause();
	paz::set_cursor_style(CursorStyle::blinking_block);
	cout << "\nBlinking block cursor";
	paz::pause();
	paz::set_cursor_style(CursorStyle::steady_block);
	cout << "\nSteady block cursor";
	paz::pause();
	paz::set_cursor_style(CursorStyle::blinking_underline);
	cout << "\nBlinking underline cursor";
	paz::pause();
	paz::set_cursor_style(CursorStyle::steady_underline);
	cout << "\nSteady underline cursor";
	paz::pause();
	paz::set_cursor_style(CursorStyle::blinking_i_beam);
	cout << "\nBlinking i-beam cursor";
	paz::pause();
	paz::set_cursor_style(CursorStyle::steady_i_beam);
	cout << "\nSteady i-beam cursor";
	paz::pause();
	paz::set_cursor_style(CursorStyle::reset);
	cout << "\nBack to the default cursor";
	paz::pause();
	cout << endl;
}

void test_set_window_title() {
	paz::set_window_title("hey look, a custom title!");
	cout << "Title set to \"hey look, a custom title!\" The title will only be "
		"visible in some terminals such as Command Prompt.\n\n";
	paz::pause();
}

void test_wide_set_window_title() {
	paz::set_window_title(L"a custom title with emoji! 🔥");
	cout << "Title set to \"a custom title with emoji! (flame emoji here)\" The "
		"title will only be visible in some terminals such as Command Prompt.\n";
	paz::pause();
}

void test_wide_print() {
	paz::wprint(L"A message with emoji! ✨ The emoji will not be visible in some "
		"terminals such as Command Prompt.\n");
	paz::pause();
}

void test_print_styled() {
	paz::print_styled("This is underlined.", { Style::underlined });
	paz::pause();
}

void test_wide_print_styled() {
	paz::print_styled(L"\nThis text has a bright red background.", { Style::bright_bg_red });
	paz::pause();
}

void test_print_multi_styled() {
	paz::print_styled("\nThis is magenta with a green background and strikethrough. "
		"The strikethrough is only visible in some terminals such as Windows Terminal.",
		{ Style::magenta, Style::bg_green, Style::strikethrough });
	paz::pause();
}

void test_wide_print_multi_styled() {
	paz::print_styled(L"\nThis is overlined, double underlined, and italic with an emoji: ✅. "
		"These are only visible in some terminals such as Windows Terminal.\n",
		{ Style::double_underlined, Style::overlined, Style::italic });
	paz::pause();
}

void test_set_style() {
	paz::set_style({ Style::bright_blue });
	cout << "This is \"bright\" blue.\n\n";
	paz::reset_style();
	paz::pause();
}

void test_set_multi_style() {
	paz::set_style({ Style::bg_white, Style::black, Style::bold });
	cout << "This is black and bold with a white background.\n\n";
	paz::reset_style();
	paz::pause();
}

void test_reset_style() {
	paz::set_style({ Style::red });
	cout << "This is red.\n\n";
	paz::reset_style();
	cout << "This is the default color.\n\n";
	paz::pause();
}

void test_print_rgb() {
	paz::print_rgb(95, 255, 95, "This is bright green.\n");
	paz::pause();
}

void test_print_invalid_rgb() {
	cout << "Attempting to use an invalid rgb value.\n";
	try {
		paz::print_rgb(95, 256, 95, "This will raise an exception.");
	}
	catch (invalid_argument& e) {
		cout << e.what() << endl;
	}
}

void test_wide_print_rgb() {
	paz::print_rgb(37, 100, 188, L"This is blue, and here's an emoji that's only visible in "
		"some terminals like Windows Terminal: ⚓.\n");
	paz::pause();
}

void test_print_background_rgb() {
	paz::print_bg_rgb(242, 203, 30, "This has a yellow background.\n");
	paz::pause();
}

void test_wide_print_background_rgb() {
	paz::print_bg_rgb(183, 84, 4, L"This has a brown/orange background, and here's an "
		"emoji that's only visible in some terminals like Windows Terminal: ☃.\n");
	paz::pause();
}

void test_save_cursor_coords() {
	cout << "Saving cursor location after this message.\n";
	paz::save_cursor_location();
	paz::pause();
}

void test_print_at() {
	paz::set_style({ Style::red });
	paz::print_at(0, 0, "This is in the top left corner of the window.");
	paz::reset_style();
	paz::pause();
}

void test_wide_print_at() {
	paz::set_style({ Style::bg_blue });
	paz::print_at(10, 10, L"This starts at coordinates (10,10), and here's an emoji "
		"that's only visible in some terminals like Windows Terminal: ☔.\n");
	paz::reset_style();
	paz::pause();
}

void test_set_cursor_coordinates() {
	paz::set_style({ Style::bright_bg_magenta });
	paz::set_cursor_coords(5, 5);
	cout << "This starts at coordinates (5,5).\n";
	paz::reset_style();
	paz::pause();
}

void test_get_cursor_location() {
	paz::set_style({ Style::bright_bg_green });
	Coord coords = paz::get_cursor_coords();
	cout << "The cursor was at coordinates (" << coords.x << "," << coords.y << ") at "
		"the start of this message.";
	paz::reset_style();
	paz::pause();
}

void test_restore_cursor_location() {
	paz::restore_cursor_location();
	paz::print_rgb(105, 127, 68, "Restored to the saved cursor location.");
	paz::pause();
}

void test_move_cursor_up() {
	paz::set_style({Style::bg_magenta});
	paz::move_cursor_up(3);
	cout << "This is 3 lines higher than the last output.";
	paz::reset_style();
	paz::pause();
}

void test_move_cursor_down() {
	paz::set_style({Style::bg_magenta});
	paz::move_cursor_down(3);
	cout << "This is 3 lines lower than the last output.";
	paz::reset_style();
	paz::pause();
}

void test_move_cursor_right() {
	cout << endl;
	paz::set_style({Style::bg_blue});
	paz::move_cursor_right(3);
	cout << "This is 3 columns to the right from the window's edge.\n";
	paz::reset_style();
	paz::pause();
}

void test_move_cursor_left() {
	paz::save_cursor_location();
	paz::move_cursor_right(20);
	paz::move_cursor_up(15);
	paz::set_style({Style::bg_green});
	cout << "This is higher and further to the right.";
	paz::pause();
	paz::move_cursor_left(5);
	paz::move_cursor_down();
	paz::set_style({Style::bg_cyan});
	cout << "This is 5 columns to the left and 1 line below the end of the last output.";
	paz::reset_style();
	paz::restore_cursor_location();
	paz::pause();
}

void test_get_window_size() {
	Coord window_size = paz::get_window_size();
	cout << "\nThe window has a size of (" << window_size.x << "," << window_size.y << ").\n";
	cout << "Try resizing the window now.\n";
	paz::pause();
	window_size = paz::get_window_size();
	cout << "Now the window has a size of (" << window_size.x << "," << window_size.y << ").\n";
	paz::pause();
}

void test_getch_() {
	cout << "Waiting for you to press a key.\n";
	char input = paz::getch_();
	cout << "You pressed " << input << endl;
	paz::pause();
}

void test_kbhit__() {
	cout << "Waiting for you to press a key.";
	while (!paz::kbhit__());
	cout << "\nYou pressed a key.\n";
	string _ = paz::read_key();
}

void test_getch_if_kbhit() {
	paz::show_cursor(false);
	char input;
	for (int i = 0; "yes"; i++) {
		cout << "\r(" << i << ") Waiting for you to press a key without blocking.";
		input = paz::getch_if_kbhit();
		if (input != 0)
			break;
		paz::sleep_(1000);
	}
	paz::show_cursor(true);
	cout << "\nYou pressed " << input << endl;
	paz::pause();
}

void test_insert() {
	string message = "This is printed first.";
	cout << message;
	paz::move_cursor_left(message.size());
	paz::insert("This is printed second, inserted in front without overwriting. ");
	paz::pause();
}

void test_wide_insert() {
	cout << endl;
	wstring message = L"This is also printed first.";
	paz::wprint(message);
	paz::move_cursor_left(message.size());
	paz::insert(L"Now this is also inserted, with emoji: ☕. ");
	// TODO: figure out why the first letter of the first output gets overwritten in
	// Windows Terminal and not Command Prompt. Does it have something to do with the
	// use of wide characters?
	paz::pause();
}

void test_delete_chars() {
	cout << "\nHere is yet more text. ";
	string message1 = "This is sentence is about to be deleted. ";
	cout << message1;
	string message2 = "This sentence will move to the left.";
	cout << message2;
	paz::pause();
	paz::move_cursor_left(message1.size() + message2.size());
	paz::delete_chars(message1.size());
}

void test_insert_lines() {
	cout << "\n-----------\n";
	paz::move_cursor_up();
	paz::pause();
	paz::insert_lines(1);
	cout << "The line of minuses moved down.\n\n";
	paz::pause();
}

void test_delete_lines() {
	cout << "This line is about to disappear.\nSo is this one.";
	paz::pause();
	paz::move_cursor_up(1);
	paz::delete_lines(2);
}

void test_alternate_screen_buffer() {
	cout << "Now opening the alternate screen buffer.\n";
	paz::pause();
	paz::alternate_screen_buffer();
	cout << "Here is the alternate screen buffer.\n\n";
	cout << "Now clearing the screen.\n";
	paz::pause();
	paz::clear_screen();
	cout << "Now restoring the original screen buffer.\n";
	paz::pause();
	paz::restore_screen_buffer();
	cout << "And we're back.\n";
	paz::pause();
}

void test_sleep_() {
	cout << "Sleeping for 5 seconds.\n";
	paz::sleep_(5000);
	cout << "5 seconds have passed.\n";
}

void test_read_key_with_cursor_movements() {
	paz::pause();
	cout << "\nTry moving the cursor with the arrow keys. Press any other key to stop.";
	paz::show_cursor_blink(false);
	while (true) {
		string input = paz::read_key();
		if (input == "up arrow")
			paz::move_cursor_up();
		else if (input == "left arrow")
			paz::move_cursor_left();
		else if (input == "down arrow")
			paz::move_cursor_down();
		else if (input == "right arrow")
			paz::move_cursor_right();
		else {
			paz::show_cursor_blink();
			return;
		}
	}
}
