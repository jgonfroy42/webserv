#include "../webserv.hpp"
#include "../libft/ft_bzero.c"	//SALE
#include "../libft/ft_calloc.c" //SALE
#include "../libft/ft_itoa.c"	//SALE

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

int response_to_GET_or_HEAD(Request &request, char **response, size_t &response_size)
{
	string headers("HTTP/1.1 ");
	char *to_send;
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
		if (request.get_headers()["Transfer-Encoding"] != string())
			add_header(headers, "Content-Length: ", string(ft_itoa(file_size)));
		add_header(headers, "Date: ", get_current_date());
		add_header(headers, "Last-Modified: ", get_last_modified(request.get_path().c_str()));
		add_header(headers, "Server: ", "Our webserv"); //choix statique a confirmer
		if (request.get_method() == "GET") // Ajout du body
		{
			headers += "\n";
			char *buffer = new char[file_size];
			std::ifstream stream;
			stream.open(request.get_path().c_str(), std::ifstream::binary);
			stream.read(buffer, sizeof(char) * file_size);
			response_size = file_size + headers.size();
			to_send = new char[response_size];

			//body
			for (size_t i = 0; i < headers.size(); ++i)
				to_send[i] = headers.c_str()[i];
			for (size_t i = headers.size(); i < response_size; ++i)
				to_send[i] = buffer[i - headers.size()];
			delete[] buffer;
		}
		else //Method == HEAD
		{
			to_send = new char[headers.size()];
			for (size_t i = 0; i < headers.size(); ++i)
				to_send[i] = headers.c_str()[i];
			response_size = headers.size();
		}
		stream.close();
	}
	else //404 Not Found
	{
		add_status_line(headers, NOT_FOUND);
		to_send = new char[headers.size()];
		for (size_t i = 0; i < headers.size(); ++i)
			to_send[i] = headers.c_str()[i];
		response_size = headers.size();
	}
	*response = to_send;
	return 0;
}

int build_response(Request &request, char **response)
{
	size_t response_size = 0;
	if (request.is_method_valid())
	{
		if (request.get_method() == "GET" || request.get_method() == "HEAD")
		{
			response_to_GET_or_HEAD(request, response, response_size);
		}
		else if (request.get_method() == "POST")
		{
		}
		else if (request.get_method() == "PUT")
		{
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
		add_status_line(headers, BAD_REQUEST); //=400 //ou 405 method not allowed?
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