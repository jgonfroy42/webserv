#include "../includes/webserv.hpp"

void	displayMap(map_str_str toDisplay)
{
	for (map_str_str::iterator it=toDisplay.begin(); it!=toDisplay.end(); it++)
		std::cout<< '\t'<< it->first << " = " << it->second << std::endl;
	std::cout << std::endl;
}
