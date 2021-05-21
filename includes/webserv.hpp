/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgonfroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 14:00:08 by jgonfroy          #+#    #+#             */
/*   Updated: 2021/05/21 16:21:42 by jgonfroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
#define	WEBSERV_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <fcntl.h>
#include <string.h>

#include "./../srcs/classes/request.hpp"

#define PORT 9000
#define PENDING_MAX 10
#define BUFFER_SIZE 3000

/*set_communication.cpp*/

int		setNonBlocking(int fd);
fd_set	setSelectList(int fd, fd_set socks);
int		init_server(sockaddr_in *sock_addr);
int		start_connexion(int server_fd, sockaddr_in *sock_addr);

/*set_communication.cpp*/
void	read_socks(int server_fd, fd_set socks);

#endif
