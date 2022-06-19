#pragma once
#include "str.h"
#include "terminal.h"
#include <string>
#include <vector>

namespace ynot
{

	class Paginator
	{
	public:
		Paginator(
			std::string title,
			std::string text,
			std::string line_prefix = "\n",
			std::string line_suffix = "",
			std::string page_prefix = "",
			std::string page_suffix = "",
			int page_line_count = 24,
			int page_width = 70,
			bool show_page_number = true);
		Paginator(
			std::string title,
			std::vector<std::string> lines,
			std::string line_prefix = "\n",
			std::string line_suffix = "",
			std::string page_prefix = "",
			std::string page_suffix = "",
			int page_line_count = 24,
			int page_width = 70,
			bool show_page_number = true);
		int run(int start_page = 0);
	private:
		std::vector<std::string> pages;
		int page_number = 0;
		int page_width;
		bool show_page_numbers = true;

		void print_page();
		void print_navigation_line();
		void navigate(std::string key);
		void go_to_previous_page();
		void go_to_next_page();
		void go_to_first_page();
		void go_to_last_page();

		/* Changes each isolated newline into a space and
		   each double space at end of a line into a newline. */
		std::string improve_spacing(std::string text);
	};

}
