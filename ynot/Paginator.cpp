#include "Paginator.h"
#include <regex>

namespace ynot
{
	Paginator::Paginator(
		std::string title,
		std::string text,
		std::string line_prefix,
		std::string line_suffix,
		std::string page_prefix,
		std::string page_suffix,
		int page_line_count,
		int page_width,
		bool show_page_numbers)
	{
		this->page_width = page_width;
		this->show_page_numbers = show_page_numbers;
		text = improve_spacing(text);
		text = wrap(text, page_width, line_prefix, line_suffix);
		if (startswith(text, "\n"))
			text = slice(text, 1);
		std::vector<std::string> lines = split(text, "\n");
		std::string page_title = line_prefix + "\x1b[4m" + title + "\x1b[24m" + "\n";
		std::string page;
		while (lines.size())
		{
			page = page_title;
			page += page_prefix;
			for (int i = 0; i < page_line_count && lines.size(); i++)
			{
				page += "\n" + lines[0];
				lines.erase(lines.begin(), lines.begin() + 1);
			}
			page += page_suffix;
			this->pages.push_back(page);
		}
	}

	Paginator::Paginator(std::string title,
		std::vector<std::string> lines,
		std::string line_prefix,
		std::string line_suffix,
		std::string page_prefix,
		std::string page_suffix,
		int page_line_count,
		int page_width,
		bool show_page_number)
	{
		this->page_width = page_width;
		this->show_page_numbers = show_page_numbers;
		for (std::string& line : lines)
			line = wrap(line, page_width, line_prefix, line_suffix);
		std::string page_title = title + "\n";
		std::string page;
		while (lines.size())
		{
			page = page_title;
			page += page_prefix;
			for (int i = 0; i < page_line_count && lines.size(); i++)
			{
				page += lines[0];
				lines.erase(lines.begin(), lines.begin() + 1);
			}
			page += page_suffix;
			this->pages.push_back(page);
		}
	}

	int Paginator::run(int start_page)
	{
		save_cursor_location();
		set_cursor_style(CursorStyle::hidden);
		this->page_number = start_page;
		alternate_screen_buffer();
		std::string key = "";
		while (key != "escape")
		{
			set_cursor_coords(1, 1);
			print_page();
			key = get_key();
			navigate(key);
			clear_screen();
		}
		restore_screen_buffer();
		restore_cursor_location();
		set_cursor_style(CursorStyle::not_hidden);
		return this->page_number;
	}

	void Paginator::print_page()
	{
		std::cout << this->pages[this->page_number];
		print_navigation_line();
	}

	void Paginator::print_navigation_line()
	{
		std::cout << "\n\n     ";
		if (this->page_number > 0)
			std::cout << "<-";
		else
			std::cout << "  ";
		if (!this->show_page_numbers)
		{
			int remaining_width = this->page_width - 14;
			for (int i = 0; i < remaining_width; i++)
				std::cout << " ";
		}
		else
		{
			std::string page_number_str = 
				std::to_string(this->page_number + 1)
				+ "/" + std::to_string(pages.size());
			int remaining_width =
				this->page_width - 14
				- int(page_number_str.size());
			for (int i = 0; i < remaining_width / 2; i++)
				std::cout << " ";
			std::cout << page_number_str;
			for (int i = 0; i < remaining_width / 2; i++)
				std::cout << " ";
		}
		if (this->page_number < pages.size() - 1)
			std::cout << "->";
		else
			std::cout << "  ";
		std::cout << "     ";
	}

	void Paginator::navigate(std::string key)
	{
		if (key == "left arrow" || key == "up arrow" || key == "page up")
			go_to_previous_page();
		else if (key == "right arrow" || key == "down arrow" || key == "page down")
			go_to_next_page();
		else if (key == "home")
			go_to_first_page();
		else if (key == "end")
			go_to_last_page();
	}

	void Paginator::go_to_previous_page()
	{
		if (this->page_number > 0)
			this->page_number -= 1;
	}

	void Paginator::go_to_next_page()
	{
		if (this->page_number < pages.size() - 1)
			this->page_number += 1;
	}

	void Paginator::go_to_first_page()
	{
		this->page_number = 0;
	}

	void Paginator::go_to_last_page()
	{
		this->page_number = int(pages.size()) - 1;
	}
	
	std::string Paginator::improve_spacing(std::string text)
	{
		text = std::regex_replace(text, std::regex(R"(([^\n])\n([^\n]))"), "$1 $2");
		text = std::regex_replace(text, std::regex(R"(  $)"), "\n");
		return text;
	}
}
