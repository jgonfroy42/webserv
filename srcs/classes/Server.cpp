#include "Server.hpp"
#include "../../includes/webserv.hpp"
#include <sstream>

// ********** SERVER CONSTRUCTOR / DESTRUCTOR ************ //

Server::Server()
{
	_id = -1;
}

Server::Server(Server const &src)
{
	*this = src;
}

Server::Server(string server_config, int id)
{
	this->_id = id;
	// try {
		set_host_port(server_config);
		set_server_names(server_config);
	// } catch (string msg)
	// { throw(msg); }
	
	set_locations(&server_config);
	set_root(server_config);
	set_indexes(server_config);
	set_body_max_size(server_config);
	set_error_pages(server_config);
}

Server &Server::operator=(Server const &rhs)
{
	if (this != &rhs)
	{
		_id = rhs.get_id();
		_port = rhs.get_port();
		_port_str = rhs.get_port_str();
		_host = rhs.get_host();
		_host_port = rhs.get_host_port();
		_server_names = rhs.get_server_names();
		_client_max_body_size = rhs.get_client_max_body_size();
		_root = rhs.get_root();
		_index = rhs.get_index();
		_error_pages = rhs.get_error_pages();
		_locations = rhs.get_locations();

	}
	return *this;
}

Server::~Server()
{}



// ********** SERVER GETTER ************ //

int			Server::get_id() const
{ return (this->_id); }

int		Server::get_port() const
{ return (this->_port); }

string	Server::get_host() const
{ return (this->_host); }

string	Server::get_host_port() const
{ return (this->_host_port); }

string	Server::get_port_str() const
{ return (this->_port_str); }

std::vector<string>	Server::get_server_names() const
{ return (this->_server_names); }

string	Server::get_root() const
{ return (this->_root); }

int		Server::get_client_max_body_size() const
{ return (this->_client_max_body_size); }

std::vector<string>	Server::get_index() const
{ return (this->_index); }

std::vector<Location> Server::get_locations() const
{ return (this->_locations); }

std::map<string, string> Server::get_error_pages() const
{ return (this->_error_pages); }

string Server::get_error_path(string error_code) const
{
	if ( this->_error_pages.find(error_code) == this->_error_pages.end() )
		return ("");
	else
		return (this->_error_pages.at(error_code));
}

Location	Server::server_contains_location(string path) const
{
	Location loc;
	std::vector<Location> locations = this->get_locations();
	for (std::vector<Location>::iterator it = locations.begin() ; it != locations.end(); ++it)
	{
		if (path.find(it->get_path(), 0) == 0 && it->get_path().size() > loc.get_path().size())
			loc = *it;
	}
	return (loc);
}


// ************* SERVER PARSING / SETTER ********** //

string	Server::get_configuration(const string server_config, const string label, bool optional)
{
	size_t label_pos = server_config.find(label);

	if (label_pos == string::npos && optional == false)
		throw string("Missing " + label);
	else if (label_pos == string::npos && optional == true)
		return ("");

	size_t end_line_pos = server_config.find(";", label_pos);
	if (end_line_pos == string::npos)
		throw string("Missing ; at end of line");
		// error_bad_config("Missing ; at end of line.");

	string line = server_config.substr(label_pos, end_line_pos - label_pos);
	
	// Check si doublon
	if (server_config.find(label, end_line_pos + 1) != string::npos)
		throw string("Double " + label + ".");
		// error_bad_config("Double " + label + ".");
	return (line);
}

void	Server::set_host_port(const string server_config)
{
	string port_line;
	port_line = get_configuration(server_config, "listen", false);
	size_t split_pos = port_line.find(' ');
	if (split_pos == string::npos)
		throw string("Invalid instruction. (listen)");
	size_t separator_pos = port_line.find(':', split_pos);
	if (separator_pos == string::npos)
		throw string("Missing port.");
	this->_host = port_line.substr(split_pos + 1, separator_pos - split_pos - 1);
	this->_host_port = port_line.substr(split_pos + 1);
	this->_port_str = port_line.substr(separator_pos + 1);
	std::istringstream iss (port_line.substr(separator_pos + 1));
	iss >> this->_port;
}

void Server::set_server_names(const string server_config)
{
	string name_line = get_configuration(server_config, "server_name", true);
	if (name_line == "")
		return ;
	size_t split_pos = 0;
	while ((split_pos = name_line.find(' ', split_pos + 1)) != string::npos)
	{
		if (name_line.find(' ', split_pos + 1) == string::npos)
			this->_server_names.push_back(name_line.substr(split_pos + 1));
		else
			this->_server_names.push_back(name_line.substr(split_pos + 1, name_line.find(' ', split_pos + 1) - split_pos - 1));
	}
	if (this->_server_names.empty() == true || this->_server_names.at(0) == "")
		throw string("Invalid instruction. (server name)");
}

