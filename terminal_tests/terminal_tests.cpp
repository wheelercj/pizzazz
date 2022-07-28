#include <fstream>
#include <signal.h>
#include "terminal_tests.h"
#include "../ynot/common.h"
#include "../ynot/common.cpp"
#include "../ynot/InputField.h"
#include "../ynot/InputField.cpp"
#include "../ynot/Paginator.h"
#include "../ynot/Paginator.cpp"
#include "../ynot/str.h"
#include "../ynot/str.cpp"
#include "../ynot/terminal.h"
#include "../ynot/terminal.cpp"
#include "../ynot/ynot.h"
using namespace std;
using ynot::Style;
using ynot::CursorStyle;
using ynot::Coord;

int main()
{
	test_get_key();  // test_get_key should always be the first test in every commit.
	test_get_key_without_waiting();
	test_getline_ac();
	test_getline_ac_menu();
	test_getline_ac_numbered_menu();
	test_getline_ac_with_empty_suggestion();
	test_getline_ac_without_input_validation();
	test_getline_ac_without_showing_suggestions();
	test_wrap();
	test_paginator();
	test_paginator_with_emoji();
	test_set_cursor_style();
	test_set_window_title();
	test_set_window_title_with_emoji();
	test_print_emoji();
	test_print_styled();
	test_print_styled_emoji();
	test_print_multi_styled();
	test_print_multi_styled_emoji();
	test_set_style();
	test_set_multi_style();
	test_reset_style();
	test_print_rgb();
	test_print_invalid_rgb();
	test_print_rgb_emoji();
	test_print_background_rgb();
	test_print_background_rgb_emoji();
	test_save_cursor_coords();
	test_print_at();
	test_print_at_with_emoji();
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
	test_insert_emoji();
	test_delete_chars();
	test_insert_lines();
	test_delete_lines();
	test_alternate_screen_buffer();
	test_sleep_();
	test_get_key_with_cursor_movements();
	cout << endl;
	return 0;
}

void test_get_key()
{
	ynot::reset_on_keyboard_interrupt();
	cout << "Press any key and its name will be shown on the next line. Press escape to stop.\n";
	string input = "";
	while (input != "escape")
	{
		input = ynot::get_key();
		cout << "\r                           \r" << input;
	}
	cout << endl;
}

void test_get_key_without_waiting()
{
	ynot::set_cursor_style(CursorStyle::hidden);
	string input;
	for (int i = 0; "yes"; i++)
	{
		cout << "\r(" << i << ") Waiting for you to press a key without completely blocking.";
		input = ynot::get_key(false);
		if (input.size())
			break;
		ynot::sleep_(1000);
	}
	ynot::set_cursor_style(CursorStyle::not_hidden);
	cout << "\nYou pressed " << input << endl;
}

void test_getline_ac()
{
	cout << "\nAutocomplete suggestions example:\n";
	vector<string> large_city_names = { "Ahmedabad", "Alexandria", "Atlanta", "Baghdad", "Bangalore", "Bangkok", "Barcelona", "Beijing", "Belo Horizonte", "Bogotá", "Buenos Aires", "Cairo", "Chengdu", "Chennai", "Chicago", "Chongqing", "Dalian", "Dallas", "Dar es Salaam", "Delhi", "Dhaka", "Dongguan", "Foshan", "Fukuoka", "Guadalajara", "Guangzhou", "Hangzhou", "Harbin", "Ho Chi Minh City", "Hong Kong", "Houston", "Hyderabad", "Istanbul", "Jakarta", "Jinan", "Karachi", "Khartoum", "Kinshasa", "Kolkata", "Kuala Lumpur", "Lagos", "Lahore", "Lima", "London", "Los Angeles", "Luanda", "Madrid", "Manila", "Mexico City", "Miami", "Moscow", "Mumbai", "Nagoya", "Nanjing", "New York", "Osaka", "Paris", "Philadelphia", "Pune", "Qingdao", "Rio de Janeiro", "Riyadh", "Saint Petersburg", "Santiago", "Seoul", "Shanghai", "Shenyang", "Shenzhen", "Singapore", "Surat", "Suzhou", "São Paulo", "Tehran", "Tianjin", "Tokyo", "Toronto", "Washington", "Wuhan", "Xi'an", "Yangon" };
	string name = ynot::getline_ac(large_city_names, "type a large city's name");
	cout << "\nYou chose " << name << endl;
}

void test_getline_ac_menu()
{
	cout << "\nSample menu:"
		"\n * Create"
		"\n * Read"
		"\n * Update"
		"\n * Delete"
		"\n> ";
	string choice = ynot::getline_ac(
		{ "Create", "Read", "Update", "Delete" },
		"type an option");
	cout << "\nYou chose " << choice << "\n\n";
}

