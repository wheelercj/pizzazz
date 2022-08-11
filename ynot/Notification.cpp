#include "Notification.h"
#include "str.h"

namespace ynot
{
	Notification::Notification(std::string text)
	{
		this->text = dedent(text);
		this->window_size = get_window_size();
		this->wrap_text();
		this->text_height = count(this->text, '\n') + 2;
	}

	void Notification::run()
	{
		reset_on_keyboard_interrupt();
		save_cursor_location();
		alternate_screen_buffer();
		set_cursor_style(CursorStyle::hidden);
		Coord temp_window_size = get_window_size();
		if (this->window_size != temp_window_size)
		{
			this->window_size = temp_window_size;
			this->wrap_text();
			this->text_height = count(this->text, '\n') + 2;
		}
		int top_margin = (this->window_size.y - this->text_height) / 2;
		if (top_margin < 0)
			throw std::runtime_error("The notification doesn't fit in the window.");
		this->print_notification(top_margin);
		pause();
		reset_cursor_style();
		restore_screen_buffer();
		restore_cursor_location();
	}

	void Notification::print_notification(int top_margin)
	{
		set_cursor_coords(0, 0);
		print(center("", top_margin, '\n'));
		std::string final_text = this->text + "\n\n\x1b[2mPress any key to continue.\x1b[0m";
		int text_width = this->get_text_width(final_text);
		final_text = center_multiline(ljust_multiline(final_text, text_width), this->window_size.x);
		print(final_text);
	}

	void Notification::wrap_text()
	{
		int width = this->window_size.x;
		if (width > 20)
			width = this->window_size.x - 8;
		this->text = wrap(this->text, width, "", "\n");
	}

	int Notification::get_text_width(const std::string& text)
	{
		int width = 0;
		for (const std::string line : split(text, "\n"))
		{
			if (line.size() > width)
				width = int(line.size());
		}
		return width;
	}
}
