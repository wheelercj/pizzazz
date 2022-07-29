#pragma once

#include "terminal.h"
#include <optional>  // C++17+

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

		/* Returns a falsy object if the user exits the menu by pressing
		   escape. Throws std::runtime_error if the menu's text doesn't
		   fit in the terminal window, e.g. if there is too much text,
		   the font size is too large, and/or the window is too small.
		   Returns a std::optional containing one of the given option
		   strings otherwise. */
		std::optional<std::string> run();

	private:
		std::string title;
		std::string description;
		std::vector<std::string> options;
		int min_horiz_margin_size;
		size_t max_option_width = 0;
		size_t current_selection = 0;

		void before_loop();

		void after_loop();

		/* Returns the coordinates of the top-left corner of the options. */
		Coord print_menu(Coord window_size, int top_margin, int left_title_margin, int left_option_margin);

		void print_options(Coord option_coords, int left_option_margin);

		void print_option(int option_index, int left_option_margin);

		/* Returns true if the current selection changed, false otherwise. */
		bool on_key(std::string key);

		void format_strings(Coord window_size);

		int get_title_width();

		void validate_max_option_width(Coord window_size);

		int get_height(Coord window_size);
	};
}
