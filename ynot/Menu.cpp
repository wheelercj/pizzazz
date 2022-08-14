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
		this->original_title = title;
		this->title = title;
		this->original_description = description;
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
		this->window_size = get_window_size();
		this->text_height = this->get_height();
	}

	std::string Menu::run()
	{
		Coord temp_window_size = get_window_size();
		if (this->window_size != temp_window_size)
		{
			this->window_size = temp_window_size;
			this->format_strings();
			this->text_height = this->get_height();
		}
		int left_option_margin = (this->window_size.x - int(this->max_option_width)) / 2;
		int title_width = this->get_title_width();
		int left_title_margin = (this->window_size.x - title_width) / 2;
		int top_margin = (this->window_size.y - this->text_height) / 2;
		this->before_loop();
		if (top_margin < 0)
		{
			this->after_loop();
			throw std::runtime_error("The menu doesn't fit in the window.");
		}
		Coord option_coords = this->print_menu(top_margin, left_title_margin, left_option_margin);
		bool selection_changed = true;
		std::string key = "";
		while (key != "enter")
		{
			if (selection_changed)
				this->print_options(option_coords, left_option_margin);
			key = get_key();
			selection_changed = this->on_key(key);
		}

		this->after_loop();
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

	Coord Menu::print_menu(int top_margin, int left_title_margin, int left_option_margin)
	{
		set_cursor_coords(0, 0);
		for (int i = 0; i < top_margin; i++)
			print("\n");
		if (this->title.size())
		{
			for (int i = 0; i < left_title_margin; i++)
				print(" ");
			print_styled(this->title + "\n\n", { Style::underlined });
		}
		if (this->description.size())
			print(center(this->description, this->window_size.x) + "\n\n");
		Coord option_coords = get_cursor_coords();
		this->print_options(option_coords, left_option_margin);
		return option_coords;
	}

	void Menu::print_options(Coord option_coords, int left_option_margin)
	{
		set_cursor_coords(option_coords);
		for (size_t i = 0; i < this->options.size(); i++)
			this->print_option(int(i), left_option_margin);
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
		if (key == "up arrow")
			return this->key_up_arrow();
		else if (key == "down arrow")
			return this->key_down_arrow();
		else if (key == "home")
			return this->key_home();
		else if (key == "end")
			return this->key_end();
		return false;
	}

	bool Menu::key_up_arrow()
	{
		if (this->current_selection > 0)
			this->current_selection--;
		else
			this->current_selection = this->options.size() - 1;
		return true;
	}

	bool Menu::key_down_arrow()
	{
		if (this->current_selection < this->options.size() - 1)
			this->current_selection++;
		else
			this->current_selection = 0;
		return true;
	}

	bool Menu::key_home()
	{
		if (this->current_selection > 0)
		{
			this->current_selection = 0;
			return true;
		}
		return false;
	}

	bool Menu::key_end()
	{
		if (this->current_selection < this->options.size() - 1)
		{
			this->current_selection = this->options.size() - 1;
			return true;
		}
		return false;
	}

	void Menu::format_strings()
	{
		this->validate_max_option_width();
		int max_width = window_size.x - this->min_horiz_margin_size;
		this->title = wrap(this->original_title, max_width, "", "\n");
		this->description = wrap(this->original_description, max_width, "", "\n");
		this->description = center_multiline(this->original_description, window_size.x);
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

	void Menu::validate_max_option_width()
	{
		int true_max_option_width = this->window_size.x - this->min_horiz_margin_size * 2;
		if (true_max_option_width > this->max_option_width)
			return;
		this->max_option_width = true_max_option_width;
		for (std::string& option : this->options)
			option = wrap(option, int(this->max_option_width), "", "\n");
	}

	int Menu::get_height()
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
