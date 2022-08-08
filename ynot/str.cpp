#include "str.h"
#include <stdexcept>

namespace ynot
{

	std::string to_lower(std::string str)
	{
		for (char& ch : str)
			ch = tolower(ch);
		return str;
	}

	std::string to_upper(std::string str)
	{
		for (char& ch : str)
			ch = toupper(ch);
		return str;
	}

	std::string slice(std::string str, int start, int end, int step)
	{
		if (step == 0)
			throw std::invalid_argument("step must not be zero.");
		bool reversed = false;
		if (step < 0)
		{
			reversed = true;
			step *= -1;
		}
		while (start < 0)
			start += int(str.size());
		while (end < 0)
			end += int(str.size());
		if (end > str.size())
			end = int(str.size());
		std::string result = "";
		for (int i = start; i < end; i += step)
			result.push_back(str[i]);
		if (reversed)
			std::reverse(result.begin(), result.end());
		return result;
	}

	std::vector<std::string> split(std::string str, std::string split_by)
	{
		if (split_by.empty())
			throw std::invalid_argument("split_by must not be empty.");
		std::vector<std::string> result;
		size_t prev = 0;
		size_t i = 0;
		while (i != std::string::npos)
		{
			i = str.find(split_by, i);
			if (i != std::string::npos)
			{
				result.push_back(str.substr(prev, i - prev));
				i += split_by.size();
				prev = i;
			}
		}
		if (prev < str.size())
			result.push_back(str.substr(prev));
		else if (prev == str.size())
			result.push_back("");
		return result;
	}

	std::string join(std::vector<std::string> strings, std::string join_by)
	{
		std::string result;
		for (int i = 0; i < strings.size(); i++)
		{
			result.append(strings[i]);
			if (i < strings.size() - 1)
				result.append(join_by);
		}
		return result;
	}

	std::string strip(std::string str, std::string to_remove)
	{
		return lstrip(rstrip(str, to_remove), to_remove);
	}

	std::string lstrip(std::string str, std::string to_remove)
	{
		while (str.size())
		{
			if (contains(to_remove, str[0]))
				str.erase(str.begin(), str.begin() + 1);
			else
				return str;
		}
		return str;
	}

	std::string rstrip(std::string str, std::string to_remove)
	{
		while (str.size())
		{
			if (contains(to_remove, str[str.size() - 1]))
				str.pop_back();
			else
				return str;
		}
		return str;
	}

	std::string center(std::string str, int width, char fill_char)
	{
		while (int(str.size()) < width)
		{
			str.push_back(fill_char);
			if (int(str.size()) < width)
				str.insert(str.begin(), fill_char);
		}
		return str;
	}

	std::string center_multiline(std::string str, int width, char fill_char)
	{
		std::vector<std::string> lines = split(str, "\n");
		for (std::string& line : lines)
			line = center(line, width);
		return join(lines, "\n");
	}

	std::string ljust(std::string str, int width, char fill_char)
	{
		while (int(str.size()) < width)
			str.push_back(fill_char);
		return str;
	}

	std::string ljust_multiline(std::string str, int width, char fill_char)
	{
		std::vector<std::string> lines = split(str, "\n");
		for (std::string& line : lines)
			line = ljust(line, width);
		return join(lines, "\n");
	}

	std::string rjust(std::string str, int width, char fill_char)
	{
		while (int(str.size()) < width)
			str.insert(str.begin(), fill_char);
		return str;
	}

	std::string rjust_multiline(std::string str, int width, char fill_char)
	{
		std::vector<std::string> lines = split(str, "\n");
		for (std::string& line : lines)
			line = rjust(line, width);
		return join(lines, "\n");
	}

	bool startswith(std::string str, std::string prefix)
	{
		if (prefix.size() > str.size())
			return false;
		for (size_t i = 0; i < prefix.size(); i++)
		{
			if (prefix[i] != str[i])
				return false;
		}
		return true;
	}

	bool endswith(std::string str, std::string suffix)
	{
		if (suffix.size() > str.size())
			return false;
		for (size_t i = 0; i < suffix.size(); i++)
		{
			size_t j = str.size() - suffix.size() + i;
			if (suffix[i] != str[j])
				return false;
		}
		return true;
	}

