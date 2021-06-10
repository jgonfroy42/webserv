/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_communication.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgonfroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 14:32:11 by jgonfroy          #+#    #+#             */
/*   Updated: 2021/06/10 11:27:02 by jgonfroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/webserv.hpp"

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
	if (listen(param->socketId, 3000) < 0)
	{
		std::cerr << "Error: cannot listen to server" << std::endl;
		close(param->socketId);
		return -1;
	}
	return 0;
}

void launch_server(t_param_server *param)
{
	fd_set entries;
	int readable, max_sd;
	int	new_co = 0;

	FD_ZERO(&param->socket);
	max_sd = param->socketId;
	FD_SET(param->socketId, &param->socket);
	while (1)
	{
		memcpy(&entries, &param->socket, sizeof(param->socket));
		std::cout << "Waiting for connection..." << std::endl;
		if ((readable = select(max_sd + 1, &entries, NULL, NULL, &param->timeout)) < 0)
		{
			std::cerr << "Error: cannot select" << std::endl;
			return ;
		}
		if (readable == 0)
		{
			std::cout << "Time out" << std::endl;
			return ;
		}
		for (int i = 0; i <= max_sd && readable; ++i)
		{
			if (FD_ISSET(i, &entries))
			{
				readable--;
				if (i == param->socketId)
				{
					do
					{
						new_co = accept(param->socketId, NULL, NULL);
						if (new_co < 0)
						{
							if (errno != EAGAIN)
								std::cerr << "Error: cannot accept new connection" << std::endl;
							break;
						}
						FD_SET(new_co, &param->socket);
						if (new_co > max_sd)
							max_sd = new_co;
					} while (new_co != -1);
				}
				else
					if (get_data(i, param->socketAddr))
					{
						close(i);
						FD_CLR(i, &param->socket);
						if (i == max_sd)
							while (FD_ISSET(max_sd, &param->socket) == false)
								max_sd--;
					}
			}
		}
	}
}

int	get_data(int i, struct sockaddr_in6 addr)
{
	int	data_len;
	char	buffer[BUFFER_SIZE];

		data_len = recv(i, buffer, sizeof(buffer), 0);
		if (data_len < 0)
			return 0;
		if (data_len == 0)
		{
			std::cout << "Connection closed" << std::endl;
			return 1;
		}

		//parsing request
		std::string requestStr(buffer);
		Request request(requestStr, &addr);
		std::cout << request;

		//send response
		std::string response = "Server response\n";
		send(i, response.c_str(), response.size(), 0);
	return 0;
}
