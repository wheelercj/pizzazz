#pragma once
#include <string>
#include <vector>

namespace ynot
{

	class Paginator
	{
	public:

		/* Throws std::invalid_argument if no text is given. */
		Paginator(
			std::string title,
			std::string text,
			std::string line_prefix = "\n",
			std::string line_suffix = "",
			std::string page_prefix = "",
			std::string page_suffix = "",
			int max_page_lines = 24,
			int max_page_width = 70,
			bool show_page_numbers = true);

		/* Throws std::invalid_argument if no lines are given. */
		Paginator(
			std::string title,
			std::vector<std::string> lines,
			std::string line_prefix = "\n",
			std::string line_suffix = "",
			std::string page_prefix = "",
			std::string page_suffix = "",
			int max_page_lines = 24,
			int max_page_width = 70,
			bool show_page_numbers = true);

		/* Returns the page number on which the paginator was
		   closed. If the terminal window is resized between Paginator
		   runs, attempting to start at the same previously viewed
		   page might not be possible because the number of pages may
		   change. */
		int run(int start_page = 0);

	private:
		std::string title;
		std::string text;
		std::vector<std::string> lines;
		bool using_lines_not_text;
		std::vector<std::string> pages;
		int page_number = 0;  // the current index of the pages vector
		int max_page_width;
		int max_page_lines;
		bool show_page_numbers = true;
		std::string line_prefix;
		std::string line_suffix;
		std::string page_prefix;
		std::string page_suffix;

		void before_loop();
		void after_loop();

		void format_lines();
		void create_pages();
		void print_page();
		void print_navigation_line();

		/* The navigation functions return true if the page
		   changed, false otherwise. */
		bool on_key(std::string key);
		bool go_to_previous_page();
		bool go_to_next_page();
		bool go_to_first_page();
		bool go_to_last_page();

		/* Changes each isolated newline into a space and
		   each double space at end of a line into a newline. */
		std::string improve_spacing(std::string text);
	};

}
