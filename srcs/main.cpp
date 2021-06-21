#include "./../includes/webserv.hpp"

int	main(int argc, char** argv)
{
	t_param_server *param = new t_param_server;
	std::vector<Server> servers;

	const char *default_config_path = "config/default.conf";
	if (argc == 2)
		servers = parsing_config(argv[1]);
	else
		servers = parsing_config(default_config_path);

	generate_autoindex("/srcs", "/srcs");

	//a changer avec la conf
	param->timeout.tv_sec = 3600;
	param->timeout.tv_usec = 0;
	// if (init_server(param))
	// {
	// 	delete param;
	// 	return -1;
	// }
	// launch_server(param);
	// delete param;
}
