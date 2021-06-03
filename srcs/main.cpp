/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgonfroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 13:57:36 by jgonfroy          #+#    #+#             */
/*   Updated: 2021/06/03 16:42:14 by jgonfroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/webserv.hpp"

int	max_sd;
int	readable;
int close_co;

int	main(void)
{
	int server_sd;
	int	on = 1;
	int	new_co = 0;
	fd_set	server, entries;
	timeval	timeout;
	struct sockaddr_in6 addr;

	if ((server_sd = socket(AF_INET6, SOCK_STREAM, 0)) < 0)
	{
		std::cerr << "Error : can't create socket" << std::endl;
		return -1;
	}
	if (setsockopt(server_sd, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on)) < 0)
	{
		std::cerr << "Error : can't put option on socket" << std::endl;
		return -1;
	}
	if (fcntl(server_sd, F_SETFL, O_NONBLOCK) < 0)
	{
		std::cerr << "Error : can't set non blocking" << std::endl;
		return -1;
	}

	memset(&addr, 0, sizeof(addr));
	addr.sin6_family      = AF_INET6;
	memcpy(&addr.sin6_addr, &in6addr_any, sizeof(in6addr_any));
	addr.sin6_port        = htons(PORT);

	int  rc = bind(server_sd,
			(struct sockaddr *)&addr, sizeof(addr));
	if (rc < 0)
	{
		std::cerr << "Cannot bind socket" << std::endl;
		close(server_sd);
		return (-1);
	}

	if (listen(server_sd, 32) < 0)
	{
		std::cerr << "Error: cannot listen server" << std::endl;
		close(server_sd);
		return -1;
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
					do
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
					} while (new_co != -1);
				}
				else
					get_data(i, server, addr);
			}
		}
	}
}
