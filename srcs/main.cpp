/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgonfroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 13:57:36 by jgonfroy          #+#    #+#             */
/*   Updated: 2021/05/24 13:34:17 by jgonfroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/webserv.hpp"

int	highsock;
int	connectList[PENDING_MAX];

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
