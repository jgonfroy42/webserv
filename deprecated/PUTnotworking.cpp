/* #include "../webserv.hpp"
#include "../libft/ft_bzero.c"	 //SALE
#include "../libft/ft_calloc.c"	 //SALE
#include "../libft/ft_itoa.c"	 //SALE
#include "../libft/ft_strjoin.c" //SALE
#include "../libft/ft_strlcpy.c" //SALE
#include "../libft/ft_strlcat.c" //SALE
#include "../libft/ft_strlen.c"	 //SALE

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
} */