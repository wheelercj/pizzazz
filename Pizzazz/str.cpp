#include "str.h"
#include <stdexcept>

namespace pizzazz
{

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

	int find_next_space(std::string str, size_t start)
	{
		if (start >= str.size())
			throw std::invalid_argument("start index out of range.");
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
			throw std::invalid_argument("start index out of range.");
		while (start > 0 && str[start] != ' ')
			start--;
		if (str[start] != ' ')
			return -1;
		return int(start);
	}

}
