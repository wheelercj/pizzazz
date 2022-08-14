#pragma once
#include "terminal.h"

namespace ynot
{
	/* Creates and runs a Notification. Optionally choose whether
	   to wait for the user to press a key. */
	void notify(std::string text, bool wait = true);

	class Notification
	{
	public:

		Notification(std::string text);

		/* Runs the Notification. Optionally choose whether
		   to wait for the user to press a key. */
		void run(bool wait);

	private:
		std::string text;
		Coord window_size;
		int text_height;

		void print_notification(int top_margin, bool wait);

		void wrap_text();
		
		int get_text_width(const std::string& text);
	};
}