	std::string indent(std::string str, std::string line_prefix)
	{
		std::vector<std::string> lines = split(str, "\n");
		for (std::string& line : lines)
			line = line_prefix + line;
		return join(lines, "\n");
	}

	std::string dedent(std::string str)
	{
		if (str.empty())
			return "";
		std::vector<std::string> lines = split(str, "\n");
		std::string whitespace_char = " ";
		for (size_t i = 0; i < lines.size(); i++)
		{
			if (!lines[i].size())
				continue;
			if (lines[i].size() - lstrip(lines[i], "\t").size())
			{
				whitespace_char = "\t";
				break;
			}
		}
		// Find the lowest number of tabs or spaces at the starts of the lines.
		size_t min_count = 99999;
		for (size_t i = 0; i < lines.size(); i++)
		{
			if (!lines[i].size())
				continue;
			size_t count = lines[i].size() - lstrip(lines[i], whitespace_char).size();
			if (count < min_count)
				min_count = count;
		}
		if (!min_count)
			return str;
		for (size_t i = 0; i < lines.size(); i++)
			lines[i] = slice(lines[i], int(min_count));
		return join(lines, "\n");
	}

	std::string wrap(std::string str, int width, std::string line_prefix, std::string line_suffix)
	{
		if (line_prefix.size() + line_suffix.size() >= width)
			throw std::invalid_argument("The wrap width must be greater than the line prefix and suffix width.");
		bool starts_with_newline = startswith(str, "\n");
		bool ends_with_newline = endswith(str, "\n");
		str = strip(str, "\n");
		std::vector<std::string> input_lines = split(str, "\n");
		std::vector<std::string> wrapped_lines;
		for (size_t i = 0; i < input_lines.size(); i++)
		{
			std::string long_line = input_lines[i];
			while (true)
			{
				long_line = line_prefix + long_line;
				if (long_line.size() < width)
				{
					long_line += line_suffix;
					wrapped_lines.push_back(long_line);
					long_line.clear();
					break;
				}
				int prev_space = find_previous_space(long_line, width);
				if (line_prefix.size() >= prev_space)
					prev_space = -1;
				if (prev_space >= 0)
				{
					wrapped_lines.push_back(long_line.substr(0, prev_space) + line_suffix);
					long_line.erase(0, size_t(prev_space) + 1);
				}
				else
				{
					std::string next_line = long_line.substr(0, width) + line_suffix;
					wrapped_lines.push_back(next_line);
					long_line.erase(0, width);
				}
			}
		}
		str = join(wrapped_lines, "");
		if (!starts_with_newline)
			str = lstrip(str, "\n");
		if (!ends_with_newline)
			str = rstrip(str, "\n");
		return str;
	}

	bool contains(std::string str, std::string substr)
	{
		for (size_t i = 0; i + substr.size() < str.size(); i++)
		{
			if (substr == slice(str, int(i), int(i + substr.size())))
				return true;
		}
		return false;
	}

	bool contains(std::string str, char ch)
	{
		for (size_t i = 0; i < str.size(); i++)
		{
			if (ch == str[i])
				return true;
		}
		return false;
	}

	int count(std::string str, std::string substr)
	{
		if (str.size() < substr.size())
			return 0;
		if (substr.empty())
			return int(str.size() + 1);
		int c = 0;
		for (size_t i = 0; i < str.size() - substr.size() + 1; i++)
		{
			if (substr == slice(str, int(i), int(i + substr.size())))
			{
				c++;
				i += substr.size();
			}
		}
		return c;
	}

	int count(std::string str, char ch)
	{
		return (int)std::count(str.begin(), str.end(), ch);
	}

	int find_next_space(std::string str, size_t start)
	{
		if (start >= str.size())
			return -1;
		while (start < str.size() && str[start] != ' ')
			start++;
		if (str[start] != ' ')
			return -1;
		return int(start);
	}

	int find_previous_space(std::string str, size_t start)
	{
		if (start == std::string::npos)
			start = str.size() - 1;
		if (start >= str.size())
			return -1;
		while (start > 0 && str[start] != ' ')
			start--;
		if (str[start] != ' ')
			return -1;
		return int(start);
	}

}
