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
	int on = 1;

	//creation du socket qui va recevoir les connections, besoin d'un socket par port
	if ((param->socketId = socket(AF_INET6, SOCK_STREAM, 0)) < 0)
	{
		std::cerr << "Error : cannot create socket" << std::endl;
		return -1;
	}
	if (setsockopt(param->socketId, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)) < 0)
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
	param->socketAddr.sin6_family = AF_INET6;
	memcpy(&param->socketAddr.sin6_addr, &in6addr_any, sizeof(in6addr_any));
	param->socketAddr.sin6_port = htons(param->port);
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

void launch_server(std::vector<int> socketID, std::vector<Server> &servers)
{
	int nb_readable, new_co;
	int nfds = socketID.size();
	struct pollfd *fds = NULL;

	fds = (struct pollfd *)calloc(nfds, sizeof(struct pollfd));
	int	i = 0;
	for (std::vector<int>::iterator it = socketID.begin(); it != socketID.end(); ++it)
	{
		fds[i].fd = *it;
		fds[i].events = POLLIN;
		++i;
	}
	while (1)
	{
		std::cout << "Waiting for connection..." << std::endl;
		if ((nb_readable = poll(fds, nfds, 36000)) < 0)
		{
			std::cerr << "Error: cannot select" << std::endl;
			return;
		}
		if (nb_readable == 0)
		{
			std::cout << "Time out" << std::endl;
			return;
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
			for (std::vector<int>::iterator it = socketID.begin(); it != socketID.end(); ++it)
			{
				if (fds[i].fd == *it)
				{
					do
					{
						new_co = accept(*it, NULL, NULL);
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
					break ;
					//si le poll correspondait a un socket on ne fait pas de read sur les connections
				}
			}
			if (get_data(fds[i].fd, servers))
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

int	get_data(int fd, std::vector<Server> &servers)
{
	int data_len;
	char buffer[BUFFER_SIZE]; //remplacer BUFFER_SIZE par max body client?

	memset(buffer, 0, BUFFER_SIZE);
	data_len = recv(fd, buffer, BUFFER_SIZE, 0);
	if (data_len < 0)
		return 0;
	if (data_len == 0)
	{
		std::cout << "Connection closed" << std::endl;
		return 1;
	}

	//parsing request
	std::cout << std::endl
			  << "---RAW REQUEST FROM CLIENT(requestStr):\n"
			  << buffer << "--end of requestStr--\n"
			  << std::endl;
	std::string requestStr(buffer);
	Request request(buffer);
	std::cout << request;

	//send response

	char *response = NULL;
	size_t response_size;
	response_size = build_response(request, &response, servers);

	//send response
	send(fd, response, response_size, 0);
	std::cout << std::endl
			  << "---RESPONSE SENT TO CLIENT:\n"
			  << response << "\n--end of response to client--\n"
			  << std::endl;

	delete response; //new dans build_response	 // a reverifier
//	if (chunked)
//		return 0;
	return 1;
}

/*		std::string response = "Server response\n";
//		std::string response = "HTTP/1.1 301 Moved Permanently\nLocation: http://www.google.com/\nContent-Type: text/html; charset=UTF-8\n \
		Date: Mon, 14 Jun 2021 14:47:11 GMT\nExpires: Wed, 14 Jul 2021 14:47:11 GMT\nCache-Control: public, max-age=2592000\nServer: gws\n \
		Content-Length: 219\nX-XSS-Protection: 0\nX-Frame-Options: SAMEORIGIN\n\n \
		HTML><HEAD><meta http-equiv=\"content-type\" content=\"text/html;charset=utf-8\">\n \
		<TITLE>301 Moved</TITLE></HEAD><BODY>\n<H1>301 Moved</H1>\nThe document has moved\n<A HREF=\"http://www.google.com/\">here</A>.\n</BODY></HTML>\n"; */
