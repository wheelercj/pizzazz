#pragma once
#include <string>
#include <vector>

namespace pizzazz
{
	/* Splits a string by a substring. */
	std::vector<std::string> split(std::string str, std::string split_by = " ");

	/* Removes characters from the edges of a string. */
	std::string strip(std::string characters, std::string str);
	
	/* Removes characters from the left edge of a string. */
	std::string lstrip(std::string characters, std::string str);
	
	/* Removes characters from the right edge of a string. */
	std::string rstrip(std::string characters, std::string str);
}
