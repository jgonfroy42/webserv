#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <map>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef std::string string;
typedef std::map<std::string, std::string> map;

void	displayMap(map toDisplay)
{
	for (map::iterator it=toDisplay.begin(); it!=toDisplay.end(); it++)
		std::cout<<it->first << " = " << it->second << std::endl;
	std::cout << std::endl;
}