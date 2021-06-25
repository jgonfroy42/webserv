#include "../../includes/webserv.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Request::Request()
{
}

Request::Request(const char *request_array)
{
	_chunked = false;
	_chunked_error = false;
	_body = parse_body(request_array);
	string request_string = string(request_array);
	string start_line = string(request_string, 0, request_string.find('\n'));
	parse_start_line(request_string);
	_translated_path = string();
	_headers = parse_headers(request_string);

	_host_port = string(_headers["Host"]);
	size_t pos;
	if ((pos = _host_port.find(":")) != string::npos)
	{
		_host = string(_host_port, 0, pos);
		_port = string(_host_port, pos + 1);
		while (!isdigit(_port[_port.size() - 1]))
			_port = string(_port, 0, _port.size() - 1);
	}
	else
	{
		_host = _host_port;
		_port = string();
	}

	std::string encoding;
	encoding = string(_headers["Transfer-Encoding"]);
	if (encoding == "chunked\r")
		_chunked = true;
}

Request::Request(const Request &src)
{
	std::cout << "copy constructor" << std::endl;
	//	_CGI_env = src._CGI_env;
	_chunked = src._chunked;
	_chunked_error = src._chunked_error;
	_method = src._method;
	_headers = src._headers;
	_host_port = src.get_host_port();
	_host = src.get_host();
	_port = src.get_port();
	_body = src._body;
	_path = src._path;
	_translated_path = src._translated_path;
	std::cout << "end of copy constructor" << std::endl;
}

