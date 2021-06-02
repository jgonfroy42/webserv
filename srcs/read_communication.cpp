/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_communication.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgonfroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 14:32:11 by jgonfroy          #+#    #+#             */
/*   Updated: 2021/06/02 12:14:25 by jgonfroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/webserv.hpp"

extern int	highsock;
extern int	readable;
extern int	connectList[PENDING_MAX];

void	handle_new_connection(int server_fd)
{
	int new_connection;

	if ((new_connection = accept(server_fd, 0, 0) < 0))
	{
		std::cerr << "Accept failed" << std::endl;
		return ;
	}
	setNonBlocking(new_connection);
	for (int i = 0; i < PENDING_MAX; ++i)
	{
		if (connectList[i] == -1)
		{
			connectList[i] = new_connection;
			return ;
		}
	}
	//si plus de place pour la co envoyer un message au client pour lui dire qu'il n'y a plus de place
	std::cout << "No more connection available" << std::endl;
	close(new_connection);
}

void	deal_with_data(int id)
{
	char buffer[BUFFER_SIZE];

	if (recv(connectList[id], &buffer, BUFFER_SIZE, 0) < 0)
	{
		std::cout << "connection lost" << std::endl;
		close(connectList[id]);
		connectList[id] = -1;
	}
	else
	{
		std::cout << "Received : " << buffer << std::endl;
	}
}

void	read_socks(int server_fd, fd_set current_socks, fd_set server_sock)
{
	int	ret;
	int new_co = 0;
	bool close_co;
	char *buffer[BUFFER_SIZE];

	for (int i = 0; i <= highsock && readable; ++i)
	{
		if (FD_ISSET(i, &current_socks))
		{
			readable--;
			if (i == server_fd)
			{
				std::cout << "Server readable" << std::endl;
				while (new_co != -1)
				{
					new_co = accept(server_fd, NULL, NULL);
					if (new_co < 0)
					{
						if (errno != EAGAIN)
							std::cerr << "Error when accept conection" << std::endl;
						return;
					}
					std::cout << "new connection to server" << std::endl;
					FD_SET(new_co, &server_sock);
					if(new_co > highsock)
						highsock = new_co;
				}
			}
			else
			{
				std::cout << "connection readable" << std::endl;
				close_co = false;
				while (1)
				{
					if ((ret = recv(i, buffer, sizeof(BUFFER_SIZE), 0)) < 0)
					{
						if (errno != EWOULDBLOCK)
						{
							std::cerr << "failure while reading" << std::endl;
							close_co = true;
						}
						return ;
					}
					if (ret == 0)
					{
						std::cout << "connection closed" << std::endl;
						close_co = true;
						return;
					}
					std::cout << "message receive : " << buffer << std::endl;
				}
				if (close_co)
				{
					close(i);
					FD_CLR(i, &server_sock);
					if (i == highsock)
						while (FD_ISSET(highsock, &current_socks) == false)
							highsock--;

				}
			}
		}
	}
}
