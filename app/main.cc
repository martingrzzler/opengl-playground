#include "lib.h"
#include <exception>
#include <iostream>

int main(int argc, char **argv)
{
	try
	{
		run();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}

	return 0;
}