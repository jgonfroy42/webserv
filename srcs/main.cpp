#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <fcntl.h>
#include "./classes/request.hpp"

#define PORT 8080
#define PENDING_MAX 10
#define BUFFER_SIZE 3000

int	connectList[PENDING_MAX];

int	setNonBlocking(int fd)
{
	int	attributes;

	attributes = fcntl(fd, F_GETFL);
	if (attributes == -1)
	{
		std::cout << "Can't get file attributes" << std::endl;
		return -1;
	}
	attributes = (attributes | O_NONBLOCK);
	if (fcntl(fd, F_SETFL, attributes))
	{
		std::cout << "Can't set file attributes" << std::endl;
		return -1;
	}
	return 0;
}

int	init_server(sockaddr_in *sock_addr)
{
	int sock_fd;
	int enable = 1;

	if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		std::cerr << "Cannot create socket" << std::endl;
		return (-1);
	}

	if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) ==  -1)
	{
		std::cerr << "Cannot set options to socket" << std::endl;
		return (-1);
	}

	if (setNonBlocking(sock_fd))
		return -1;

	sock_addr = (sockaddr_in*)calloc(1, sizeof(sockaddr_in)); //attention, a changer par ft_calloc
	sock_addr->sin_family = AF_INET;
	sock_addr->sin_addr.s_addr = INADDR_ANY;
	sock_addr->sin_port = htons(PORT);
	if (bind(sock_fd, (struct sockaddr *)sock_addr, sizeof(*sock_addr)) == -1)
	{
		std::cerr << "Cannot bind socket" << std::endl;
		return (-1);
	}
	if (listen(sock_fd, PENDING_MAX) < 0)
	{
		std::cerr << "Cannot listen socket" << std::endl;
		return (-1);
	}
	return (sock_fd);
}

int	start_connexion(int server_fd, sockaddr_in *sock_addr)
{
	int			new_connexion;
	int 		addrlen = sizeof(*sock_addr);
	std::string	response;
	char		buffer[BUFFER_SIZE];
	int			highsock;

	highsock = server_fd;
	memset((char *) &connectList, 0, sizeof(connectList)); //changer par ft_memset
	while (1)
	{
		std::cout << "----- Waiting for new connection ------" << std::endl << std::endl;
		if ((new_connexion = accept(server_fd, (struct sockaddr *)sock_addr, (socklen_t*)&addrlen)) < 0)
		{
			std::cerr << "Accept failed" << std::endl;
			return (-1);
		}

		//get request
		recv(new_connexion, &buffer, BUFFER_SIZE, 0);
		std::cout << buffer << std::endl;

		//parsing request
		std::string requestStr(buffer);
		Request request(requestStr, sock_addr);
		std::cout << request;

/*
	parsing_request();
*/

		//send response
		response = "Server response\n";
		send(new_connexion, response.c_str(), response.size(), 0);
		close(new_connexion);
	}
	return 0;
}


int main(void)
{
	int			server_fd;
	sockaddr_in	*sock_addr = NULL;
	if ((server_fd = init_server(sock_addr)) == -1)
		return 0;
	if (start_connexion(server_fd, sock_addr) == -1)
		return 0;
	free(sock_addr);
  	close(server_fd);
	return (0);
}
