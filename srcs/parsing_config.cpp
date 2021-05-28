#include "../webserv.hpp"


void	find_server_block(string config)
{
	size_t start_block_server;
	size_t end_block_server;
	size_t location_block = 0;

	// Trouver le premier block
	start_block_server = config.find_first_of("server {");
	if (start_block_server == string::npos)
	{
		std::cerr << "Error: Bad configuration file.\n";
		exit (1);
	}
	std::cout << "debut = " << start_block_server << std::endl;
	
	// Chercher s'il y a un deuxieme block server
	if (config.find("server {", start_block_server + 7) != string::npos)
	{
		std::cout << "several server !" << std::endl;
		while ((config.find("location ", location_block)) != string::npos)
		{
			location_block = config.find("location ", location_block);
			std::cout << "location = " << location_block;
			location_block = config.find("}", location_block);
			if (location_block == string::npos)
			{
				std::cerr << "Error: Bad configuration file.\n";
				exit (1);
			}
			std::cout << " - " << location_block << std::endl;
		}
		std::cout << location_block << std::endl;
		// if (location_block)
		// look for end of first server here
	}
	else // Si pas de deuxième block server
	{
		end_block_server = config.find_last_of("}");
		if (end_block_server == string::npos)
		{
			std::cerr << "Error: Bad configuration file.\n";
			exit (1);
		}
		std::cout << "end = " << end_block_server << std::endl;
		// return block here
	}
	
	// while ((location_block = config.find("location ", location_block)) != string::npos)
	// {
	// 	std::cout << "location = " << location_block;
	// 	location_block = config.find("}", location_block);
	// 	std::cout << " - " << location_block << std::endl;
	// }
}

bool is_double_space(char lhs, char rhs) { return (lhs == rhs) && (lhs == ' '); }

void	parsing_config(char *config_file)
{
	int file_fd;
	char *config_read;
	int ret_read;

	file_fd = open(config_file, O_RDONLY);
	if (file_fd == -1)
	{
		std::cerr << "Error: Couldn't open configuration file." << std::endl;
		exit (1);
	}
	config_read = (char*)malloc(sizeof(char) * 8000);
	ret_read = read(file_fd, config_read, 8000);
	if (ret_read == -1 || ret_read == 0)
	{
		std::cerr << "Error: Couldn't read configuration file." << std::endl;
		free(config_read);
		close(file_fd);
		exit (1);
	}

	string config(config_read);
	free(config_read);
	close(file_fd);

	// std::cout << config << std::endl;
	
	// Transforming all new line and tabulations into spaces
	for (string::iterator it = config.begin(); it != config.end(); it++)
	{
		if (*it == '\n')
			config.replace(it, it + 1, " ");
		if (*it == '\t')
			config.replace(it, it + 1, " ");
	}
	
	// Transforming all multiple white spaces into one white space
	std::string::iterator new_end = std::unique(config.begin(), config.end(), is_double_space);
	config.erase(new_end, config.end());

	std::cout << config << std::endl;
	find_server_block(config);
	
}

