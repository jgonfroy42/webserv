#include "request.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

void	initHeaders(map headers)
{
	headers["Accept-Charset"] = string();
	headers["Accept-Language"] = string();
	headers["Authorization"] = string();
	headers["Content-Length"] = string();
	headers["Content-Type"] = string();
	headers["Date"] = string();
	headers["Host"] = string();
	headers["Referer"] = string();
	headers["Transfer-Encoding"] = string();//also called TE
	headers["User-Agent"] = string();
}


map		getRequestHeaders(const string requestStr, const map CGIEnv)
{
	map	headers;


	return headers;
}

Request::Request()
{
}

Request::Request(const string requestStr, const sockaddr_in client_addr)
{
	_CGI_env = getCGIEnv(requestStr, client_addr);
	_method = _CGI_env["REQUEST_METHOD"];
	_headers = getRequestHeaders(requestStr, _CGI_env);
	_body;
	_client_addr = client_addr; //useful pour le REMOTE_ADDR pas sous forme de str car fonction pour l'avoir sous forme de str pas autorisee?
	//_addr_len; //idem: useful?
}

class Request	parse_request(string request_str)
{
	
}

Request::Request( const Request & src )
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Request::~Request()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Request &				Request::operator=( Request const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Request const & i )
{
	//o << "Value = " << i.getValue();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */