#include "./../includes/webserv.hpp"

extern bool	end_server;
void	sigint_handler(int signo)
{
	(void)signo;

	end_server = true;
	write(1, "Bye\n", 4);
}
