#include "Server.hpp"

// ********** CONSTRUCTOR / DESTRUCTOR ************ //

Server::Server(const string server_config)
{
}

Server::~Server()
{
}

// ********** METHODES GET ************ //

int		Server::get_port() const
{
	return (_port);
}

string	Server::get_server_name() const
{
	return (_server_name);
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
}