#ifndef SERVER_HPP
# define SERVER_HPP

# include "../../webserv.hpp"

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

		int			get_port() const;
		string		get_host() const;
		vec_string	get_server_names() const;
		string		get_root() const;
		int			get_client_max_body_size() const;
		vec_string	get_index() const;
		string		get_error_page(string error_code) const;
		map_str_str get_error_pages() const;
		vec_location get_locations() const;
	
	private:
		int				_id;
		int				_port;
		string			_host;
		vec_string		_server_names;
		int				_client_max_body_size;
		string			_root;
		vec_string		_index;
		map_str_str		_error_pages;
		vec_location	_locations;

		void	set_host_port(const string server_config);
		void	set_server_names(const string server_config);
		void	set_error_pages(const string server_config);

		string	get_configuration(const string server_config, const string label, bool optional);

};

std::ostream &	operator<<( std::ostream & o, Server const & i );

#endif