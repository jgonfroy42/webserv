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
# include <poll.h>

//Toujours utile ?
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
# define BAD_REQUEST		"400"
# define NOT_FOUND			"404"
# define NOT_IMPLEMENTED	"501"
# define NO_CONTENT			"204"
# define CREATED			"200"

typedef struct s_param_server
{
	int		socketId;
	int		port;
	struct sockaddr_in6	socketAddr; //utile ? Est-ce que la structure est toujours utile ?
	struct s_param_server	*next;
}	t_param_server;

typedef std::string string;
typedef std::map<std::string, std::string> map_str_str;

class Server;

void			displayMap(map_str_str toDisplay);
int				build_response(Request &request, char **response);
map_str_str		statusCodes();


/*parsing_config.cpp*/
std::vector<Server>	parsing_config(const char *config_file);
void	error_bad_config(string error);
std::vector<size_t>	find_block(string config, size_t start_pos);
string	get_block_type(string config, size_t start_block);
std::vector<int>	get_ports(std::vector<Server> servers);



/*set_communication.cpp*/
int		init_server(t_param_server *param);
void	launch_server(t_param_server *param);
int		get_data(int i, struct sockaddr_in6 addr);

#endif
