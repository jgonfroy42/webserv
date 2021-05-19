#include "../../webserv.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Request::Request()
{
}

Request::Request(const string requestStr, const sockaddr_in *client_addr)
{
	_CGI_env = new map_str_str(getCGIEnv(requestStr, client_addr));
	_method = (*_CGI_env)["REQUEST_METHOD"];
	_headers = new map_str_str(getRequestHeaders(requestStr));
	_body = getBody(requestStr);	
//	_client_addr = client_addr; //useful pour le REMOTE_ADDR pas sous forme de str car fonction pour l'avoir sous forme de str pas autorisee?
	//_addr_len; //idem: useful?
}

Request::Request( const Request & src )
{
	std::cout << "copy constructor" << std::endl;
	_CGI_env = src._CGI_env;
	_method = src._method;
	_headers = src._headers;
	_body = src._body;
		std::cout << "end of copy constructor" << std::endl;
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Request::~Request()
{
	delete _headers;
	delete _CGI_env;
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Request &				Request::operator=( Request const & rhs )
{

	if ( this != &rhs )
	{
		_CGI_env = rhs.get_CGI_env();
		_method = rhs.get_method();
		_headers = rhs.get_headers();
		_body = rhs.get_body();
	}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Request const & i )
{
	o << "--- REQUEST CLASS CONTENT ---" << std::endl;
	o << "Method: " << i.get_method() << std::endl << std::endl;
	o << "Headers:" << std::endl;
	displayMap(*i.get_headers());
	o << std::endl << "Body:" << std::endl << i.get_body() << std::endl;	
	o << std::endl << "CGI variables:" << std::endl;
	displayMap(*i.get_CGI_env());
	o << std::endl;	

	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

bool	Request::is_method_valid() const{
	if (_method == "GET" || _method == "HEAD" || _method == "POST"
		|| _method == "PUT" || _method == "DELETE" || _method == "CONNECT"
		|| _method == "OPTIONS" || _method == "TRACE")
		return true;
	else
		return false;
}

bool	Request::is_CGI() const{
	string request_uri = (*get_CGI_env())["REQUEST_URI"];
	string cgi_path = (*get_CGI_env())["SCRIPT_NAME"];

	if (_method == "GET" && request_uri.find(cgi_path) != string::npos)
		return true;
	else
		return false;
}

string	Request::get_method() const{
	return(_method);
}

map_str_str		*Request::get_headers() const{
	return(_headers);
}

string	Request::get_body() const{
	return(_body);
}

map_str_str		*Request::get_CGI_env() const{
	return(_CGI_env);
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */