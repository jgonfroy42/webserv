#include <sys/socket.h>
#include <netinet/in.h> // pour struct sockaddr_in
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <string>
#include <map>

#include <stdlib.h>

#include <fcntl.h>
#include <errno.h>

#include "../../libft/ft_bzero.c"  //SALE
#include "../../libft/ft_calloc.c" //SALE
#include "../../libft/ft_itoa.c"   //SALE
#include "../parsing_request.cpp"  //SALE

// ATTENTION CE N'EST PAS C++98 COMPLIANT, J'AI EU LA FLEMME DE FAIRE UN ITOA LIGNE 100 & 117

int main(int argc, char **argv, char **env)
{
	(void)argc;
	(void)argv;
	(void)env;
	// CREATING THE SOCKET
	int server_fd;
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cerr << "Cannot create socket" << std::endl;
		return (0);
	}
	else
		std::cout << "Socket created. (return: " << server_fd << ")" << std::endl;

	int enable = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
		std::cerr << "setsockopt(SO_REUSEADDR) failed" << std::endl;

	// NAMING THE SOCKET = BIND
	sockaddr_in server;
	const int PORT = 3000;

	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);

	if (bind(server_fd, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		std::cerr << "Cannot bind socket" << std::endl;
		return (0);
	}

	// PREPARING FOR CONNEXION
	if (listen(server_fd, 10) < 0)
	{
		std::cerr << "Listen failed" << std::endl;
		return (1);
	}
	int new_socket;
	sockaddr_in client_addr;
	socklen_t addr_len;
	std::string hello = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 12\n\nHello World!";
	while (1)
	{
		std::cout << "----- Waiting for new connection ------" << std::endl
				  << std::endl;
		if ((new_socket = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len)) < 0)
		{
			std::cerr << "Accept failed" << std::endl;
			return (0);
		}
		char buffer[3000] = {0};
		int valread = read(new_socket, buffer, 3000);
		std::cout << buffer << std::endl;

		// Extraction du fichier dans la requete
		std::string request(buffer);
		size_t start = request.find("/");
		size_t end = request.find(" ", start);
		std::string file(request, start + 1, end - 1 - start);

		std::cout << file << std::endl;
		// CASE WHERE WE NEED TO USE CGI
		if (file.find(".php") != std::string::npos)
		{
			std::map<std::string, std::string> CGIEnv;
			CGIEnv = getCGIEnv(std::string(buffer), client_addr);
			displayCGIEnv(CGIEnv);
			usleep(500); //sinon ca imprime mal
			int link[2];
			char foo[8000]; // TAILLE RANDOM ICI, FAUDRA METTRE UN VRAI TRUC
			pipe(link);
			int fork_ret = fork();
			if (fork_ret == 0)
			{
				dup2(link[1], STDOUT_FILENO);
				close(link[0]);
				close(link[1]);
				char **input = (char **)malloc(sizeof(char *) * 3);

				input[0] = strdup("/usr/bin/php-cgi");
				input[1] = strdup(file.c_str());
				input[2] = NULL;
				int ret_exec = execve("/usr/bin/php-cgi", input, NULL);
				exit(EXIT_FAILURE);
				(void)ret_exec; //unused
			}
			else
			{
				close(link[1]);
				int nbytes = read(link[0], foo, sizeof(foo));
				std::string response("HTTP/1.1 200 OK\nContent-Length: ");
				std::string cgi_response(foo);
				response += ft_itoa(nbytes);
				response += "; ";
				response += cgi_response;
				write(new_socket, response.c_str(), strlen(response.c_str()));
			}
			close(new_socket);
		}
		else
		{
			// Openation du fichier
			int ret = open(file.c_str(), O_RDONLY);
			std::cout << ret << std::endl;
			if (ret != -1)
			{
				char read_buff[8000]; // TAILLE RANDOM ICI, FAUDRA METTRE UN VRAI TRUC
				int read_ret = read(ret, read_buff, 8000);
				std::string file_body(read_buff);
				std::string response("HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ");
				response += ft_itoa(file_body.length());
				response += "\n\n";
				response += file_body;
				std::cout << response << std::endl;
				// Envoie réponse
				write(new_socket, response.c_str(), strlen(response.c_str()));
				close(ret);
				(void)read_ret; //unused
			}
			else
				write(new_socket, hello.c_str(), hello.size());
			std::cout << strerror(errno) << std::endl;
			close(new_socket);
		}
		(void)valread;
	}
	return (0);
}