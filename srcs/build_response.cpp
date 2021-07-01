#include "../includes/webserv.hpp"

// source : https://www.tutorialspoint.com/http/http_responses.htm

template <typename T>
string NumberToString(T Number)
{
	std::ostringstream ss;
	ss << Number;
	return ss.str();
}

void add_status_line(string &status_line, string code)
{
	map_str_str statusMsg = statusCodes();
	status_line += code;
	status_line += " ";
	status_line += statusMsg[code];
	status_line += "\n";
}

void add_header(string &headers, string header, string value)
{
	headers += header;
	headers += value;
	headers += "\n";
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
	last_modification = stat_struct.st_mtime;
	mtime = localtime(&last_modification);
	strftime(tmbuf, sizeof tmbuf, "%a, %d %b %Y %H:%M:%S GMT", mtime);
	return string(tmbuf);
}

off_t get_file_size(const char *path)
{
	struct stat stat_struct;

	if (stat(path, &stat_struct) != 0) //could not open file
	{
		std::cerr << strerror(errno) << " for path " << path << std::endl;
		return -1;
	}
	else
		return stat_struct.st_size;
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

char *clean_CGI_env_token(string token)
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
		CGI_env[i] = clean_CGI_env_token(CGI_vector[i]); //achtung malloc
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

void add_body_from_path(string &response, string path, off_t file_size)
{
	char *size_itoa;
	size_itoa = (char *)NumberToString(file_size).c_str();
	char *buffer = new char[file_size];
	std::ifstream stream;
	stream.open(path.c_str(), std::ifstream::binary);
	stream.read(buffer, sizeof(char) * file_size);
	add_header(response, "Content-Length: ", string(size_itoa));
	response += "\n";
	response += string(buffer);
	delete[] buffer;
	stream.close();
}

size_t CGI_response(Request &request, string &response, Location &location)
{
	string request_body = request.get_body();
	std::vector<string> CGI_vector = convert_CGI_string_to_vector(request_body);
	char **CGI_env = convert_CGI_vector_to_CGI_env(CGI_vector); //double malloc

	int link[2];
	char cgi_response[8000];
	memset(cgi_response, 0, 8000);
	pipe(link);
	if (fork() == 0)
	{
		dup2(link[1], STDOUT_FILENO);
		close(link[0]);
		close(link[1]);
		char **input = (char **)malloc(sizeof(char *) * 3);

		input[0] = strdup("/usr/bin/php-cgi");
		input[1] = strdup(location.get_cgi_path().c_str());
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
		close(link[1]);
		read(link[0], cgi_response, sizeof(cgi_response));
		string cgi_str(cgi_response);
		add_status_line(response, CREATED); //a verifier
		char separator[5] = {13, 10, 13, 10, 0};
		size_t pos = cgi_str.find(separator) + 4;
		string body = string(cgi_str, pos);
		add_header(response, "Date: ", get_current_date());
		char *size_itoa;
		size_itoa = (char *)NumberToString(body.size()).c_str();
		add_header(response, "Content-Length: ", string(size_itoa));
		size_t begin = cgi_str.find("Content-type: ");
		size_t end = string(cgi_str, begin + 14).find('\n' | ';');
		if (end != string::npos && begin != string::npos)
			add_header(response, "Content-Type: ", string(cgi_str, begin + 14, end));
		response += '\n';
		response += body;
	}
	return 42;
}

bool request_is_cgi(Request &request, Location &location)
{
	string path = request.get_translated_path();
	if (location.is_empty())
		return false;
	string cgi_ext = location.get_cgi_extension();
	if (location.get_cgi_path() != "" && path.size() >= 4 && path.compare(path.size() - cgi_ext.size(), cgi_ext.size(), cgi_ext) == 0)
		return true;
	else
		return false;
}

bool error_page_found_and_valid(Server &server, string code)
{
	if (server.get_id() != -1 && server.get_error_path(code) != "" && (get_file_size(server.get_error_path(code).c_str())) >= 0)
		return true;
	else
		return false;
}

size_t error_response(string &response, string code, Server &server)
{
	off_t file_size = -1;
	add_status_line(response, code);
	string server_name = string();
	string body_path;
	if (server.get_id() != -1 && server.get_server_names() != vec_string())
		server_name = server.get_server_names()[0];
	add_header(response, "Server: ", server_name);
	add_header(response, "Date: ", get_current_date());
	if (server.get_id() != -1 && error_page_found_and_valid(server, code) == true)
		body_path = server.get_error_path(code);
	else
		body_path = "error_pages/" + code + "_error.html";
	file_size = get_file_size(body_path.c_str());
	add_body_from_path(response, body_path, file_size);

	return response.size();
}

bool path_is_a_directory(string path, bool slash_needed)
{
	if (slash_needed && path[path.size() - 1] != '/') //nginx considers a / ends directory paths
		return false;
	DIR *dir;
	bool ret = false;
	if ((dir = opendir(path.c_str())) != NULL)
		ret = true;
	closedir(dir);
	return ret;
}

string path_where_to_upload_file(Request &request, Location &location)
{
	if (location.is_empty())
		return string();
	string new_file_path = request.get_translated_path();
	return new_file_path;
}

bool request_is_to_upload_a_file_and_valid(Request &request, Location &location)
{
	string new_file_path = path_where_to_upload_file(request, location);
	if (location.is_empty() || location.get_upload_path() == "" || path_is_a_directory(location.get_upload_path(), false) || request.get_method() != "POST" || new_file_path == string())
		return false;
	else
		return true;
}

size_t upload_response(Request &request, string &response, Location &location)
{
	off_t file_size;
	std::fstream stream;
	char *size_itoa;

	string new_file_path = path_where_to_upload_file(request, location);
	if ((file_size = get_file_size(new_file_path.c_str())) >= 0 && path_is_a_directory(new_file_path, false) == false)
		remove(new_file_path.c_str());
	size_t pos = new_file_path.rfind('/');
	if (pos != string::npos) //=file in a directory
	{
		//opendir avant? ou osef?
		mkdir(string(new_file_path, 0, pos + 1).c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
	}
	stream.open(new_file_path.c_str(), stream.out | stream.binary | stream.trunc);
	stream.write(request.get_body().c_str(), request.get_body().size());
	add_status_line(response, CREATED);
	size_itoa = (char *)NumberToString(file_size).c_str();
	add_header(response, "Content-Length: ", string(size_itoa));
	if (request.get_headers()["Content-Type"] == string())
		add_header(response, "Content-Type: ", "application/octet-stream"); //see nginx behavior
	else
		add_header(response, "Content-Type: ", request.get_headers()["Content-Type"]);
	add_header(response, "Location: ", new_file_path);
	stream.close();
	return 42;
}

size_t chunked_response(Request &request, string &response)
{
		add_status_line(response, CREATED); //a verifier
		string body = request.get_body();
		char *size_itoa;
		size_itoa = (char *)NumberToString(body.size()).c_str();
		add_header(response, "Date: ", get_current_date());
		add_header(response, "Content-Length: ", string(size_itoa));
		response += '\n';
		response += body;
		return 42;
}

size_t response_to_POST(Request &request, string &response, Server &server, Location &location)
{
	if (request.is_chunked())
		return chunked_response(request, response);
	else if (request_is_cgi(request, location))
		return CGI_response(request, response, location);
	else if (request_is_to_upload_a_file_and_valid(request, location))
		return upload_response(request, response, location);
	else
		return error_response(response, NOT_FOUND, server);
}

bool autoindex_is_on_and_valid(Request &request, Location &location)
{
	if (path_is_a_directory(request.get_translated_path(), true) && location.is_empty() == false && location.auto_index_is_on())
		return true;
	else
		return false;
}

size_t directory_listing_response(Request &request, string &response, Server &server)
{
	add_status_line(response, OK); //200

	//Adding headers
	add_header(response, "Date: ", get_current_date());
	string server_name = string();
	if (server.get_id() != -1 && server.get_server_names() != vec_string())
		server_name = server.get_server_names()[0];
	add_header(response, "Server: ", server_name);

	string body = generate_autoindex(request.get_translated_path(), request.get_path());
	char *size_itoa;
	size_itoa = (char *)NumberToString(body.size()).c_str();
	add_header(response, "Content-Length: ", string(size_itoa));

	response += '\n';
	response += body;
	return 42;
}

size_t redirected_response(string &response, pair_str_str redirect, Server &server)
{
	add_status_line(response, redirect.first);
	string server_name = string();
	if (server.get_server_names() != vec_string())
		server_name = server.get_server_names()[0];
	add_header(response, "Server: ", server_name);
	add_header(response, "Date: ", get_current_date());
	add_header(response, "Location: ", redirect.second);
	return response.size();
}

void append_index_to_path(Request &request, Server &server, Location &location)
{
	vec_string indexes = vec_string();
	string new_path = request.get_translated_path();
	if (location.is_empty() == false && location.get_index() != vec_string())
		indexes = location.get_index();
	else if (server.get_index() != vec_string())
		indexes = server.get_index();
	if (indexes == vec_string())
		new_path += "index.html";
	else
	{
		for (size_t i = 0; i < indexes.size(); i++)
		{
			new_path = request.get_translated_path() + indexes[i];
			if (get_file_size(new_path.c_str()) >= 0)
				break;
		}
	}
	request.set_translated_path(new_path);
}

size_t response_to_GET_or_HEAD(Request &request, string &response, Server &server, Location &location)
{
	off_t file_size = 0;

	//	string index;
	if (path_is_a_directory(request.get_translated_path(), true) && autoindex_is_on_and_valid(request, location))
		return directory_listing_response(request, response, server);
	else if (path_is_a_directory(request.get_translated_path(), true))
		append_index_to_path(request, server, location);
	if (request_is_cgi(request, location) && request.get_query_string() != "")
	{
		Request newRequest = Request(request, request.get_query_string());
		return CGI_response(newRequest, response, location);
	}
	if ((file_size = get_file_size(request.get_translated_path().c_str())) >= 0 && path_is_a_directory(request.get_translated_path(), false) == false)
	{
		//opening file
		std::ifstream stream;
		stream.open(request.get_translated_path().c_str(), std::ifstream::binary);

		//status line
		add_status_line(response, OK); //200

		//Adding headers
		add_header(response, "Date: ", get_current_date());
		add_header(response, "Last-Modified: ", get_last_modified(request.get_translated_path().c_str()));
		string server_name = string();
		if (server.get_id() != -1 && server.get_server_names() != vec_string())
			server_name = server.get_server_names()[0];
		add_header(response, "Server: ", server_name);

		//Adding body
		if (request.get_method() == "GET")
			add_body_from_path(response, request.get_translated_path(), file_size);
	}
	else
		return error_response(response, NOT_FOUND, server);
	return 42;
}

Server choose_server(Request &request, std::vector<Server> &servers)
{
	bool port_found = false;
	Server chosen_server = Server(servers[0]);
	for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); ++it)
	{
		if (it->get_port_str() == request.get_port())
		{
			if (port_found == false)
			{
				port_found = true;
				chosen_server = Server(*it);
			}
			if (it->get_host() == request.get_host())
			{
				chosen_server = Server(*it);
				return (chosen_server);
			}
		}
	}
	return chosen_server;
}

