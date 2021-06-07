/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgonfroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 13:57:36 by jgonfroy          #+#    #+#             */
/*   Updated: 2021/06/07 17:21:59 by jgonfroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/webserv.hpp"

int	max_sd;
int	readable;
int close_co;

void launch_server(t_param_server *param)
{
	fd_set entries;
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
					get_data(i, param->socket, param->socketAddr);
			}
		}
	}
}

int	main(void)
{
	t_param_server *param = new t_param_server;

	//a mettre dans init struct ? Avec close_co a FALSE ? + timeout;
	param->timeout.tv_sec = 3600;
	param->timeout.tv_usec = 0;
	if (init_server(param))
	{
		delete param;
		return -1;
	}
	launch_server(param);
}
