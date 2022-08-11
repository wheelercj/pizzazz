#pragma once
#include "terminal.h"

namespace ynot
{
	/* Creates and runs a Notification. */
	void notify(std::string text);

	class Notification
	{
	public:

		Notification(std::string text);

		/* Throws std::runtime_error if the notification's text doesn't
		   fit in the terminal window because there is too much text,
		   the font size is too large, and/or the window is too small. */
		void run();

	private:
		std::string text;
		Coord window_size;
		int text_height;

		void print_notification(int top_margin);

		void wrap_text();
		
		int get_text_width(const std::string& text);
	};
}
