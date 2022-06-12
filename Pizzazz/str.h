#pragma once
#include <string>
#include <vector>
#include <limits.h>  // INT_MAX

namespace pizzazz
{
	/* Makes an entire string lowercase. */
	std::string to_lower(std::string str);

	/* Makes an entire string uppercase. */
	std::string to_upper(std::string str);

	/* Returns part of a given string. The start, end, and step parameters can be
	   negative. A negative start and/or end counts from the end of the string
	   instead of the beginning. A negative step reverses the result. An end
	   value greater than the length of the string is changed to length of the
	   string. The start is inclusive but the end is exclusive. */
	std::string slice(std::string str, int start, int end = INT_MAX, int step = 1);

	/* Splits a string by a substring. Throws std::invalid_argument if split_by
	   is an empty string. */
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

	/* Prepends a prefix to each line of a multiline string. */
	std::string indent(std::string str, std::string line_prefix = "    ");

	/* Evenly removes leading whitespace from each line of a multiline string. */
	std::string dedent(std::string str);

	/* Wraps words cleanly into lines. Throws std::invalid_argument if the wrap
	   width is not greater than the line prefix width. */
	std::string wrap(std::string str, int width, std::string line_prefix = "");

	/* Finds the index of the next space after an optional starting index.
	   Returns -1 if there is no next space. */
	int find_next_space(std::string str, size_t start = 0);
	
	/* Finds the index of the latest space before an optional starting index.
	   Returns -1 if there is no previous space. */
	int find_previous_space(std::string str, size_t start = std::string::npos);
}
