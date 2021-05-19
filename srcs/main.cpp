#include "../webserv.hpp"

#include "../libft/ft_bzero.c"//SALE
#include "../libft/ft_calloc.c"//SALE
#include "../libft/ft_itoa.c"//SALE

// source : https://www.tutorialspoint.com/http/http_responses.htm

void add_status_line(string &response, string code)
{
	map_str_str statusMsg = statusCodes();
	response += code;
	response += " ";
	response += statusMsg[code];
	response += "\n";
}

void add_response_header(string &response, string header, string value)
{
	response += header;
	response += value;
	response += "\n";
}

string get_current_date()
{
	struct timeval tv;
	time_t nowtime;
	struct tm *nowtm;
	char tmbuf[64];

	gettimeofday(&tv, NULL);
	nowtime = tv.tv_sec;
	nowtm = localtime(&nowtime);// in <ctime> so authorized
	strftime(tmbuf, sizeof tmbuf, "%a, %d %b %Y %H:%M:%S GMT", nowtm);
	//tmbuf should look like "Sun, 06 Nov 1994 08:49:37 GMT"

	return string(tmbuf);
}

string get_last_modified(const char *path)
{
	struct stat buf;
	time_t last_modification;
	struct tm *mtime;
	char tmbuf[64];

	stat(path, &buf);//gerer les cas d'erreur ?
	last_modification = buf.st_mtime;
	mtime = localtime(&last_modification);
	strftime(tmbuf, sizeof tmbuf, "%a, %d %b %Y %H:%M:%S GMT", mtime);
	
/* 	Quizas gerer ca (via strptime ?):
   An origin server with a clock MUST NOT send a Last-Modified date that
   is later than the server's time of message origination (Date).  If
   the last modification time is derived from implementation-specific
   metadata that evaluates to some time in the future, according to the
   origin server's clock, then the origin server MUST replace that value
   with the message origination date.  This prevents a future
   modification date from having an adverse impact on cache validation. 

   An origin server without a clock MUST NOT assign Last-Modified values
   to a response unless these values were associated with the resource
   by some other system or user with a reliable clock.   */

	return string(tmbuf);
}

string build_response(Request *request)
{
	std::cout<< "response000" << std::endl;
	std::cout<< "response000" << std::endl;
	std::cout<< "response000" << std::endl;
	std::cout<< "response000" << std::endl;
	string response("HTTP/1.1 ");

	std::cout<< "response00" << std::endl;


	if (request->is_method_valid())
	{
		std::cout<< "response0" << std::endl;
		if (request->get_method() == "GET") //|| request->get_method() == "HEAD")
		{
		std::cout<< "response1" << std::endl;
			if (request->is_CGI())
			{
				return "cgi_request\n";//En attente nouveau sujet ?
			}
			else
			{
		std::cout<< "response2" << std::endl;
				const char *file_path = (*request->get_CGI_env())["REQUEST_URI"].c_str();
				int ret = open(file_path, O_RDONLY);
				//std::cout << ret << std::endl;
		std::cout<< "response3" << std::endl;
				if (ret != -1)
				{
		std::cout<< "response4" << std::endl;
					//lecture du fichier
					char read_buff[INT_MAX];
					ssize_t body_size = read(ret, read_buff, INT_MAX);
					if (body_size == -1)
						return "could not read requested file\n";//a traiter differement, voir les differents cas d'erreur avec errno ? puis set le status code en fonction ?
					if (body_size == INT_MAX)
						return "requested file body is too long\n";//idem ou faire un gnl ?
					
		std::cout<< "response5" << std::endl;
					//status line
					add_status_line(response, OK);//200

		std::cout<< "response6" << std::endl;
					//headers
					if ((*request->get_headers())["Transfer-Encoding"] != string())
						add_response_header(response, "Content-Length: ", string(ft_itoa(body_size)));
					add_response_header(response, "Date: ", get_current_date());
					add_response_header(response, "Last-Modified: ", get_last_modified(file_path));
					add_response_header(response, "Server: ", "Our webserv");//choix statique a confirmer

		std::cout<< "response7" << std::endl;
					//body
					std::string body(read_buff);
					if (body.size() > 0)//useful ?
					{
						response += "\n\n";
						response += body;
					}
					close(ret);
				}
				else
					add_status_line(response, NOT_FOUND);//404
			}
		}
		else if (request->get_method() == "HEAD")
		{}
		else if (request->get_method() == "POST")
		{}
		else if (request->get_method() == "PUT")
		{}
		else if (request->get_method() == "DELETE")
		{}
		else if (request->get_method() == "CONNECT")
		{}
		else if (request->get_method() == "OPTIONS")
		{}
		else if (request->get_method() == "TRACE")
		{}
	}
	else
		add_status_line(response, BAD_REQUEST);//=400 //ou 405 method not allowed?
	return response;
}

	/* response headers:
"Allow" https://datatracker.ietf.org/doc/html/rfc7231#section-7.4.1 obligatoire pour reponse 405
"Content-Language" //osef
"Content-Length" https://datatracker.ietf.org/doc/html/rfc7230#section-3.3.2
"Content-Location"	//osef
"Content-Type"
"Date" https://datatracker.ietf.org/doc/html/rfc7231#section-7.1.1.2 
"Last-Modified" https://datatracker.ietf.org/doc/html/rfc7232#section-2.2
"Location" https://datatracker.ietf.org/doc/html/rfc7231#section-7.1.2 utilise en cas de creation (201) ou de redirection (3xx)
"Retry-After" //osef
"Server" https://datatracker.ietf.org/doc/html/rfc7231#section-7.4.2
"Transfer-Encoding" osef ? https://datatracker.ietf.org/doc/html/rfc7230#section-3.3.1
"WWW-Authenticate" osef ?
 */


