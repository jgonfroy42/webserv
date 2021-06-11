#include "../webserv.hpp"
#include "../libft/ft_bzero.c"	 //SALE
#include "../libft/ft_calloc.c"	 //SALE
#include "../libft/ft_itoa.c"	 //SALE
#include "../libft/ft_strjoin.c" //SALE
#include "../libft/ft_strlcpy.c" //SALE
#include "../libft/ft_strlcat.c" //SALE
#include "../libft/ft_strlen.c"	 //SALE

// source : https://www.tutorialspoint.com/http/http_responses.htm

void add_status_line(string &response, string code)
{
	map_str_str statusMsg = statusCodes();
	response += code;
	response += " ";
	response += statusMsg[code];
	response += "\n";
}

void add_header(string &response, string header, string value)
{
	response += header;
	response += value;
	response += "\n";
}

string get_current_date()
{
	//from https://stackoverflow.com/questions/2408976/struct-timeval-to-printable-format
	struct timeval tv;
	time_t nowtime;
	struct tm *nowtm;
	char tmbuf[64];

	gettimeofday(&tv, NULL);
	nowtime = tv.tv_sec;
	nowtm = localtime(&nowtime); // in <ctime> so authorized
	strftime(tmbuf, sizeof tmbuf, "%a, %d %b %Y %H:%M:%S GMT", nowtm);
	//tmbuf should look like "Sun, 06 Nov 1994 08:49:37 GMT"

	return string(tmbuf);
}