Location choose_location(string path, vec_location locations)
{
	string to_match = string("/") + path;
	while (to_match != string())
	{
		for (vec_location::iterator it = locations.begin(); it != locations.end(); it++)
		{
			if (to_match.find(it->get_path()) == 0) //=first matching
				return (*it);
		}
		to_match = string(to_match, 0, to_match.find_last_of('/'));
	}
	return Location();
}

bool redirection_found(Location &location)
{
	if (location.is_empty() || location.get_redirect() == pair_str_str(string(), string()))
		return false;
	else
		return true;
}

void translate_path(Request &request, Server &server, Location &location)
{
	string root;
	if (location.is_empty() == true || location.root_is_set() == false)
		root = server.get_root();
	else if (request.get_method() == "POST" && location.get_upload_path() != "")
		root = location.get_upload_path();
	else
		root = location.get_root();
	request.append_root_to_path(root);
}

size_t response_to_DELETE(Request &request, string &response, Server &server)
{
	if (remove(request.get_path().c_str()) != 0)
		error_response(response, NOT_FOUND, server);
	else
	{
		add_status_line(response, NO_CONTENT);
		add_header(response, "Date: ", get_current_date());
	}
	return (42); // a clean later
}

size_t build_response(Request &request, string &response, std::vector<Server> &servers)
{
	Server server = Server();
	if (request.is_bad_request())
		return error_response(response, BAD_REQUEST, server); //400
	server = choose_server(request, servers);
	Location location = choose_location(request.get_path(), server.get_locations());
	if (location.method_is_allowed(request.get_method()) == false)
		return error_response(response, NOT_ALLOWED, server);
	translate_path(request, server, location);
	if (redirection_found(location))
		return redirected_response(response, location.get_redirect(), server);
	else if (request.get_method() == "GET" || request.get_method() == "HEAD")
		return response_to_GET_or_HEAD(request, response, server, location);
	else if (request.get_method() == "POST")
		return response_to_POST(request, response, server, location);
	else if (request.get_method() == "DELETE") //a implementer
		return response_to_DELETE(request, response, server);
	else
		return error_response(response, NOT_ALLOWED, server); //405
	return 42;
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
