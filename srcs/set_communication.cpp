/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_communication.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgonfroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 14:10:20 by jgonfroy          #+#    #+#             */
/*   Updated: 2021/06/03 16:17:13 by jgonfroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/webserv.hpp"

extern int highsock;
extern int readable;
extern int connectList[PENDING_MAX];

int	setNonBlocking(int fd)
{
//	int	attributes;
//
//	attributes = fcntl(fd, F_GETFL);
//	if (attributes == -1)
//	{
//		std::cout << "Can't get file attributes" << std::endl;
//		return -1;
//	}
//	attributes = (attributes | O_NONBLOCK);
//	if (fcntl(fd, F_SETFL, attributes))
//	{
//		std::cout << "Can't set file attributes" << std::endl;
//		return -1;
//	}
	(void)fd;
	return 0;
}

fd_set	setSelectList(int fd, fd_set socks)
{
	FD_ZERO(&socks);
	FD_SET(fd, &socks);

	for (int i = 0; i < PENDING_MAX; ++i)
	{
		if (connectList[i] != -1)
		{
			FD_SET(connectList[i], &socks);
			if (connectList[i] > highsock)
				highsock = connectList[i];
		}
	}
	return socks;
}

int	init_server(sockaddr_in *sock_addr)
{
	int sock_fd;
	int enable = 1;

	if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		std::cerr << "Cannot create socket" << std::endl;
		return (-1);
	}

	if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) ==  -1)
	{
		std::cerr << "Cannot set options to socket" << std::endl;
		return (-1);
	}

	if (setNonBlocking(sock_fd))
		return -1;

	sock_addr = (sockaddr_in*)calloc(1, sizeof(sockaddr_in)); //attention, a changer par ft_calloc
	sock_addr->sin_family = AF_INET;
	sock_addr->sin_addr.s_addr = INADDR_ANY;
	sock_addr->sin_port = htons(PORT);
	if (bind(sock_fd, (struct sockaddr *)sock_addr, sizeof(*sock_addr)) == -1)
	{
		std::cerr << "Cannot bind socket" << std::endl;
		return (-1);
	}
	if (listen(sock_fd, 32) < 0)
	{
		std::cerr << "Cannot listen socket" << std::endl;
		return (-1);
	}
	return (sock_fd);
}

int	start_connexion(int server_fd, sockaddr_in *sock_addr)
{
//	int			new_connexion;
//	int 		addrlen = sizeof(*sock_addr);
	std::string	response;
//	char		buffer[BUFFER_SIZE];
	fd_set		current_socks, socks;
	struct timeval timeout;

	(void)sock_addr;
	FD_ZERO(&socks);
	highsock = server_fd;
	FD_SET(server_fd, &socks);
	timeout.tv_sec = 3 * 60;
	timeout.tv_usec = 0;

	memcpy(&current_socks, &socks, sizeof(socks));
//	memset((char *) &connectList, -1, sizeof(connectList)); //changer par ft_memset
	while (1)
	{
		std::cout << "Waiting select..." << std::endl;
//		socks = setSelectList(server_fd, socks);
		if ((readable = select(highsock+1, &current_socks, NULL, NULL, &timeout)) < 0)
		{
			std::cout << "Error with select" << std::endl;
			return -1;
		}
		if (readable == 0)
		{
			std::cout << "Time out" << std::endl;
			return 0;
		}
		read_socks(server_fd, current_socks, socks);



//		if ((new_connexion = accept(server_fd, (struct sockaddr *)sock_addr, (socklen_t*)&addrlen)) < 0)
//		{
//			std::cerr << "Accept failed" << std::endl;
//			return (-1);
//		}
//
//		//get request
//		recv(new_connexion, &buffer, BUFFER_SIZE, 0);
//		std::cout << buffer << std::endl;
//
//		//parsing request
//		std::string requestStr(buffer);
//		Request request(requestStr, sock_addr);
//		std::cout << request;
//
///*
//	parsing_request();
//*/
//
//		//send response
//		response = "Server response\n";
//		send(new_connexion, response.c_str(), response.size(), 0);
//		close(new_connexion);
	}
	return 0;
}


