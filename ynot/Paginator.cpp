#include <regex>
#include "Paginator.h"
#include "str.h"
#include "terminal.h"

namespace ynot
{
	Paginator::Paginator(
		std::string title,
		std::string text,
		std::string line_prefix,
		std::string line_suffix,
		std::string page_prefix,
		std::string page_suffix,
		int max_page_lines,
		int max_page_width,
		bool show_page_numbers)
	{
		if (text.empty())
			throw std::invalid_argument("Text must be given.");
		this->text = this->improve_spacing(text);
		this->using_lines_not_text = false;
		this->title = title;
		this->max_page_lines = max_page_lines;
		this->max_page_width = max_page_width;
		this->show_page_numbers = show_page_numbers;
		this->line_prefix = line_prefix;
		this->line_suffix = line_suffix;
		this->page_prefix = page_prefix;
		this->page_suffix = page_suffix;
	}

	Paginator::Paginator(std::string title,
		std::vector<std::string> lines,
		std::string line_prefix,
		std::string line_suffix,
		std::string page_prefix,
		std::string page_suffix,
		int max_page_lines,
		int max_page_width,
		bool show_page_numbers)
	{
		if (lines.empty())
			throw std::invalid_argument("Lines must be given.");
		this->lines = lines;
		this->using_lines_not_text = true;
		this->title = title;
		this->max_page_width = max_page_width;
		this->max_page_lines = max_page_lines;
		this->show_page_numbers = show_page_numbers;
		this->line_prefix = line_prefix;
		this->line_suffix = line_suffix;
		this->page_prefix = page_prefix;
		this->page_suffix = page_suffix;
	}

	int Paginator::run(int start_page)
	{
		this->page_number = start_page;
		this->before_loop();
		bool page_changed = true;
		std::string key = "";
		while (key != "escape")
		{
			if (page_changed)
			{
				clear_screen();
				set_cursor_coords(1, 1);
				this->print_page();
			}
			key = get_key();
			page_changed = this->on_key(key);
		}
		this->after_loop();
		return this->page_number;
	}

	void Paginator::before_loop()
	{
		reset_on_keyboard_interrupt();
		save_cursor_location();
		set_cursor_style(CursorStyle::hidden);
		alternate_screen_buffer();
		this->format_lines();
		this->title = this->line_prefix + "\x1b[4m" + this->title + "\x1b[24m\n";
		this->create_pages();
	}

	void Paginator::after_loop()
	{
		restore_screen_buffer();
		restore_cursor_location();
		set_cursor_style(CursorStyle::not_hidden);
	}

	void Paginator::format_lines()
	{
		Coord window_size = get_window_size();
		if (this->max_page_width > window_size.x - 1)
			this->max_page_width = window_size.x - 1;
		if (this->max_page_lines > window_size.y - 5)
			this->max_page_lines = window_size.y - 5;
		if (!this->using_lines_not_text)
		{
			this->text = wrap(text, this->max_page_width, this->line_prefix, this->line_suffix);
			this->lines = split(this->text, "\n");
			this->using_lines_not_text = true;
		}
		else
		{
			for (std::string& line : this->lines)
				line = wrap(line, this->max_page_width, this->line_prefix, this->line_suffix);
		}
		for (std::string& line : this->lines)
			line = indent(line, "\n");
	}

	void Paginator::create_pages()
	{
		while (this->lines.size())
		{
			std::string page = this->title;
			int total_page_lines = count(this->title, "\n")
				+ 1 + count(this->page_prefix, "\n");
			for (int i = 0; i < this->lines.size(); i++)
			{
				int line_line_count = count(this->lines[i], "\n");
				if (total_page_lines + line_line_count > this->max_page_lines)
					break;
				total_page_lines += line_line_count;
			}
			page += this->page_prefix;
			for (int i = 0; i < total_page_lines && this->lines.size(); i++)
			{
				page += this->lines[0];
				this->lines.erase(this->lines.begin(), this->lines.begin() + 1);
			}
			page += this->page_suffix;
			this->pages.push_back(page);
		}
	}

	void Paginator::print_page()
	{
		ynot::print(this->pages[this->page_number]);
		this->print_navigation_line();
	}

	void Paginator::print_navigation_line()
	{
		int prefix_and_suffix_width = int(this->line_prefix.size() + this->line_suffix.size());
		std::string page_number_str;
		if (!this->show_page_numbers)
			page_number_str = center("", this->max_page_width - 4 - prefix_and_suffix_width);
		else
		{
			page_number_str =
				std::to_string(this->page_number + 1)
				+ "/" + std::to_string(pages.size());
			page_number_str = center(page_number_str, this->max_page_width - 4 - prefix_and_suffix_width);
		}
		ynot::print("\n" + this->line_prefix);
		if (this->page_number > 0)
			ynot::print("<-");
		else
			ynot::print("  ");
		ynot::print(page_number_str);
		if (this->page_number < pages.size() - 1)
			ynot::print("->");
		else
			ynot::print("  ");
		ynot::print(this->line_suffix);
	}

	bool Paginator::on_key(std::string key)
	{
		if (key == "left arrow" || key == "up arrow" || key == "page up")
		{
			if (this->go_to_previous_page())
				return true;
		}
		else if (key == "right arrow" || key == "down arrow" || key == "page down")
		{
			if (this->go_to_next_page())
				return true;
		}
		else if (key == "home")
		{
			if (this->go_to_first_page())
				return true;
		}
		else if (key == "end")
		{
			if (this->go_to_last_page())
				return true;
		}
		return false;
	}

	bool Paginator::go_to_previous_page()
	{
		if (this->page_number > 0)
		{
			this->page_number -= 1;
			return true;
		}
		return false;
	}

	bool Paginator::go_to_next_page()
	{
		if (this->page_number < pages.size() - 1)
		{
			this->page_number += 1;
			return true;
		}
		return false;
	}

	bool Paginator::go_to_first_page()
	{
		if (this->page_number > 0)
		{
			this->page_number = 0;
			return true;
		}
		return false;
	}

	bool Paginator::go_to_last_page()
	{
		if (this->page_number < pages.size() - 1)
		{
			this->page_number = int(pages.size()) - 1;
			return true;
		}
		return false;
	}

	std::string Paginator::improve_spacing(std::string text)
	{
		std::string result = std::regex_replace(text, std::regex(R"((?!\n|  )\n(?!\n))"), " ");
		return std::regex_replace(text, std::regex(R"(  (?=\n))"), "\n");
	}
}