void test_getline_ac_numbered_menu()
{
	cout << "\nSample menu:"
		"\n 1. New"
		"\n 2. View"
		"\n 3. Edit"
		"\n 4. Remove"
		"\n> ";
	string choice = ynot::getline_ac({ "1", "2", "3", "4" });
	cout << "\nYou chose ";
	switch (choice[0])
	{
	case '1':
		cout << "New";
		break;
	case '2':
		cout << "View";
		break;
	case '3':
		cout << "Edit";
		break;
	case '4':
		cout << "Remove";
		break;
	}
	cout << "\n\n";
}

void test_getline_ac_with_empty_suggestion()
{
	cout << "\nEnter a month or just press enter to skip: ";
	string month = ynot::getline_ac({ "", "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" });
	if (month.empty())
		cout << "\nYou skipped the question";
	else
		cout << "\nYou chose " << month;
}

void test_getline_ac_without_input_validation()
{
	cout << "\nEnter a color or make up your own:\n";
	string color = ynot::getline_ac(
		{ "red", "orange", "yellow", "green", "blue", "purple" },
		"type a color", ynot::opt::no_validation);
	cout << "\nYou entered: " << color << "\n";
}

void test_getline_ac_without_showing_suggestions()
{
	ifstream file("dictionary.txt");
	if (!file)
	{
		cout << "Error: could not load dictionary.txt";
		return;
	}
	vector<string> words;
	string word;
	cout << "\nLoading dictionary.txt";
	while (!file.eof())
	{
		file >> word;
		if (word.empty())
			throw runtime_error("Not loading words correctly.");
		words.push_back(word);
	}
	ynot::Coord window_size = ynot::get_window_size();
	cout << "\nTry typing a few letters and pressing tab. Enter stop to stop.\n";
	string input = "";
	while (input != "stop")
	{
		input = ynot::getline_ac(words, "", ynot::opt::hide_suggestions);
		ynot::Coord coord = ynot::get_cursor_coords();
		if (coord.x >= window_size.x - 10)
			cout << "\n";
		else
			cout << " ";
	}
}

void test_wrap()
{
	Coord window_size = ynot::get_window_size();
	cout << "\n" << ynot::wrap("This is a very long sentence that will be nicely wrapped in the terminal window no matter what size the terminal window is, unless the terminal window changes size after this is printed.", window_size.x, "\n    ") << "\n";
}

