#include "request.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Request::Request()
{
}

Request::Request(const string requestStr, const sockaddr_in *client_addr)
{
	_CGI_env = getCGIEnv(requestStr, client_addr);
	_method = _CGI_env["REQUEST_METHOD"];
	_headers = getRequestHeaders(requestStr);
	_body = getBody(requestStr);	
//	_client_addr = client_addr; //useful pour le REMOTE_ADDR pas sous forme de str car fonction pour l'avoir sous forme de str pas autorisee?
	//_addr_len; //idem: useful?
}

Request::Request( const Request & src )
{
	(void)src;
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
	(void)rhs;
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Request const & i )
{
	o << "--- REQUEST CLASS CONTENT ---" << std::endl;
	o << "Method: " << i.get_method() << std::endl << std::endl;
	o << "Headers:" << std::endl;
	displayMap(i.get_headers());
	o << std::endl << "Body:" << std::endl << i.get_body() << std::endl;	
	o << std::endl << "CGI variables:" << std::endl;
	displayMap(i.get_CGI_env());
	o << std::endl;	

	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/
string	Request::get_method() const
{
	return(_method);
}

map		Request::get_headers() const
{
	return(_headers);
}

string	Request::get_body() const
{
	return(_body);
}

map		Request::get_CGI_env() const
{
	return(_CGI_env);
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */