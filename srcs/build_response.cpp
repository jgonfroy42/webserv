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

int response_to_PUT(Request &request, char **response, size_t response_size)
{
	string headers("HTTP/1.1 ");
	off_t file_size;
	std::fstream stream;
	char *size_itoa;
	if ((file_size = get_file_size(request.get_path().c_str())) >= 0)
	{ //cas d'un fichier deja existant
		char buffer[file_size];
		stream.open(request.get_path().c_str(), std::ifstream::binary);
		stream.read(buffer, file_size);
		size_itoa = ft_itoa(file_size);
		if (!(memcmp(buffer, request.get_body(), file_size)))
		{										  //=requete identique a ce qui existe deja
			add_status_line(headers, NO_CONTENT); //not sure
			add_header(headers, "Last-Modified: ", get_last_modified(request.get_path().c_str()));
		} //si body identique alors on renvoie un last modified, sinon MUST NOT
		else
			add_status_line(headers, OK);
		add_header(headers, "Content-Length: ", string(size_itoa));
		//If a Content-Type header field is not present, the recipient MAY either assume a media type of "application/octet-stream"([RFC2046], Section 4.5.1) or examine the data to determine its type.
		if (request.get_headers()["Content-Type"] == string())
			add_header(headers, "Content-Type: ", "application/octet-stream"); //see nginx behavior
		else
			add_header(headers, "Content-Type: ", request.get_headers()["Content-Type"]); //faire un algo (ou fonction qui m'aurait echappe) qui verifie que le content-type de la requete est identique a celui du fichier? ou osef?
		stream << request.get_body();
	}
	else //fichier cree
	{
		size_t pos = request.get_path().rfind('/');
		if (pos != string::npos) //=fichier dans un repertoire
		{
			//opendir avant? ou osef?
			mkdir(string(request.get_path(), 0, pos + 1).c_str(), S_IRWXU | S_IRWXG | S_IRWXO); //va sans doute buguer si un rep parent na pas tous ces droits
		}																						//parce que la creation d'un fichier ne fonctionne pas si le dossier n'existe pas
		stream.open(request.get_path().c_str(), stream.out | stream.binary | stream.trunc);
		stream.write(request.get_body(), sizeof(request.get_body()));
		add_status_line(headers, CREATED);
		size_itoa = ft_itoa(sizeof(request.get_body()));
		add_header(headers, "Content-Length: ", string(size_itoa));
		if (request.get_headers()["Content-Type"] == string())
			add_header(headers, "Content-Type: ", "application/octet-stream"); //see nginx behavior
		else
			add_header(headers, "Content-Type: ", request.get_headers()["Content-Type"]); //faire un algo (ou fonction qui m'aurait echappe) qui verifie que le content-type de la requete est identique a celui du fichier? ou osef?
	}
	*response = headers_body_join(headers, NULL, headers.size());
	response_size = sizeof(response);
	free(size_itoa);
	stream.close();

	//If the origin server will not make the requested PUT state change to the target resource and instead wishes to have it applied to a different resource, such as when the resource has been moved to a different URI, then the origin server MUST send an appropriate 3xx (Redirection) response
	//409 conflict par exemple pour fichier plus vieux que celui existant
	return (0);
}

int build_response(Request &request, char **response)
{
	size_t response_size = 0;
	if (request.is_method_valid())
	{
		//405 if method know but not allowed for the target
		if (request.get_method() == "GET" || request.get_method() == "HEAD")
		{
			response_to_GET_or_HEAD(request, response, response_size);
		}
		else if (request.get_method() == "POST")
		{
		}
		else if (request.get_method() == "PUT")
		{
			response_to_PUT(request, response, response_size);
		}
		else if (request.get_method() == "DELETE")
		{
		}
		else if (request.get_method() == "CONNECT")
		{
		}
		else if (request.get_method() == "OPTIONS")
		{
		}
		else if (request.get_method() == "TRACE")
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