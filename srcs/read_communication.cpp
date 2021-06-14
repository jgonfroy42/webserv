/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_communication.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgonfroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 14:32:11 by jgonfroy          #+#    #+#             */
/*   Updated: 2021/06/11 14:54:20 by jgonfroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/webserv.hpp"

int init_server(t_param_server *param)
{
	int	on = 1;

	//creation du socket qui va recevoir les connections, besoin d'un socket par port
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
	//nombre de connection max, 512 par defaut sur nginx ?
	if (listen(param->socketId, 512) < 0)
	{
		std::cerr << "Error: cannot listen to server" << std::endl;
		close(param->socketId);
		return -1;
	}
	return 0;
}

void launch_server(t_param_server *param)
{
	int nb_readable, new_co;
	int nfds = 1;
	struct pollfd *fds = NULL;

	fds = (struct pollfd *)calloc(nfds, sizeof(*fds));
	fds[0].fd = param->socketId;
	fds[0].events = POLLIN;
	while (1)
	{
		std::cout << "Waiting for connection..." << std::endl;
		if ((nb_readable = poll(fds, nfds , 36000)) < 0)
		{
			std::cerr << "Error: cannot select" << std::endl;
			return ;
		}
		if (nb_readable == 0)
		{
			std::cout << "Time out" << std::endl;
			return ;
		}
		for (int i = 0; i < nfds; ++i)
		{
			//il ne se passe rien pour ce descripteur
			if (fds[i].revents == 0)
				continue;
			if (fds[i].revents != POLLIN)
			{
				std::cout << "Error";
				return ;
			}
			//si le server est lisible, ca veut dire qu'on peut accepter les connexions entrantes
			if (fds[i].fd == param->socketId)
			{
				do
				{
					new_co = accept(param->socketId, NULL, NULL);
					if (new_co < 0)
					{
						if (errno != EWOULDBLOCK)
						{
							std::cerr << "Error: accept failed" << std::endl;
							return ;
						}
						break;
					}
					fds[nfds].fd = new_co;
					fds[nfds].events = POLLIN;
					nfds++;
				} while (new_co != -1);
			}
			else
			{
				if (get_data(fds[i].fd, param->socketAddr))
				{
				//si le read renvoie 0, la connection est fini donc on la close ici
					close(fds[i].fd);
					for (int j = i; j < nfds - 1; ++j)
						fds[j].fd = fds[j+1].fd;
					nfds--;
				}
			}
		}
	}
}

int	get_data(int fd, struct sockaddr_in6 addr)
{
	int	data_len;
	char	buffer[BUFFER_SIZE];

	(void)addr;
	data_len = recv(fd, buffer, sizeof(buffer), 0);
	std::cout << "data len : " << data_len << std::endl;
	if (data_len < 0)
		return 0;
	if (data_len == 0)
	{
		std::cout << "Connection closed" << std::endl;
		return 1;
	}

	//parsing request
	std::string requestStr(buffer);
//	Request request(requestStr, &addr);
//	std::cout << request;
	std::cout << buffer << std::endl;

	//send response
	std::string response = "Server response\n";
	send(fd, response.c_str(), response.size(), 0);
	return 0;
}