void test_paginator()
{
	std::cout << "\nPaginator test. Press any key to start the paginator, and escape to close the paginator.";
	ynot::pause();
	std::string title = "Recursive descent parser";
	std::string line_prefix = "\n    ";
	std::string body = ynot::dedent(R"(
		In computer science, a recursive descent parser is a kind of top-down parser built from a set of mutually recursive procedures (or a non-recursive equivalent) where each such procedure implements one of the nonterminals of the grammar. Thus the structure of the resulting program closely mirrors that of the grammar it recognizes. [1]

		A predictive parser is a recursive descent parser that does not require backtracking. [2] Predictive parsing is possible only for the class of LL(k) grammars, which are the context - free grammars for which there exists some positive integer k that allows a recursive descent parser to decide which production to use by examining only the next k tokens of input. The LL(k) grammars therefore exclude all ambiguous grammars, as well as all grammars that contain left recursion. Any context - free grammar can be transformed into an equivalent grammar that has no left recursion, but removal of left recursion does not always yield an LL(k) grammar. A predictive parser runs in linear time.

		Recursive descent with backtracking is a technique that determines which production to use by trying each production in turn. Recursive descent with backtracking is not limited to LL(k) grammars, but is not guaranteed to terminate unless the grammar is LL(k). Even when they terminate, parsers that use recursive descent with backtracking may require exponential time.

		Although predictive parsers are widely used, and are frequently chosen if writing a parser by hand, programmers often prefer to use a table - based parser produced by a parser generator [citation needed], either for an LL(k) language or using an alternative parser, such as LALR or LR. This is particularly the case if a grammar is not in LL(k) form, as transforming the grammar to LL to make it suitable for predictive parsing is involved. Predictive parsers can also be automatically generated, using tools like ANTLR.

		Predictive parsers can be depicted using transition diagrams for each non - terminal symbol where the edges between the initial and the final states are labelled by the symbols(terminals and non - terminals) of the right side of the production rule. [3]

		Source: https://en.wikipedia.org/wiki/Recursive_descent_parser)");
	ynot::Paginator paginator(title, body, line_prefix);
	paginator.run();
}

void test_paginator_with_emoji()
{
	std::cout << "\n\nAnother Paginator test, this time with emoji. Press any key"
		"\nto start the paginator, and escape to close the paginator.";
	ynot::pause();
	std::string title = "🍴 menu 🍴";
	std::string body = "🍄  mushroom\n🍅  tomato\n🍆  aubergine\n🍇  grapes\n🍈  melon\n🍉  watermelon\n🍊  tangerine\n🍋  lemon\n🍌  banana\n🍍  pineapple\n🍎  red apple\n🍏  green apple\n🍐  pear\n🍑  peach\n🍒  cherries\n🍓  strawberry\n🍔  hamburger\n🍕  slice of pizza\n🍖  meat on bone\n🍗  poultry leg\n🍘  rice cracker\n🍙  rice ball\n🍚  cooked rice\n🍛  curry and rice\n🍜  steaming bowl\n🍝  spaghetti\n🍞  bread\n🍟  french fries\n🍠  roasted sweet potato\n🍡  dango\n🍢  oden\n🍣  sushi\n🍤  fried shrimp\n🍥  fish cake with swirl design\n🍦  soft ice cream\n🍧  shaved ice\n🍨  ice cream\n🍩  doughnut\n🍪  cookie\n🍫  chocolate bar\n🍬  candy\n🍭  lollipop\n🍮  custard\n🍯  honey pot\n🍰  shortcake\n🍱  bento box\n🍲  pot of food\n🍵  teacup without handle\n🍶  sake bottle and cup\n🍷  wine glass\n🍸  cocktail glass\n🍹  tropical drink\n🍺  beer mug\n🍻  clinking beer mugs\n🍼  baby bottle\n🍾  bottle with popping cork\n🍿  popcorn";
	ynot::Paginator paginator(title, body, "\n    ");
	paginator.run();
}

void test_set_cursor_style()
{
	cout << "\n\nCursor visible";
	ynot::pause();
	ynot::set_cursor_style(CursorStyle::hidden);
	cout << "\nCursor not visible";
	ynot::pause();
	ynot::set_cursor_style(CursorStyle::not_hidden);
	cout << "\nCursor visible again";
	ynot::pause();
	ynot::set_cursor_style(CursorStyle::blinking_default);
	cout << "\nBlinking default cursor";
	ynot::pause();
	ynot::set_cursor_style(CursorStyle::steady_default);
	cout << "\nSteady default cursor";
	ynot::pause();
	ynot::set_cursor_style(CursorStyle::blinking_block);
	cout << "\nBlinking block cursor";
	ynot::pause();
	ynot::set_cursor_style(CursorStyle::steady_block);
	cout << "\nSteady block cursor";
	ynot::pause();
	ynot::set_cursor_style(CursorStyle::blinking_underline);
	cout << "\nBlinking underline cursor";
	ynot::pause();
	ynot::set_cursor_style(CursorStyle::steady_underline);
	cout << "\nSteady underline cursor";
	ynot::pause();
	ynot::set_cursor_style(CursorStyle::blinking_i_beam);
	cout << "\nBlinking i-beam cursor";
	ynot::pause();
	ynot::set_cursor_style(CursorStyle::steady_i_beam);
	cout << "\nSteady i-beam cursor";
	ynot::pause();
	ynot::set_cursor_style(CursorStyle::blinking_default);
	cout << "\nBack to the default cursor";
	ynot::pause();
}

void test_set_window_title()
{
	ynot::set_window_title("hey look, a custom title!");
	cout << "\n\nTitle set to \"hey look, a custom title!\"";
	ynot::pause();
}

void test_set_window_title_with_emoji()
{
	ynot::set_window_title("a custom title with emoji! 🔥");
	ynot::print("\nTitle set to \"a custom title with emoji! 🔥\"");
	ynot::pause();
}

void test_print_emoji()
{
	ynot::print("\n\nHere are a bunch of emojis that are only visible");
	ynot::print("\nin modern terminals such as Windows Terminal: ");
	ynot::print("\n🔥🎊🎁🙂😊❌◀👀😅🚩✔❤🤣😂😍😒👌😘");
	ynot::print("\n🙌🎈🎆🎇🧨🍍🤖✌🤞😉😎🎶😢💖😜👏🚲");
	ynot::print("\n😁👍🌹🎉🎂🤳😃✨😆🤔💕🐊👀💡🍉🍀🚄");
}

void test_print_styled()
{
	ynot::print_styled("\nThis is underlined.", { Style::underlined });
	ynot::pause();
}

void test_print_styled_emoji()
{
	ynot::print_styled("\nThis text has a bright red background"
		"\nand here's an emoji of a scooter: 🛴", { Style::bright_bg_red });
	ynot::pause();
}

void test_print_multi_styled()
{
	ynot::print_styled("\nThis is magenta with a green background and strikethrough. "
		"\nThe strikethrough is only visible in some terminals such as Windows Terminal.",
		{ Style::magenta, Style::bg_green, Style::strikethrough });
	ynot::pause();
}

void test_print_multi_styled_emoji()
{
	ynot::print_styled("\nThis is blue, overlined, double underlined, and italic with an emoji: ✅."
		"\nAll of these (except the color) are only visible in some terminals such "
		"\nas Windows Terminal.",
		{ Style::double_underlined, Style::overlined, Style::italic, Style::blue });
	ynot::pause();
}

void test_set_style()
{
	ynot::set_style({ Style::bright_blue });
	cout << "\nThis is bright blue.\n";
	ynot::reset_style();
	ynot::pause();
}

void test_set_multi_style()
{
	ynot::set_style({ Style::bg_white, Style::black, Style::bold });
	cout << "\nThis is black and bold with a white background.";
	ynot::reset_style();
	ynot::pause();
}

void test_reset_style()
{
	ynot::set_style({ Style::red });
	cout << "\nThis is red.";
	ynot::reset_style();
	cout << "\nThis is the default color.";
	ynot::pause();
}

void test_print_rgb()
{
	ynot::print_rgb(95, 255, 95, "\nThis is bright green.");
	ynot::pause();
}

void test_print_invalid_rgb()
{
	cout << "\nAttempting to use an invalid rgb value. An error message"
		"\nshould appear on the next line.";
	try
	{
		ynot::print_rgb(95, 256, 95, "The argument of 256 will raise an exception.");
	}
	catch (invalid_argument& e)
	{
		cout << endl << e.what();
	}
}

void test_print_rgb_emoji()
{
	ynot::print_rgb(37, 100, 188, "\nThis is blue (rgb 37, 100, 188), and here's an emoji that's only"
		"\nvisible in some terminals like Windows Terminal: ⚓.");
	ynot::pause();
}

void test_print_background_rgb()
{
	ynot::print_bg_rgb(242, 203, 30, "\nThis has a yellow background.");
	ynot::pause();
}

void test_print_background_rgb_emoji()
{
	ynot::print_bg_rgb(183, 84, 4, "\nThis has a brown/orange background, and here's an"
		"\nemoji that's only visible in some terminals like Windows Terminal: ☃.");
	ynot::pause();
}

void test_save_cursor_coords()
{
	cout << "\nSaving cursor location after this message.\n";
	ynot::save_cursor_location();
	ynot::pause();
}

void test_print_at()
{
	ynot::set_style({ Style::red });
	ynot::print_at(0, 0, "This is in the top left corner of the window.");
	ynot::reset_style();
	ynot::pause();
}

void test_print_at_with_emoji()
{
	ynot::set_style({ Style::bg_blue });
	ynot::print_at(10, 10, "This starts at coordinates (10,10), and here's an emoji "
		"that's only visible in some terminals like Windows Terminal: ☔.\n");
	ynot::reset_style();
	ynot::pause();
}

void test_set_cursor_coordinates()
{
	ynot::set_style({ Style::bright_bg_magenta });
	ynot::set_cursor_coords(5, 5);
	cout << "This starts at coordinates (5,5).\n";
	ynot::reset_style();
	ynot::pause();
}

void test_get_cursor_location()
{
	ynot::set_style({ Style::bright_bg_green });
	Coord coords = ynot::get_cursor_coords();
	cout << "The cursor was at coordinates (" << coords.x << "," << coords.y << ") at "
		"the start of this message.";
	ynot::reset_style();
	ynot::pause();
}

void test_restore_cursor_location()
{
	ynot::restore_cursor_location();
	ynot::print_rgb(105, 127, 68, "Restored to the saved cursor location.");
	ynot::pause();
}

void test_move_cursor_up()
{
	ynot::set_style({ Style::bg_magenta });
	ynot::move_cursor_up(3);
	cout << "This is 3 lines higher than the last output.";
	ynot::reset_style();
	ynot::pause();
}

void test_move_cursor_down()
{
	ynot::set_style({ Style::bg_magenta });
	ynot::move_cursor_down(3);
	cout << "This is 3 lines lower than the last output.";
	ynot::reset_style();
	ynot::pause();
}

void test_move_cursor_right()
{
	cout << endl;
	ynot::set_style({ Style::bg_blue });
	ynot::move_cursor_right(3);
	cout << "This is 3 columns to the right from the window's edge.\n";
	ynot::reset_style();
	ynot::pause();
}

void test_move_cursor_left()
{
	ynot::save_cursor_location();
	ynot::move_cursor_right(20);
	ynot::move_cursor_up(15);
	ynot::set_style({ Style::bg_green });
	cout << "This is higher and further to the right.";
	ynot::pause();
	ynot::move_cursor_left(5);
	ynot::move_cursor_down();
	ynot::set_style({ Style::bg_cyan });
	cout << "This is 5 columns to the left and 1 line below the end of the last output.";
	ynot::reset_style();
	ynot::restore_cursor_location();
	ynot::pause();
}

void test_get_window_size()
{
	Coord window_size = ynot::get_window_size();
	cout << "\nThe window has a size of (" << window_size.x << "," << window_size.y << ").\n";
	cout << "Try resizing the window now.\n";
	ynot::pause();
	window_size = ynot::get_window_size();
	cout << "Now the window has a size of (" << window_size.x << "," << window_size.y << ").\n";
	ynot::pause();
}

void test_getch_()
{
	cout << "Waiting for you to press a key.\n";
	char input = ynot::getch_();
	cout << "You pressed " << input << endl;
	ynot::pause();
}

void test_kbhit__()
{
	cout << "Waiting for you to press a key.";
	while (!ynot::kbhit__());
	cout << "\nYou pressed a key.\n";
	string _ = ynot::get_key();
}

void test_getch_if_kbhit()
{
	ynot::set_cursor_style(CursorStyle::hidden);
	char input = 0;
	for (int i = 0; "yes"; i++)
	{
		cout << "\r(" << i << ") Waiting for you to press a key without completely blocking.";
		if (ynot::kbhit__())
			input = ynot::getch_();
		if (input != 0)
			break;
		ynot::sleep_(1000);
	}
	ynot::set_cursor_style(CursorStyle::not_hidden);
	cout << "\nYou pressed " << input << endl;
	ynot::pause();
}

void test_insert()
{
	string message = "This is printed first.";
	cout << message;
	ynot::move_cursor_left(message.size());
	ynot::insert("This is printed second, inserted in front without overwriting. ");
	ynot::pause();
}

void test_insert_emoji()
{
	cout << endl;
	string message = "This is also printed first.";
	ynot::print(message);
	ynot::move_cursor_left(message.size());
	ynot::insert("Now this is also inserted, with emoji: ☕. ");
	ynot::pause();
}

void test_delete_chars()
{
	cout << "\nHere is yet more text. ";
	string message1 = "This sentence is about to be deleted. ";
	cout << message1;
	string message2 = "This sentence will move to the left.";
	cout << message2;
	ynot::pause();
	ynot::move_cursor_left(message1.size() + message2.size());
	ynot::delete_chars(message1.size());
}

void test_insert_lines()
{
	cout << "\n-----------\n";
	ynot::move_cursor_up();
	ynot::pause();
	ynot::insert_lines(1);
	cout << "The line of minuses moved down.\n\n";
	ynot::pause();
}

void test_delete_lines()
{
	cout << "This line is about to disappear.\nSo is this one.";
	ynot::pause();
	ynot::move_cursor_up(1);
	ynot::delete_lines(2);
}

void test_alternate_screen_buffer()
{
	cout << "Now opening the alternate screen buffer.\n";
	ynot::pause();
	ynot::alternate_screen_buffer();
	cout << "Here is the alternate screen buffer.\n\n";
	cout << "Now clearing the screen.\n";
	ynot::pause();
	ynot::clear_screen();
	cout << "Now restoring the original screen buffer.\n";
	ynot::pause();
	ynot::restore_screen_buffer();
	cout << "And we're back.\n";
	ynot::pause();
}

void test_sleep_()
{
	cout << "Sleeping for 5 seconds.\n";
	ynot::sleep_(5000);
	cout << "5 seconds have passed.\n";
}

void test_get_key_with_cursor_movements()
{
	ynot::pause();
	cout << "\nTry moving the cursor with the arrow keys. Press any other key to stop.";
	ynot::set_cursor_style(CursorStyle::steady_default);
	while (true)
	{
		string input = ynot::get_key();
		if (input == "up arrow")
			ynot::move_cursor_up();
		else if (input == "left arrow")
			ynot::move_cursor_left();
		else if (input == "down arrow")
			ynot::move_cursor_down();
		else if (input == "right arrow")
			ynot::move_cursor_right();
		else
		{
			ynot::set_cursor_style(CursorStyle::blinking_default);
			return;
		}
	}
}
