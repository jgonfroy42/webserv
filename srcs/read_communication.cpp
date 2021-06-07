/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_communication.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgonfroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 14:32:11 by jgonfroy          #+#    #+#             */
/*   Updated: 2021/06/07 17:23:57 by jgonfroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/webserv.hpp"

extern int	max_sd;
extern int	close_co;
extern int	readable;

int init_server(t_param_server *param)
{
	int	on = 1;

	if ((param->socketId = socket(AF_INET6, SOCK_STREAM, 0)) < 0)
	{
		std::cerr << "Error : cannot create socket" << std::endl;
		return -1;
	}
	if (setsockopt(param->socketId, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on)) < 0)
	{
		std::cerr << "Error : cannot put option on socket" << std::endl;
		return -1;
	}
	if (fcntl(param->socketId, F_SETFL, O_NONBLOCK) < 0)
	{
		std::cerr << "Error : cannot set non blocking" << std::endl;
		return -1;
	}
	memset(&param->socketAddr, 0, sizeof(param->socketAddr));
	param->socketAddr.sin6_family      = AF_INET6;
	memcpy(&param->socketAddr.sin6_addr, &in6addr_any, sizeof(in6addr_any));
	param->socketAddr.sin6_port        = htons(PORT);
	if (bind(param->socketId, (struct sockaddr *)&param->socketAddr, sizeof(param->socketAddr)) < 0)
	{
		std::cerr << "Error : cannot bind socket" << std::endl;
		close(param->socketId);
		return (-1);
	}
	if (listen(param->socketId, 32) < 0)
	{
		std::cerr << "Error: cannot listen to server" << std::endl;
		close(param->socketId);
		return -1;
	}
	return 0;
}

int	get_data(int i, fd_set server, struct sockaddr_in6 addr)
{
	int	data_len, close_co;
	char	buffer[BUFFER_SIZE];

	close_co = 0;
	while (1)
	{
		data_len = recv(i, buffer, sizeof(buffer), 0);
		if (data_len < 0)
		{
			if (errno != EWOULDBLOCK)
			{
				std::cerr << "Error: receive didn't work" << std::endl;
				close_co = 1;
			}
			break;
		}
		if (data_len == 0)
		{
			std::cout << "Connection closed" << std::endl;
			close_co = 1;
			break;
		}

		//parsing request
		std::string requestStr(buffer);
		Request request(requestStr, &addr);
		std::cout << request;

		//send response
		std::string response = "Server response\n";
		send(i, response.c_str(), response.size(), 0);

	}
	if (close_co)
	{
		close(i);
		close_co--;
		FD_CLR(i, &server);
		if (i == max_sd)
			while (FD_ISSET(max_sd, &server) == false)
				max_sd--;
	}
	return 0;
}
