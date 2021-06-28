#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <iostream>
# include <string>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <unistd.h>
# include <stdlib.h>
# include <sstream>
# include <stdio.h>
# include <map>
# include <arpa/inet.h>
# include <limits.h>
# include <sys/time.h>
# include <time.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <errno.h>
# include <fstream>
# include <vector>
# include <sys/ioctl.h>
# include <cstring>
# include "../srcs/classes/Server.hpp"
# include "../srcs/classes/Location.hpp"
# include "../srcs/classes/Request.hpp"

# define PORT 8080
# define PENDING_MAX 10
# define BUFFER_SIZE 3000

// STATUS CODES
# define OK					"200"
# define CREATED			"201"
# define NO_CONTENT			"204"
# define BAD_REQUEST		"400"
# define NOT_FOUND			"404"
# define NOT_ALLOWED		"405"
# define NOT_IMPLEMENTED	"501"

typedef struct s_param_server
{
	int		socketId;
	fd_set	socket;
	timeval	timeout;
	struct sockaddr_in6	socketAddr;
}	t_param_server;

typedef std::string string;
typedef std::map<std::string, std::string> map_str_str;

class Server;

void			displayMap(map_str_str toDisplay);
size_t			build_response(Request &request, char **response);
map_str_str		statusCodes();


/*parsing_config.cpp*/
std::vector<Server>	parsing_config(const char *config_file);
void	error_bad_config(string error);
std::vector<size_t>	find_block(string config, size_t start_pos);
string	get_block_type(string config, size_t start_block);
std::vector<int>	get_ports(std::vector<Server> servers);

/*autoindex.cpp*/
string	generate_autoindex(string path, string display_path);

/*set_communication.cpp*/
int		init_server(t_param_server *param);
void	launch_server(t_param_server *param);
int		get_data(int i, struct sockaddr_in6 addr);

/*build_response.cpp*/
string get_last_modified(const char *path);
off_t get_file_size(const char *path);

#endif
