#include "./../includes/webserv.hpp"

int	main(int argc, char** argv)
{
	t_param_server *param = new t_param_server;
	std::vector<Server> servers;
	std::vector<int> ports;
	std::vector<int> socketID; //remplacer par stack ?
	std::vector<int>::iterator it;

	const char *default_config_path = "config/double.conf";
	if (argc == 2)
		servers = parsing_config(argv[1]);
	else
		servers = parsing_config(default_config_path);

	ports = get_ports(servers);
	for (it = ports.begin(); it != ports.end(); ++it)
	{
		param->port = *it;
		if (init_server(param))
		{
			delete param; //remplacer pour delete liste chaine;
			return -1;
		}
		socketID.push_back(param->socketId);
	}
	launch_server(socketID);
	delete param;
}