Request::Request(Request const &src, string body)
{
	std::cout << "copy constructor with new body" << std::endl;
	_method = src._method;
	_headers = src._headers;
	_host_port = src.get_host_port();
	_host = src.get_host();
	_port = src.get_port();
	_path = src._path;
	_translated_path = src._translated_path;
	_body = body;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Request::~Request()
{
	//delete _body;
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Request &Request::operator=(Request const &rhs)
{

	if (this != &rhs)
	{
		_method = rhs.get_method();
		_headers = rhs.get_headers();
		_body = rhs.get_body();
		_host_port = rhs.get_host_port();
		_host = rhs.get_host();
		_port = rhs.get_port();
		_path = rhs.get_path();
		_translated_path = rhs.get_translated_path();
	}
	return *this;
}

std::ostream &operator<<(std::ostream &o, Request const &i)
{
	o << "--- REQUEST CLASS CONTENT ---" << std::endl;
	o << "Method: " << i.get_method() << std::endl
	  << "URI: " << i.get_URI() << std::endl
	  << "Path: " << i.get_path() << std::endl
	  << "Query string: " << i.get_query_string() << std::endl
	  << "Protocol: " << i.get_protocol() << std::endl
	  << "Host: " << i.get_host() << std::endl
	  << "Port: " << i.get_port() << " of size " << i.get_port().size() << std::endl
	  << "Headers:" << std::endl;
	displayMap(i.get_headers());
	o << std::endl
	  << "Body:" << std::endl
	  << i.get_body() << std::endl;
	o << "---end of request class--" << std::endl;

	return o;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

int get_header_value(map_str_str &headers, string request, string header)
{
	size_t pos = 0;
	int len = 0;
	if ((pos = request.find(header + ": ")) != string::npos)
	{
		pos += header.size() + 2;
		while (request[pos + len] && request[pos + len] != '\n')
			len++;
		headers[header] = string(request, pos, len);
	}
	return 0; //choix éditorial a confirmer
}

void init_headers(map_str_str &headers)
{
	headers["Accept-Charset"] = string();
	headers["Accept-Language"] = string();
	headers["Authorization"] = string();
	headers["Content-Length"] = string();
	headers["Content-Type"] = string();
	headers["Date"] = string();
	headers["Host"] = string();
	headers["Referer"] = string();
	headers["Transfer-Encoding"] = string(); //also called TE
	headers["User-Agent"] = string();
}

map_str_str Request::parse_headers(const string request_str)
{
	map_str_str headers;

	init_headers(headers);
	get_header_value(headers, request_str, "Accept-Charset");
	get_header_value(headers, request_str, "Accept-Language");
	get_header_value(headers, request_str, "Authorization");
	get_header_value(headers, request_str, "Content-Length");
	get_header_value(headers, request_str, "Content-Type");
	get_header_value(headers, request_str, "Date");
	get_header_value(headers, request_str, "Host");
	get_header_value(headers, request_str, "Referer");
	get_header_value(headers, request_str, "Transfer-Encoding");
	get_header_value(headers, request_str, "User-Agent");

	return headers;
}

string Request::parse_body(const char *request)
{
	//char *tmp;
	string requestStr = string(request);
	size_t pos;
	char separator[5] = {13, 10, 13, 10, 0}; //sequence of non printable chars that separate the headers of the body in a client request(chrome) + \0
	pos = requestStr.find(separator);

	if (pos == (requestStr.size() + 4) || pos == string::npos) //no body//+4 = taille du separator == on test si body vide
		return (string());
	else
		return (string(requestStr, pos + 4));
}

int Request::parse_start_line(string start_line)
{
	string::iterator it_begin = start_line.begin();
	string::iterator it_end = it_begin + start_line.find(" ");
	if (*it_begin && *it_end)
		_method = string(it_begin, it_end++);
	it_begin = it_end;
	while (*it_end && *it_end != ' ')
		it_end++;
	if (*it_begin && *it_end)
		_URI = string(it_begin, it_end++);
	size_t pos;
	size_t len = 0;
	if ((pos = _URI.find('?')) != string::npos)
	{
		_path = string(_URI, 1, pos - 1);
		pos += 1;
		while (start_line[pos + len] && start_line[pos + len] != ' ')
			len++;
		_query_string = string(_URI, pos, len);
	}
	else if (_URI != string())
	{
		_path = string(_URI, 1, string::npos);
		_query_string = string();
	}
	it_begin = it_end;
	while (*it_end && *it_end != '\n')
		it_end++;
	if (*it_begin && *it_end)
		_protocol = string(it_begin, it_end);
	if ((pos = start_line.find("cgi-bin/myscript.cgi")) != string::npos)
	{ //NB: CGI PATH EN STATIQUE
		len = 0;
		pos += string("cgi-bin/myscript.cgi").size() + 1; //NB: CGI PATH EN STATIQUE
		while (start_line[pos + len] && start_line[pos + len] != ' ' && start_line[pos + len] != '?')
			len++;
		_path = string(start_line, pos, len);
	}
	return 0;
}

bool Request::is_method_valid() const
{
	if (_method == "GET" || _method == "HEAD" || _method == "POST" || _method == "DELETE")
		return true;
	else
		return false;
}

bool Request::is_bad_request() const
{
	if (get_headers()["Host"] == string())
		return true;
	else
		return false;
}

void Request::append_root_to_path(string root)
{
	if (root == string())
		return;
	while (root.size() >= 1 && root[0] == ('.' | '/'))
	{
		//deletion of the "./" part at the beginning of root
		if (root[0] == '.')
			root.erase(0, 1);
		if (root.size() >= 1 && root[0] == '/')
			root.erase(0, 1);
	}
	_path = root + '/' + _path;
}

void Request::set_unchunked_body(string new_body)
{
	_body = new_body;
}

void Request::set_chunked_error(bool error)
{
	_chunked_error = error;
}

bool Request::is_CGI() const //NB: CGI PATH EN STATIQUE
{
	if (_method == "GET" && _URI.find("cgi-bin/myscript.cgi") != string::npos)
		return true;
	else
		return false;
}

bool	Request::is_chunked() const
{
	return _chunked;
}

bool	Request::is_chunked_false() const
{
	return _chunked_error;
}

string Request::get_method() const
{
	return (_method);
}

string Request::get_URI() const
{
	return (_URI);
}

string Request::get_path() const
{
	return (_path);
}

string Request::get_translated_path() const
{
	return (_translated_path);
}

string Request::get_query_string() const
{
	return (_query_string);
}

string Request::get_protocol() const
{
	return (_protocol);
}

map_str_str Request::get_headers() const
{
	return (_headers);
}

string Request::get_body() const
{
	return (_body);
}

string Request::get_host_port() const
{
	return (_host_port);
}

string Request::get_host() const
{
	return (_host);
}

string Request::get_port() const
{
	return (_port);
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

/* ************************************************************************** */
