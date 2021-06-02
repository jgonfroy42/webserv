/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgonfroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 13:57:36 by jgonfroy          #+#    #+#             */
/*   Updated: 2021/06/02 12:14:23 by jgonfroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/webserv.hpp"

int	highsock;
int	readable;
int	connectList[PENDING_MAX];

//int main(void)
//{
//	int			server_fd;
//	sockaddr_in	*sock_addr = NULL;
//
//	if ((server_fd = init_server(sock_addr)) == -1)
//		return 0;
//	if (start_connexion(server_fd, sock_addr) == -1)
//		return 0;
//	free(sock_addr);
//  	close(server_fd);
//	return (0);
//}
//
int	main(void)
{
	int server_sd, max_sd;
	int	on = 1;
	int	new_co = 0;
	fd_set	server, entries;
	sockaddr_in	sock_addr = NULL;
	bool	close_co;

	if ((server_sd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cerr << "Error : can't create socket" << std::endl;
		return -1;
	}
	if (setsockopt(server_sd, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on)) < 0)
	{
		std::cerr << "Error : can't put option on socket" << std::endl;
		return -1;
	}
	//ioctl is forbidden : need do test with fcntl
	if (ioctl(server_sd, FIONBIO, (char *)&on) < 0)
	{
		std::cerr << "Error : can't set non blocking" << std::endl;
		return -1;
	}

	//est-ce que c'est la ligne qui pose pb ?
	memset(&sock_addr, 0, sizeof(sock_addr));
	sock_addr->sin_family = AF_INET;
	sock_addr->sin_addr.s_addr = INADDR_ANY;
	sock_addr->sin_port = htons(PORT);

	if (bind(server_sd, (struct sockaddr *)sock_addr, sizeof(*sock_addr)) < 0)
	{
		std::cerr << "Cannot bind socket" << std::endl;
		close(server_sd);
		return (-1);
	}
	FD_ZERO(&server);
	max_sd = server_sd;
	FD_SET(server_sd, &server);
	timeout.tv_sec = 3 * 60;
	timeout.tv_usec = 0;

	while (1)
	{
		memcpy(&entries, &server, sizeof(server));
		std::cout << "Waiting connection" << std::endl;
		if ((readable = select(max_sd + 1, &entries, NULL, NULL, &timeout)) < 0)
		{
			std::cout << "Error with select" << std::endl;
			return -1;
		}
		if (readable == 0)
		{
			std::cout << "Time out" << std::endl;
			return 0;
		}
		for (int i = 0; i <= max_sd && readable; ++i)
		{
			if (FD_ISSET(i, &entries))
			{
				readable--;
				if (i == server_sd)
				{
					std::cout << "Server readable" << std::endl;
					while (new_co =! -1)
					{
						new_co = accept(server_sd, NULL, NULL);
						if (new_co < 0)
						{
							if (errno != EAGAIN)
								std::cerr << "Error when accept conection" << std::endl;
							break;
						}
						std::cout << "new connection to server" << std::endl;
						FD_SET(new_co, &server);
						if (new_co > max_sd)
							max_sd = new_co;
					}
				}
				else
				{
					std::cout << "Connection readable" << std::endl;
					close_co = false;
					while (1)
					{
					}
				}
			}
		}
	}
}
