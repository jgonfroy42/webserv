#include "../includes/webserv.hpp"
#include <sstream>
#include <string.h>
#include <algorithm>
#include "./classes/Server.hpp"

std::vector<int>	get_ports(std::vector<Server> servers)
{
	std::vector<int> ports;
	for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); ++it)
		ports.push_back(it->get_port());
	std::sort( ports.begin(), ports.end() );
	ports.erase( std::unique( ports.begin(), ports.end() ), ports.end() );
	return (ports);
}


void	error_bad_config(string error)
{
	std::cerr << "Error: Bad configuration file.\n\t-> " << error << std::endl;
	exit (1);
}

string	get_block_type(string config, size_t start_block)
{
	size_t end = config.find_last_of(" ", start_block);
	size_t start = config.find_last_of(" ", end - 1);
	if (start == string::npos)
		start = -1;
	string type = config.substr(start + 1, end - start - 1);
	if (type[0] == '/')
		return (get_block_type(config, start));
	else if (type != "server" && type != "location")
		return ("");
	return (type);
}

std::vector<size_t>	find_block(string config, size_t start_pos)
{
	size_t start_block;
	size_t end_block;
	size_t middle_start_block;
	size_t middle_end_block;
	std::vector<size_t> block;

	start_block = config.find('{', start_pos);
	if (start_block == string::npos)
		return (block);
	end_block = config.find('}', start_block);
	if (end_block == string::npos)
		return (block);
	middle_start_block = config.find('{', start_block + 1);
	while (middle_start_block < end_block && middle_start_block != string::npos)
	{
		middle_end_block = end_block;
		end_block = config.find('}', middle_end_block + 1);
		if (end_block == string::npos)
			return (block);
		middle_start_block = config.find('{', middle_start_block + 1);
	}
	block.push_back(start_block);
	block.push_back(end_block);
	return (block);
}

bool	valid_count_brackets(string s) {
	int count_open = 0;
	int count_close = 0;

	for (size_t i = 0; i < s.size(); i++)
	{
		if (s[i] == '{') count_open++;
		if (s[i] == '}') count_close++;
	}
	
	if (count_open != count_close)
		return (false);
	return (true);
}

bool is_double_space(char lhs, char rhs) { return (lhs == rhs) && (lhs == ' '); }

std::vector<Server>		parsing_config(const char *config_path)
{
	// Opening file
	std::ifstream config_file;
    config_file.open(config_path);
	if (config_file.fail())
	{
		std::cerr << "Error: " << strerror(errno) << std::endl;
		exit (1);
	}

	// Reading file
    std::stringstream config_read;
    config_read << config_file.rdbuf();
    std::string config = config_read.str();
	if (config.empty())
	{
		std::cerr << "Error: Couldn't read configuration file.\n";
		exit (1);
	}
	
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

	// Checking validity by number of brackets
	if (!valid_count_brackets(config))
		error_bad_config("Invalid number of brackets.");

	std::vector<Server> servers;
	std::vector<size_t> block = find_block(config, 0);
	int id_server = 0;
	while (block.empty() == false)
	{
		if (get_block_type(config, block[0]) != "server")
			error_bad_config("Not a server.");
		size_t start_block = config.find("server", 0) + 7;
		Server new_server(config.substr(start_block + 2, block[1] - start_block - 2), id_server);
		servers.push_back(new_server);
		config.erase(start_block - 6, block[1] - start_block + 7);
		block = find_block(config, 0);
		id_server++;
	}

	for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); ++it)
		std::cout << *it;
	get_ports(servers);
	return (servers);
}
