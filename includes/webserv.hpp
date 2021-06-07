/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgonfroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 14:00:08 by jgonfroy          #+#    #+#             */
/*   Updated: 2021/06/07 16:40:31 by jgonfroy         ###   ########.fr       */
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

#include "../srcs/classes/request.hpp"

#define PORT 9000
#define BUFFER_SIZE 3000

typedef struct s_param_server
{
	int		socketId;
	fd_set	socket;
	timeval	timeout;
	struct sockaddr_in6	socketAddr;
}	t_param_server;

/*set_communication.cpp*/
int		init_server(t_param_server *param);
int		get_data(int i, fd_set server, struct sockaddr_in6 addr);

#endif
