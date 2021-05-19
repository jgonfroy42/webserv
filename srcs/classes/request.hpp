#ifndef REQUEST_HPP
# define REQUEST_HPP
# include <map>

typedef std::string string;
typedef std::map<std::string, std::string> map_str_str;

class Request
{

	public:

		Request();
		Request(string requestStr, const sockaddr_in *client_addr);
		Request( Request const & src );
		~Request();

		bool			is_method_valid() const;
		bool			is_CGI() const;
		string			get_method() const;
		map_str_str		*get_headers() const;
		string			get_body() const;
		map_str_str		*get_CGI_env() const;

		Request &		operator=( Request const & rhs );

	private:
		string			_method;
		map_str_str		*_headers;
		string			_body;
		map_str_str		*_CGI_env;
	//	sockaddr_in	*_client_addr; //useful pour le REMOTE_ADDR?
	//	socklen_t	_addr_len; //idem: useful?
};

std::ostream &			operator<<( std::ostream & o, Request const & i );


#endif /* ********************************************************* REQUEST_H */