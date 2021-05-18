#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <iostream>
# include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <map>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


typedef std::string string;
typedef std::map<std::string, std::string> map;

class Request
{

	public:

		Request();
		Request(string requestStr, const sockaddr_in client_addr);
		Request( Request const & src );
		~Request();

		Request &		operator=( Request const & rhs );

	private:
		string		_method;
		map			_headers;
		string		_body;
		map			_CGI_env;
		sockaddr_in	_client_addr; //useful pour le REMOTE_ADDR?
		socklen_t	_addr_len; //idem: useful?
};

std::ostream &			operator<<( std::ostream & o, Request const & i );

map		getCGIEnv(const string request, const sockaddr_in client_addr);

#endif /* ********************************************************* REQUEST_H */