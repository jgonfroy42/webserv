#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <map>
# include <vector>

typedef std::string string;
typedef std::map<std::string, std::string> map_str_str;

class Server
{
	public:
		Server(const string server_config);
		~Server();

		int		get_port() const;
		string	get_host() const;
		std::vector<string>	get_server_names() const;
		string	get_root() const;
		int		get_client_max_body_size() const;
		string	get_index() const;
		string	get_error_page(string error_code) const;
	
	private:
		class Location 
		{
			public:
				Location(const string location_config);
				~Location();
				bool	allows_method(const string method);
				string	generate_directory_listing();

			private:
				string	_path;
				string	_root;
				string	_index; // vector ??
				std::map<std::string, bool> _allowed_methods;
				bool	_auto_index;
				string	_default_answer; // ??
		};

		int			_port; // pas int ???
		string		_host;
		std::vector<string>		_server_names; // vector ??
		int			_client_max_body_size;
		string		_root;
		string		_index;  // vector ??
		map_str_str	_error_pages;
		std::vector<Location> _locations; // ??

		void	set_host_port(const string server_config);
		void	set_server_names(const string server_config);

};

std::ostream &			operator<<( std::ostream & o, Server const & i );

#endif