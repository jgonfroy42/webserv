/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_communication.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgonfroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 14:32:11 by jgonfroy          #+#    #+#             */
/*   Updated: 2021/05/21 18:08:46 by jgonfroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/webserv.hpp"

extern int	connectList[PENDING_MAX];

void	handle_new_connection(int server_fd)
{
	int	i;
	int new_connection;

	if ((new_connection = accept(server_fd, 0, 0) < 0))
	{
		std::cerr << "Accept failed" << std::endl;
		return ;
	}
	setNonBlocking(new_connection);
	for (i = 0; i < PENDING_MAX; ++i)
	{
		if (connectList[i] == 0)
		{
			connectList[i] = new_connection;
			return ;
		}
	}
	//si plus de place pour la co envoyer un message au client pour lui dire qu'il n'y a plus de place
	std::cout << "No more connection available" << std::endl;
	close(new_connection);
}

void	read_socks(int server_fd, fd_set socks)
{
	int	i;

	if (FD_ISSET(server_fd, &socks))
		handle_new_connection(server_fd);
	(void)i;
}