string get_last_modified(const char *path)
{
	struct stat stat_struct;
	time_t last_modification;
	struct tm *mtime;
	char tmbuf[64];

	if (stat(path, &stat_struct) != 0)
		std::cout << strerror(errno) << " for path " << path << std::endl;
	; //gerer les cas d'erreur ?
	last_modification = stat_struct.st_mtime;
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

off_t get_file_size(const char *path)
{
	struct stat stat_struct;

	if (stat(path, &stat_struct) != 0)
	{
		std::cerr << strerror(errno) << " for path " << path << std::endl; //pertinence ?
		return -1;
	}
	else
		return stat_struct.st_size;
}

char *headers_body_join(string headers, char *body, size_t size)
{
	char *dst;

	dst = new char[size];
	for (size_t i = 0; i < headers.size(); ++i)
		dst[i] = headers.c_str()[i];
	for (size_t i = headers.size(); i < size; ++i)
		dst[i] = body[i - headers.size()];
	return dst;
}

int response_to_GET_or_HEAD(Request &request, char **response, size_t &response_size)
{
	string headers("HTTP/1.1 ");
	off_t file_size;
	if (request.is_CGI()) //En attente nouveau sujet ?
	{
		std::cerr << "cgi_request\n";
		return -1;
	}
	else if ((file_size = get_file_size(request.get_path().c_str())) >= 0)
	{
		//opening file
		std::ifstream stream;
		stream.open(request.get_path().c_str(), std::ifstream::binary);

		//status line
		add_status_line(headers, OK); //200

		//headers
		char *size_itoa = ft_itoa(file_size); //malloc --'
		if (request.get_headers()["Transfer-Encoding"] != string())
			add_header(headers, "Content-Length: ", string(size_itoa));
		add_header(headers, "Date: ", get_current_date());
		add_header(headers, "Last-Modified: ", get_last_modified(request.get_path().c_str()));
		add_header(headers, "Server: ", "Our webserv"); //choix statique a confirmer
		if (request.get_method() == "GET")				// Ajout du body
		{
			headers += "\n";
			char *buffer = new char[file_size];
			std::ifstream stream;
			stream.open(request.get_path().c_str(), std::ifstream::binary);
			stream.read(buffer, sizeof(char) * file_size);
			response_size = file_size + headers.size();
			*response = headers_body_join(headers, buffer, response_size);
			delete[] buffer;
		}
		else //Method == HEAD
		{
			response_size = headers.size();
			*response = headers_body_join(headers, NULL, response_size);
		}
		stream.close();
		free(size_itoa);
	}
	else //stat renvoie -1 == 404 Not Found ou voir en fonction de errno ?
	{
		add_status_line(headers, NOT_FOUND);
		response_size = headers.size();
		*response = headers_body_join(headers, NULL, response_size);
	}
	return 0;
}

std::vector<string> convert_CGI_string_to_vector(string CGI_string)
{
	std::vector<string> pairs;

	char *to_split = (char *)CGI_string.c_str();
	char *ret;
	ret = strtok(to_split, "&");
	while (ret != NULL)
	{
		pairs.push_back(string(ret));
		ret = strtok(NULL, "&");
	}
	return pairs;
}

char *clean_CGI_env_token(string token) //a voir si on fait plus propre ou osef= a voir si on fouille un peu dans la syntaxe des bodys envoyes par POST ou pas
{
	char *shiny_token;

	if ((shiny_token = (char *)malloc(sizeof(char) * (token.size() + 1))) == NULL)
	{
		std::cerr << "Failed allocating memory for a brand new shiny CGI token\n";
		return NULL;
	}
	shiny_token[token.size()] = '\0';
	strcpy(shiny_token, token.c_str());
	for (size_t i = 0; i < strlen(shiny_token); i++)
	{
		if (shiny_token[i] == '+')
			shiny_token[i] = ' ';
	}
	return shiny_token;
}

char **convert_CGI_vector_to_CGI_env(std::vector<string> CGI_vector)
{
	char **CGI_env = NULL;

	if ((CGI_env = (char **)malloc(sizeof(char *) * (CGI_vector.size() + 1))) == NULL)
	{
		std::cerr << "Failed allocating memory for CGI env\n";
		return NULL;
	}
	CGI_env[CGI_vector.size()] = NULL;
	for (size_t i = 0; i < CGI_vector.size(); i++)
	{
		CGI_env[i] = clean_CGI_env_token(CGI_vector[i]); //achtung malloc
		std::cout << "shinytoken=" << CGI_env[i]<<"%\n";
	}
	return CGI_env;
}

void free_CGI_env(char **CGI_env)
{
	size_t i = 0;
	while (CGI_env && CGI_env[i])
	{
		free(CGI_env[i]);
		CGI_env[i] = NULL;
		i++;
	}
	if (CGI_env)
		free(CGI_env);
	CGI_env = NULL;
}

string	process_post_CGI()
{
	return string();
}

int response_to_POST(Request &request, char **response, size_t &response_size)
{
	string request_body = request.get_body();
	std::vector<string> CGI_vector = convert_CGI_string_to_vector(request_body);
	char **CGI_env = convert_CGI_vector_to_CGI_env(CGI_vector); //double malloc

	//rajouter if on est bien dans un cgi

//	usleep(500); //sinon ca imprime mal
	int link[2];
	char cgi_response[8000]; // TAILLE RANDOM ICI, FAUDRA METTRE UN VRAI TRUC /probablement client body size
	pipe(link);
	if (fork() == 0)
	{
//		usleep(500); //sinon ca imprime mal
		dup2(link[1], STDOUT_FILENO);
		close(link[0]);
		close(link[1]);
		char **input = (char **)malloc(sizeof(char *) * 3);

		input[0] = strdup("/usr/bin/php-cgi");
		input[1] = strdup("./post.php");
		input[2] = NULL;
		execve("/usr/bin/php-cgi", input, CGI_env);
		free(input[0]);
		free(input[1]);
		free(input);
		free_CGI_env(CGI_env);
		exit(EXIT_FAILURE);
	}
	else
	{
	//	usleep(500); //sinon ca imprime mal
		close(link[1]);
		size_t ret = read(link[0], cgi_response, sizeof(cgi_response));
		(void)ret;
	//	std::fstream stream;
	//	stream.open("./tmpfd", stream.out| stream.binary | stream.trunc);
	//	stream.write(cgi_response, ret);
		string cgi_str(cgi_response);
		string headers("HTTP/1.1 ");
		add_status_line(headers, CREATED);//a verifier
		char separator[5] = {13, 10, 13, 10, 0};
		size_t pos = cgi_str.find(separator) + 4;
		string body = string(cgi_str, pos);
		char *size_itoa = ft_itoa(body.size());
		add_header(headers, "Content-Length: ", string(size_itoa));
		free(size_itoa);

		size_t begin = cgi_str.find("Content-type: ") + 14;
		size_t end = string(cgi_str, begin).find('\n' | ';');
		if (end != string::npos && begin != string::npos)
		{
			add_header(headers, "Content-Type: ", string(cgi_str, begin, end));
		}
		headers += '\n';
		response_size = headers.size() + body.size();
		*response = headers_body_join(headers, (char*)body.c_str(), response_size);
	}

	return 0;
}

int build_response(Request &request, char **response)
{
	size_t response_size = 0;
	if (request.is_method_valid())
	{
		//405 if method known but not allowed for the target
		if (request.get_method() == "GET" || request.get_method() == "HEAD")
		{
			response_to_GET_or_HEAD(request, response, response_size);
		}
		else if (request.get_method() == "POST")
		{
			response_to_POST(request, response, response_size);
		}
		else if (request.get_method() == "DELETE")
		{
		}
	}
	else
	{
		string headers("HTTP/1.1 ");
		add_status_line(headers, NOT_IMPLEMENTED); //=501 //cf 4.1
	}
	return response_size;
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