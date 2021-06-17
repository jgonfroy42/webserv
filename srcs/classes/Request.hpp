#ifndef REQUEST_HPP
#define REQUEST_HPP
#include <map>
#include <cstring>

typedef std::string string;
typedef std::map<std::string, std::string> map_str_str;

class Request
{
public:
	Request();
	Request(const char *request_str);
	Request(Request const &src);
	Request(Request const &src, string body);
	~Request();

	bool is_method_valid() const;
	bool is_CGI() const;

	//GETTERS:
	string get_method() const;
	string get_URI() const;
	string get_path() const;
	string get_query_string() const;
	string get_protocol() const;
	map_str_str get_headers() const;
	string get_body() const;

	Request &operator=(Request const &rhs);

private:
	string _body;
	string _method;
	string _URI;
	string _path;
	string _query_string;
	string _protocol; //=SERVER PROTOCOL pour CGI
	map_str_str _headers;
	int parse_start_line(string start_line);
	map_str_str parse_headers(const string request_str);
	string parse_body(const char *request_str);
};

std::ostream &operator<<(std::ostream &o, Request const &i);

#endif /* ********************************************************* REQUEST_H */
