/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgonfroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 14:00:08 by jgonfroy          #+#    #+#             */
/*   Updated: 2021/06/03 16:13:11 by jgonfroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
#define	WEBSERV_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <iostream>
#include <cstring>
#include <errno.h>

#include "./../srcs/classes/request.hpp"

#define PORT 9000
#define PENDING_MAX 3
#define BUFFER_SIZE 3000

/*set_communication.cpp*/

int		setNonBlocking(int fd);
fd_set	setSelectList(int fd, fd_set socks);
int		init_server(sockaddr_in *sock_addr);
int		start_connexion(int server_fd, sockaddr_in *sock_addr);

/*set_communication.cpp*/
void	read_socks(int server_fd, fd_set socks, fd_set server_sock);
int		get_data(int i, fd_set server, struct sockaddr_in6 addr);

#endif
