#include "Server.hpp"
#include "../../webserv.hpp"

string	get_configuration(const string server_config, const string label)
{
	size_t label_pos = server_config.find(label);
	if (label_pos == string::npos)
		error_bad_config();
	size_t end_line_pos = server_config.find(";", label_pos);
	if (end_line_pos == string::npos)
		error_bad_config();
	string line = server_config.substr(label_pos, end_line_pos - label_pos);
	if (server_config.find(label, end_line_pos + 1) != string::npos)
		error_bad_config();
	return (line);
}

void	Server::set_host_port(const string server_config)
{
	string port_line = get_configuration(server_config, "listen");
	size_t split_pos = port_line.find(' ');
	if (split_pos == string::npos)
		error_bad_config();
	size_t separator_pos = port_line.find(':', split_pos);
	if (split_pos == string::npos)
		error_bad_config();
	this->_host = port_line.substr(split_pos + 1, separator_pos - split_pos - 1);
	this->_port = stoi(port_line.substr(separator_pos + 1));
}

// ********** CONSTRUCTOR / DESTRUCTOR ************ //

Server::Server(const string server_config)
{
	// setting port
	set_host_port(server_config);
}

Server::~Server()
{
}

// ********** METHODES GET ************ //

int		Server::get_port() const
{
	return (_port);
}

string	Server::get_host() const
{
	return (_host);
}

string	Server::get_server_names() const
{
	return (_server_names);
}

string	Server::get_root() const
{
	return (_root);
}

int		Server::get_client_max_body_size() const
{
	return (_client_max_body_size);
}

string	Server::get_index() const
{
	return (_index);
}

// ********** LOCATION CLASS ************ //

Server::Location::Location(const string location_config)
{
	(void)location_config;
}

Server::Location::~Location()
{}


// ************ << OPERATOR **************** //

std::ostream &			operator<<( std::ostream & o, Server const & i )
{
	o << std::endl << "--- SERVER CONFIGURATION ---" << std::endl;
	o << "Host: " << i.get_host() << std::endl;
	o << "Port:" << i.get_port() << std::endl;
	o << std::endl;
	return o;
}