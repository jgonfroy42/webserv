#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <map>
# include <vector>

typedef std::string string;
typedef std::map<std::string, std::string> map_str_str;

// !!!!!!!!!! redirections + cgi

class Server
{
	public:
		Server(string server_config);
		~Server();

		int		get_port() const;
		string	get_host() const;
		std::vector<string>	get_server_names() const;
		string	get_root() const;
		int		get_client_max_body_size() const;
		std::vector<string>	get_index() const;
		string	get_error_page(string error_code) const;
		
		class Location 
		{
			public:
				Location(const string location_config);
				~Location();

				bool	method_is_allowed(const string method) const;
				// string	generate_directory_listing() const;  // here or somewhere else ???
				string	get_path() const;
				string	get_root() const;
				std::vector<string>	get_index() const;
				bool	auto_index_is_on() const;

			private:
				string	_path;
				string	_root;
				std::vector<string>	_index;
				std::map<std::string, bool> _allowed_methods;
				bool	_auto_index;
		};

		std::vector<Location> get_locations() const;
	
	private:
		int			_port;
		string		_host;
		std::vector<string>		_server_names;
		int			_client_max_body_size;
		string		_root;
		std::vector<string>		_index;
		map_str_str	_error_pages;
		std::vector<Location> _locations; // ??

		void	set_host_port(const string server_config);
		void	set_server_names(const string server_config);

};

std::ostream &	operator<<( std::ostream & o, Server const & i );
std::ostream &	operator<<( std::ostream & o, Server::Location const & i );

#endif