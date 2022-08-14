#pragma once

#include "terminal.h"

namespace ynot
{
	class Menu
	{
	public:

		/* Throws std::invalid_argument if no options are given. */
		Menu(
			std::string title,
			std::vector<std::string> options,
			std::string description = "",
			int min_horiz_margin_size = 5);

		/* Throws std::runtime_error if the menu's text doesn't
		   fit in the terminal window because there is too much text,
		   the font size is too large, and/or the window is too small.
		   Returns one of the given option strings otherwise. */
		std::string run();

	private:
		std::string original_title;
		std::string title;
		std::string original_description;
		std::string description;
		std::vector<std::string> options;
		Coord window_size;
		int text_height;
		int min_horiz_margin_size;
		size_t max_option_width = 0;
		size_t current_selection = 0;  // The current index of the options vector.

		void before_loop();

		void after_loop();

		/* Returns the coordinates of the top-left corner of the options. */
		Coord print_menu(int top_margin, int left_title_margin, int left_option_margin);

		void print_options(Coord option_coords, int left_option_margin);

		void print_option(int option_index, int left_option_margin);

		/* The navigation functions return true if the current selection
		   changed, false otherwise. */
		bool on_key(std::string key);
		bool key_up_arrow();
		bool key_down_arrow();
		bool key_home();
		bool key_end();

		void format_strings();

		int get_title_width();

		void validate_max_option_width();

		int get_height();
	};
}
