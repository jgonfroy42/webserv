/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_communication.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgonfroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 14:32:11 by jgonfroy          #+#    #+#             */
/*   Updated: 2021/05/24 16:22:06 by jgonfroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/webserv.hpp"

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

	std::cout << "ready to read" << std::endl;
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

void	read_socks(int server_fd, fd_set socks)
{
	if (FD_ISSET(server_fd, &socks))
		handle_new_connection(server_fd);

	for (int i = 0; i < PENDING_MAX; ++i)
		if (connectList[i] != -1 && FD_ISSET(connectList[i], &socks))
			deal_with_data(i);
}
