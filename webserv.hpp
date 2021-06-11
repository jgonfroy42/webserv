#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <iostream>
# include <string>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
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
# include <errno.h>
# include <fstream>
# include <vector>

# define PORT 8080
# define PENDING_MAX 10
# define BUFFER_SIZE 3000

// STATUS CODES
# define OK					"200"
# define BAD_REQUEST		"400"
# define NOT_FOUND			"404"
# define NOT_IMPLEMENTED	"501"
# define NO_CONTENT			"204"
# define CREATED			"200"

typedef std::string string;
typedef std::map<std::string, std::string> map_str_str;

void			displayMap(map_str_str toDisplay);
int				build_response(Request &request, char **response);
map_str_str		statusCodes();

#endif