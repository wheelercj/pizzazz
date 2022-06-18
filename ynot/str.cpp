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

	std::string strip(std::string to_remove, std::string str)
	{
		return lstrip(to_remove, rstrip(to_remove, str));
	}

	std::string lstrip(std::string to_remove, std::string str)
	{
		bool found = false;
		unsigned a = 0;
		for (; a < str.size() && !found; a++)
		{
			found = true;
			for (unsigned i = 0; i < to_remove.size() && found; i++)
			{
				if (str[a] == to_remove[i])
				{
					found = false;
				}
			}
		}
		if (!found)
			return str;
		a--;
		return str.erase(0, a);
	}

	std::string rstrip(std::string to_remove, std::string str)
	{
		bool found = false;
		size_t z = str.size() - 1;
		for (; z > 0 && !found; z--)
		{
			found = true;
			for (unsigned i = 0; i < to_remove.size() && found; i++)
			{
				if (str[z] == to_remove[i])
				{
					found = false;
				}
			}
		}
		if (!found)
			return str;
		z += 2;
		return str.erase(z);
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
		if (lines[0].size() - lstrip("\t", lines[0]).size())
			whitespace_char = "\t";
		// Find the lowest number of tabs or spaces at the starts of the lines.
		size_t min_count = 99999;
		for (size_t i = 0; i < lines.size(); i++)
		{
			size_t count = lines[i].size() - lstrip(whitespace_char, lines[i]).size();
			if (count < min_count)
				min_count = count;
		}
		if (!min_count)
			return str;
		for (size_t i = 0; i < lines.size(); i++)
			lines[i] = slice(lines[i], int(min_count));
		return join(lines, "\n");
	}

	std::string wrap(std::string str, int width, std::string line_prefix)
	{
		if (line_prefix.size() >= width)
			throw std::invalid_argument("The wrap width must be greater than the line prefix width.");
		std::vector<std::string> lines;
		while (true)
		{
			if (str.empty())
				break;
			str = line_prefix + str;
			if (str.size() < width)
			{
				lines.push_back(str);
				str.clear();
				break;
			}
			int prev_space = find_previous_space(str, width);
			if (prev_space >= 0)
			{
				lines.push_back(str.substr(0, prev_space));
				str.erase(0, prev_space + 1);
			}
			else
			{
				lines.push_back(str.substr(0, width));
				str.erase(0, width);
			}
		}
		return join(lines, "\n");
	}

	bool contains(std::string str, std::string substr)
	{
		for (size_t i = 0; i + substr.size() < str.size(); i++)
		{
			if (substr == slice(str, i, i + substr.size()))
				return true;
		}
		return false;
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
