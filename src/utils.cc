#include "utils.h"
#include <fstream>
#include <iostream>
#include <sstream>

namespace utils
{
	std::string read_file_to_string(const char *path)
	{
		std::ifstream stream(path);
		if (!stream.is_open())
			throw "ERROR: file not found";

		std::stringstream buf;
		buf << stream.rdbuf();
		return buf.str();
	}
}