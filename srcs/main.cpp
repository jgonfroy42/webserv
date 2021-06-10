/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgonfroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 13:57:36 by jgonfroy          #+#    #+#             */
/*   Updated: 2021/06/10 13:47:22 by jgonfroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/webserv.hpp"

int	main(void)
{
	t_param_server *param = new t_param_server;

	//a changer avec la conf
	param->timeout.tv_sec = 3600;
	param->timeout.tv_usec = 0;
	if (init_server(param))
	{
		delete param;
		return -1;
	}
	launch_server(param);
	delete param;
}
