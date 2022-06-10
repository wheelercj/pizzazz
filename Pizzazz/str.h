#pragma once
#include <string>
#include <vector>

namespace pizzazz
{
	/* Splits a string by a substring.
	   Throws std::invalid_argument if split_by is an empty string. */
	std::vector<std::string> split(std::string str, std::string split_by = " ");

	/* Joins strings, optionally putting another string between them. */
	std::string join(std::vector<std::string> strings, std::string join_by = "");

	/* Removes characters from the edges of a string. */
	std::string strip(std::string characters, std::string str);
	
	/* Removes characters from the left edge of a string. */
	std::string lstrip(std::string characters, std::string str);
	
	/* Removes characters from the right edge of a string. */
	std::string rstrip(std::string characters, std::string str);

	/* Determines whether a string starts with a given substr. */
	bool startswith(std::string str, std::string prefix);

	/* Determines whether a string ends with a given substr. */
	bool endswith(std::string str, std::string suffix);
}
