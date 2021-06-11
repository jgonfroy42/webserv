#ifndef LOCATION_HPP
# define LOCATION_HPP

#include "../../webserv.hpp"

#include <utility>
typedef std::pair<string, string> pair;
typedef std::vector<string> vec_string;
typedef std::map<std::string, bool> map_str_bool;

class Location 
{
	public:
		Location(const string location_config, int id);
		~Location();

		int			get_id() const;
		bool		method_is_allowed(const string method) const;
		string		get_path() const;
		string		get_root() const;
		vec_string	get_index() const;
		bool		auto_index_is_on() const;
		pair		get_redirect() const;
		string		get_cgi_path() const;

	private:
		int				_id;
		string			_path;
		string			_root;
		vec_string		_index;
		map_str_bool	_allowed_methods;
		bool			_auto_index;
		pair			_redirect;
		string			_cgi_path;

		void	set_root(const string location_config);
		void	set_indexes(const string location_config);
		void	set_methods(const string location_config);
		void	set_autoindex(const string location_config);
		void	set_redirection(const string location_config);
		void	set_cgi_path(const string location_config);

		string	get_configuration(const string server_config, const string label, bool optional);
};

std::ostream &	operator<<( std::ostream & o, Location const & i );

#endif