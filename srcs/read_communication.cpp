/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_communication.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgonfroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 14:32:11 by jgonfroy          #+#    #+#             */
/*   Updated: 2021/06/03 16:17:06 by jgonfroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/webserv.hpp"

extern int	max_sd;
extern int	close_co;
extern int	readable;
extern int	connectList[PENDING_MAX];

int	get_data(int i, fd_set server, struct sockaddr_in6 addr)
{
	int	data_len, close_co;
	char	buffer[BUFFER_SIZE];

	std::cout << "Connection readable" << std::endl;
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

/*
	parsing_request();
*/

		//send response
		std::string response = "Server response\n";
		send(i, response.c_str(), response.size(), 0);

//		std::cout << "Message receive " << std::endl;
//		send(i, buffer, data_len, 0);
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
