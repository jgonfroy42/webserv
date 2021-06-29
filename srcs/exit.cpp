#include "./../includes/webserv.hpp"

void	sigint_handler(int signo)
{
	(void)signo;
	write(1, "CTRL C\n", 7);
	
}
