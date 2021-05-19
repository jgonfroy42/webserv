#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <iostream>
# include <string>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <unistd.h>
# include <stdlib.h>
# include <map>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <limits.h>
# include <sys/time.h>
# include "./srcs/classes/request.hpp"
# include "./libft/libft.h"
# include <time.h>
# include <fcntl.h>
# include <sys/stat.h>

# define PORT 8080
# define PENDING_MAX 10
# define BUFFER_SIZE 3000

// STATUS CODES
# define OK				"200"
# define BAD_REQUEST	"400"
# define NOT_FOUND		"404"

typedef std::string string;
typedef std::map<std::string, std::string> map_str_str;

void			displayMap(map_str_str toDisplay);
map_str_str		getCGIEnv(const string request, const sockaddr_in *client_addr);
string			getBody(string request);
map_str_str		getRequestHeaders(const string requestStr);
//string			build_response(Request *request);
map_str_str		statusCodes();

#endif