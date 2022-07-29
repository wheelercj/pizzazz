#include "Menu.h"
#include "str.h"

namespace ynot
{
	Menu::Menu(
		std::string title,
		std::vector<std::string> options,
		std::string description,
		int min_horiz_margin_size)
	{
		this->title = title;
		this->description = description;
		this->options = options;
		this->min_horiz_margin_size = min_horiz_margin_size;
		if (options.empty())
			throw std::invalid_argument("At least one option must be given.");
		for (std::string option : options)
		{
			if (option.size() > this->max_option_width)
				this->max_option_width = option.size();
		}
	}

	std::string Menu::run()
	{
		Coord window_size = get_window_size();
		format_strings(window_size);
		int left_option_margin = (window_size.x - int(this->max_option_width)) / 2;
		int title_width = get_title_width();
		int left_title_margin = (window_size.x - title_width) / 2;
		int text_height = get_height(window_size);
		int top_margin = (window_size.y - text_height) / 2;
		before_loop();
		if (top_margin < 0)
		{
			after_loop();
			throw std::runtime_error("The menu doesn't fit in the window.");
		}
		Coord option_coords = print_menu(window_size, top_margin, left_title_margin, left_option_margin);
		bool selection_changed = true;
		std::string key = "";
		while (key != "enter")
		{
			if (selection_changed)
				print_options(option_coords, left_option_margin);
			key = get_key();
			selection_changed = on_key(key);
		}

		after_loop();
		return this->options[this->current_selection];
	}

	void Menu::before_loop()
	{
		reset_on_keyboard_interrupt();
		save_cursor_location();
		alternate_screen_buffer();
		clear_screen();
		set_cursor_style(CursorStyle::hidden);
	}

	void Menu::after_loop()
	{
		reset_cursor_style();
		restore_screen_buffer();
		restore_cursor_location();
	}

	Coord Menu::print_menu(Coord window_size, int top_margin, int left_title_margin, int left_option_margin)
	{
		set_cursor_coords(0, 0);
		for (int i = 0; i < top_margin; i++)
			print("\n");
		if (this->title.size())
		{
			for (int i = 0; i < left_title_margin; i++)
				print(" ");
			print_styled(this->title + "\n", { Style::underlined });
		}
		if (this->description.size())
			print(description + "\n");
		Coord option_coords = get_cursor_coords();
		print_options(option_coords, left_option_margin);
		return option_coords;
	}

	void Menu::print_options(Coord option_coords, int left_option_margin)
	{
		set_cursor_coords(option_coords);
		for (size_t i = 0; i < this->options.size(); i++)
			print_option(int(i), left_option_margin);
	}

	void Menu::print_option(int option_index, int left_option_margin)
	{
		std::vector<std::string> option_lines = split(this->options[option_index], "\n");
		for (std::string line : option_lines)
		{
			for (int j = 0; j < left_option_margin - 3; j++)
				print(" ");
			if (option_index == this->current_selection)
				print("=> ");
			else
				print("   ");
			print(line + "\n");
		}
	}

	bool Menu::on_key(std::string key)
	{
		if (key == "up arrow" && this->current_selection > 0)
		{
			this->current_selection--;
			return true;
		}
		else if (key == "down arrow" && this->current_selection < this->options.size() - 1)
		{
			this->current_selection++;
			return true;
		}
		else if (key == "home" && this->current_selection > 0)
		{
			this->current_selection = 0;
			return true;
		}
		else if (key == "end" && this->current_selection < this->options.size() - 1)
		{
			this->current_selection = this->options.size() - 1;
			return true;
		}
		return false;
	}

	void Menu::format_strings(Coord window_size)
	{
		validate_max_option_width(window_size);
		int max_width = window_size.x - this->min_horiz_margin_size;
		this->title = wrap(this->title, max_width, "", "\n");
		this->description = wrap(this->description, max_width, "", "\n");
		this->description = center_multiline(this->description, window_size.x);
	}

	int Menu::get_title_width()
	{
		int max_size = 0;
		for (std::string line : split(this->title, "\n"))
		{
			if (line.size() > max_size)
				max_size = int(line.size());
		}
		return max_size;
	}

	void Menu::validate_max_option_width(Coord window_size)
	{
		int true_max_option_width = window_size.x - this->min_horiz_margin_size * 2;
		if (true_max_option_width > this->max_option_width)
			return;
		this->max_option_width = true_max_option_width;
		for (std::string& option : this->options)
			option = wrap(option, int(this->max_option_width), "", "\n");
	}

	int Menu::get_height(Coord window_size)
	{
		int lines_count = 0;
		if (this->title.size())
			lines_count += count(this->title, '\n') + 1;
		if (this->description.size())
			lines_count += count(this->description, '\n') + 1;
		for (std::string option : this->options)
			lines_count += count(option, '\n') + 1;
		return lines_count;
	}
}