int	init_server(sockaddr_in *sock_addr)
{
	int sock_fd;
	int enable = 1;

	if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		std::cerr << "Cannot create socket" << std::endl;
		return (-1);
	}

	if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) ==  -1)
	{
		std::cerr << "Cannot set options to socket" << std::endl;
		return (-1);
	}
	sock_addr = (sockaddr_in*)calloc(1, sizeof(sockaddr_in)); //attention, a changer par ft_calloc
	sock_addr->sin_family = AF_INET;
	sock_addr->sin_addr.s_addr = INADDR_ANY;
	sock_addr->sin_port = htons(PORT);
	if (bind(sock_fd, (struct sockaddr *)sock_addr, sizeof(*sock_addr)) == -1)
	{
		std::cerr << "Cannot bind socket" << std::endl;
		return (-1);
	}
	if (listen(sock_fd, PENDING_MAX) < 0)
	{
		std::cerr << "Cannot listen socket" << std::endl;
		return (-1);
	}
	return (sock_fd);
}

int	start_connexion(int server_fd, sockaddr_in *sock_addr)
{
	int			new_connexion;
	int 		addrlen = sizeof(*sock_addr);
	char		buffer[BUFFER_SIZE];

	while (1)
	{
		std::cout << "----- Waiting for new connection ------" << std::endl << std::endl;
		if ((new_connexion = accept(server_fd, (struct sockaddr *)sock_addr, (socklen_t*)&addrlen)) < 0)
		{
			std::cerr << "Accept failed" << std::endl;
			return (-1);
		}

		//get request
		recv(new_connexion, &buffer, BUFFER_SIZE, 0);
		std::cout << buffer << std::endl;

		//parsing request
		std::string requestStr(buffer);
		Request *request = new Request(requestStr, sock_addr);
		std::cout << *request;

		//parsing du fichier de config ? ou plutot fichier de config est un arg de build_response ci-apres ? A priori ce serait + logique

		//send response

		std::cout<< "main0" << std::endl;
		std::string	response;
		std::cout << "response = "  << std::endl;
		Request *newrequest = new Request(*request);
		build_response(newrequest);
		std::cout<< "main1" << std::endl;
		std::cout<< "main2" << std::endl;

		std::cout << "\n---RESPONSE SENT TO CLIENT:\n" << response << std::endl;
		send(new_connexion, response.c_str(), response.size(), 0);
		close(new_connexion);
		delete request;
	}
	return 0;
}


int main(void)
{
	int			server_fd;
	sockaddr_in	*sock_addr = NULL;
	if ((server_fd = init_server(sock_addr)) == -1)
		return 0;
	if (start_connexion(server_fd, sock_addr) == -1)
		return 0;
	free(sock_addr);
  	close(server_fd);
	return (0);
}