void Server::set_error_pages(const string server_config)
{
	size_t error_page_pos = 0;
	while ((error_page_pos = server_config.find("error_page", error_page_pos + 1)) != string::npos)
	{
		size_t end_line_pos = server_config.find(";", error_page_pos);
		if (end_line_pos == string::npos)
			throw string("Missing ; at end of line.");
		string line = server_config.substr(error_page_pos, end_line_pos - error_page_pos);
		size_t start_path = line.find_last_of(" ");
		string path = line.substr(start_path + 1);
		size_t start_code = line.find(" ");
		size_t end_code = start_code + 1;
		while ((end_code = line.find(" ", end_code + 1)) != string::npos)
		{
			string code = line.substr(start_code + 1, end_code - start_code - 1);
			this->_error_pages[code] = path;
			start_code = end_code;
		}
	}
}

void Server::set_locations(string *server_config)
{
	std::vector<size_t> location = find_block(*server_config, 1);
	int id_location = 0;
	while (location.empty() == false)
	{
		if (get_block_type(*server_config, location[0]) != "location")
			throw string("Invalid block.");
		size_t start_location = (*server_config).find("location", 0) + 7;
		Location new_location((*server_config).substr(start_location + 2, location[1] - start_location - 2), id_location);
		this->_locations.push_back(new_location);
		(*server_config).erase(start_location - 8, location[1] - start_location + 9);
		location = find_block(*server_config, 0);
		id_location++;
	}
}

void	Server::set_root(const string server_config)
{
	string root_line = get_configuration(server_config, "root", false);
	if (root_line != "")
	{
		size_t  split_pos = root_line.find(' ');
		if (split_pos == string::npos)
			throw string("Invalid instruction. (root)");
		this->_root = root_line.substr(split_pos + 1);
		if (this->_root == "")
			throw string("Invalid instruction. (root)");
	}
}

void	Server::set_indexes(const string server_config)
{
	string index_line = get_configuration(server_config, " index ", true);
	if (index_line != "")
	{
		size_t split_pos = 0;
		while ((split_pos = index_line.find(' ', split_pos + 1)) != string::npos)
		{
			if (index_line.find(' ', split_pos + 1) == string::npos)
				this->_index.push_back(index_line.substr(split_pos + 1));
			else
				this->_index.push_back(index_line.substr(split_pos + 1, index_line.find(' ', split_pos + 1) - split_pos - 1));
		}
		if (this->_index.empty() == true || this->_index.at(0) == "")
			throw string("Invalid instruction. (index)");
	}
}

void	Server::set_body_max_size(const string server_config)
{
	string max_size_line = get_configuration(server_config, "client_max_body_size", true);
	if (max_size_line != "")
	{
		size_t split_pos = max_size_line.find(' ');
		if (split_pos == string::npos || split_pos + 1 == max_size_line.size())
			throw string("Invalid instruction. (client max body size)");
		std::istringstream iss (max_size_line.substr(split_pos + 1));
		iss >> this->_client_max_body_size;
	}
	else 
		this->_client_max_body_size = 8000;
}




// ************ << OPERATOR **************** //

std::ostream &			operator<<( std::ostream & o, Server const & i )
{
	o << std::endl << "--- SERVER CONFIGURATION ---" << std::endl << std::endl;
	o << "Id:\t\t" << i.get_id() << std::endl;
	o << "Host:\t\t" << i.get_host() << std::endl;
	o << "Port:\t\t" << i.get_port_str() << std::endl;
	o << "Host_Port:\t" << i.get_host_port() << std::endl;
	o << "Names:\t\t";
	std::vector<string> names = i.get_server_names();
	for (std::vector<string>::iterator it = names.begin() ; it != names.end(); ++it)
		o << *it << " ";
	o << std::endl;
	o << "Root:\t\t" << i.get_root() << std::endl;
	std::vector<string> index = i.get_index();
	if (index.empty() == false)
	{
		o << "Index:\t\t";
		for (std::vector<string>::iterator it_index = index.begin(); it_index != index.end(); ++it_index)
			o << *it_index << " ";
		o << std::endl;
	}
	else
		o << "Index:\t\t[none]\n";
	o << "Max body size:\t" << i.get_client_max_body_size() << std::endl;
	
	map_str_str error_pages = i.get_error_pages();
	if (error_pages.empty() == false)
	{
		o << "Error pages:\n";
		for(map_str_str::const_iterator it_error = error_pages.begin(); it_error != error_pages.end(); ++it_error)
			std::cout << "   " << it_error->first << " " << it_error->second << "\n";
	}
	
	std::vector<Location> locations = i.get_locations();
	for (std::vector<Location>::iterator it = locations.begin() ; it != locations.end(); ++it)
		o << *it;
	o << std::endl;
	
	return o;
}

