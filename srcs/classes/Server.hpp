#ifndef SERVER_HPP
# define SERVER_HPP

# include "Location.hpp"

# include <iostream>
# include <string>
# include <sys/types.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <map>
# include <fcntl.h>
# include <errno.h>
# include <fstream>
# include <vector>

class Location;

typedef std::string string;
typedef std::map<std::string, std::string> map_str_str;
typedef std::vector<string> vec_string;
typedef std::vector<Location> vec_location;

class Server
{
	public:
		Server(string server_config, int id);
		~Server();

		int			get_id() const;
		int			get_port() const;
		string		get_port_str() const;
		string		get_host() const;
		string		get_host_port() const;
		vec_string	get_server_names() const;
		string		get_root() const;
		int			get_client_max_body_size() const;
		vec_string	get_index() const;
		string		get_error_page(string error_code) const;
		map_str_str get_error_pages() const;
		vec_location get_locations() const;
		Location	server_contains_location(string path) const;
	
	private:
		int				_id;
		int				_port;
		string			_port_str;
		string			_host;
		string			_host_port;
		vec_string		_server_names;
		int				_client_max_body_size;
		string			_root;
		vec_string		_index;
		map_str_str		_error_pages;
		vec_location	_locations;

		void	set_host_port(const string server_config);
		void	set_server_names(const string server_config);
		void	set_error_pages(const string server_config);
		void	set_locations(string *server_config);
		void	set_root(const string server_config);
		void	set_indexes(const string server_config);
		void	set_body_max_size(const string server_config);

		string	get_configuration(const string server_config, const string label, bool optional);

};

std::ostream &	operator<<( std::ostream & o, Server const & i );

#endif
