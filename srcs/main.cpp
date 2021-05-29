#include "../webserv.hpp"

int init_server(sockaddr_in *sock_addr)
{
	int sock_fd;
	int enable = 1;

	if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		std::cerr << "Cannot create socket" << std::endl;
		return (-1);
	}

	if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) == -1)
	{
		std::cerr << "Cannot set options to socket" << std::endl;
		return (-1);
	}
	sock_addr = (sockaddr_in *)calloc(1, sizeof(sockaddr_in)); //attention, a changer par ft_calloc
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

int start_connexion(int server_fd, sockaddr_in *sock_addr)
{
	int new_connexion;
	int addrlen = sizeof(*sock_addr);
	char buffer[BUFFER_SIZE];

	while (1)
	{
		std::cout << "----- Waiting for new connection ------" << std::endl
				  << std::endl;
		if ((new_connexion = accept(server_fd, (struct sockaddr *)sock_addr, (socklen_t *)&addrlen)) < 0)
		{
			std::cerr << "Accept failed" << std::endl;
			return (-1);
		}

		//get request
		recv(new_connexion, &buffer, BUFFER_SIZE, 0);
		std::cout << buffer << std::endl;

		//parsing request
		Request request(buffer, sock_addr);
		std::cout << request;

		//parsing du fichier de config ? ou plutot fichier de config est un arg de build_response ci-apres ? A priori ce serait + logique

		//building response
		char	*response;
		int		response_size;
		response = NULL;
		response_size = build_response(request, &response);
		std::cout << std::endl << "---RESPONSE SENT TO CLIENT:\n" << response << std::endl;

		//send response	
		send(new_connexion, response, response_size, 0);
		delete response;//new dans build_response
		close(new_connexion);
	}
	return 0;
}

int main(void)
{
	int server_fd;
	sockaddr_in *sock_addr = NULL;
	if ((server_fd = init_server(sock_addr)) == -1)
		return 0;
	if (start_connexion(server_fd, sock_addr) == -1)
		return 0;
	free(sock_addr);
	close(server_fd);
	return (0);
}
