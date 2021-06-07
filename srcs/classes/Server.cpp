#include "Server.hpp"
#include "../../webserv.hpp"

string	get_configuration(const string server_config, const string label, bool optional)
{
	size_t label_pos = server_config.find(label);

	if (label_pos == string::npos && optional == false)
		error_bad_config();
	else if (label_pos == string::npos && optional == true)
		return ("");

	size_t end_line_pos = server_config.find(";", label_pos);
	if (end_line_pos == string::npos)
		error_bad_config();

	string line = server_config.substr(label_pos, end_line_pos - label_pos);
	
	// Check si doublon
	if (server_config.find(label, end_line_pos + 1) != string::npos)
		error_bad_config();
	return (line);
}

// ********** LOCATION CLASS ************ //

Server::Location::Location(const string location_config)
{
	size_t split_pos;

	// Checking if not another block inside location block
	size_t start_block = location_config.find('{', 0);
	if (location_config.find('{', start_block + 1) != string::npos)
		error_bad_config();
	
	// Setting path
	this->_path = location_config.substr(0, location_config.find(" "));
	
	// Setting root
	string root_line = get_configuration(location_config, "root", false);
	if (root_line != "")
	{
		split_pos = root_line.find(' ');
		if (split_pos == string::npos)
			error_bad_config();
		this->_root = root_line.substr(split_pos + 1);
	}
	
	// Setting index
	string index_line = get_configuration(location_config, " index ", true);
	if (index_line != "")
	{
		split_pos = 0;
		while ((split_pos = index_line.find(' ', split_pos + 1)) != string::npos)
		{
			if (index_line.find(' ', split_pos + 1) == string::npos)
				this->_index.push_back(index_line.substr(split_pos + 1));
			else
				this->_index.push_back(index_line.substr(split_pos + 1, index_line.find(' ', split_pos + 1) - split_pos));
		}
		if (this->_index.empty() == true)
			error_bad_config();
	}
	
	// Setting methods
	
	// Setting auto index
	string auto_index_line = get_configuration(location_config, "auto_index", true);
	if (auto_index_line != "")
	{
		split_pos = auto_index_line.find(' ');
		if (split_pos == string::npos)
			error_bad_config();
		string boolean = auto_index_line.substr(split_pos + 1);
		if (boolean == "on")
			this->_auto_index = true;
		else
			this->_auto_index = false;
	}
	else
		this->_auto_index = false;
}

Server::Location::~Location()
{}

// ************* LOCATION GETTER ********** //

string Server::Location::get_path() const
{
	return (this->_path);
}

string Server::Location::get_root() const
{
	return (this->_root);
}

std::vector<string> Server::Location::get_index() const
{
	return (this->_index);
}

bool	Server::Location::auto_index_is_on() const
{
	return (this->_auto_index);
}

// ************* SERVER PARSING / SETTER ********** //


void	Server::set_host_port(const string server_config)
{
	string port_line = get_configuration(server_config, "listen", false);
	size_t split_pos = port_line.find(' ');
	if (split_pos == string::npos)
		error_bad_config();
	size_t separator_pos = port_line.find(':', split_pos);
	if (split_pos == string::npos)
		error_bad_config();
	this->_host = port_line.substr(split_pos + 1, separator_pos - split_pos - 1);
	this->_port = stoi(port_line.substr(separator_pos + 1));
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
			this->_server_names.push_back(name_line.substr(split_pos + 1, name_line.find(' ', split_pos + 1) - split_pos));
	}
	if (this->_server_names.empty() == true)
		error_bad_config();
}

// ********** SERVER CONSTRUCTOR / DESTRUCTOR ************ //

Server::Server(string server_config)
{
	// std::cout << server_config << std::endl << std::endl;
	// setting port
	set_host_port(server_config);
	set_server_names(server_config);

	std::vector<size_t> location = find_block(server_config, 1);
	while (location.empty() == false)
	{
		if (get_block_type(server_config, location[0]) != "location")
			error_bad_config();
		size_t start_location = server_config.find_last_of("location", location[0]);
		Location new_location(server_config.substr(start_location + 2, location[1] - start_location - 2));
		this->_locations.push_back(new_location);
		server_config.erase(start_location, location[1] - start_location - 2);
		std::cout << std::endl << server_config << std::endl << std::endl;
		// erase location in config
		location = find_block(server_config, location[1]);
	}
}

Server::~Server()
{
}

// ********** SERVER GETTER ************ //

int		Server::get_port() const
{
	return (_port);
}

string	Server::get_host() const
{
	return (_host);
}

std::vector<string>	Server::get_server_names() const
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

std::vector<string>	Server::get_index() const
{
	return (_index);
}

std::vector<Server::Location> Server::get_locations() const
{
	return (_locations);
}



// ************ << OPERATOR **************** //

std::ostream &			operator<<( std::ostream & o, Server const & i )
{
	o << std::endl << "--- SERVER CONFIGURATION ---" << std::endl << std::endl;
	o << "Host: " << i.get_host() << std::endl;
	o << "Port: " << i.get_port() << std::endl;
	o << "Server names:";
	std::vector<string> names = i.get_server_names();
	for (std::vector<string>::iterator it = names.begin() ; it != names.end(); ++it)
		o << " " << *it;
	o << std::endl;
	std::vector<Server::Location> locations = i.get_locations();
	for (std::vector<Server::Location>::iterator it = locations.begin() ; it != locations.end(); ++it)
	{
		o << *it;
	}
	return o;
}

std::ostream &			operator<<( std::ostream & o, Server::Location const & i )
{
	o << std::endl << "- Location " << i.get_path() << std::endl;
	o << "    Root: " << i.get_root() << std::endl;
	std::vector<string> loc_index = i.get_index();
	if (loc_index.empty() == false)
	{
		o << "    Index:";
		for (std::vector<string>::iterator it_index = loc_index.begin(); it_index != loc_index.end(); ++it_index)
		{
			o << " " << *it_index;
		}
		o << std::endl;
	}
	o << "    Auto index: " << i.auto_index_is_on() << std::endl;
	return o;
}