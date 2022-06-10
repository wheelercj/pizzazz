#pragma once
#include <string>
#include <vector>
#include <algorithm>

namespace pizzazz
{

	std::string strip(std::string characters, std::string str);
	std::string lstrip(std::string characters, std::string str);
	std::string rstrip(std::string characters, std::string str);

	std::vector<std::string> split(std::string str, std::string split_by = " ");
}
