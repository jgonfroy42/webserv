#include "../webserv.hpp"

typedef std::map<std::string, std::string> map_str_str;
typedef std::string string;

int		getHeaderValue(map_str_str &headers, string request, string header)
{
	size_t pos = 0;
	int len = 0;
	if ((pos = request.find(header + ": ")) != string::npos)
	{
		pos += header.size() + 2;
		while(request[pos + len] && request[pos + len] != '\n')
			len++;
		headers[header] = string(request, pos, len);
	}
	return 0;//choix éditorial a confirmer
}

void	initHeaders(map_str_str &headers)
{
	headers["Accept-Charset"] = string();
	headers["Accept-Language"] = string();
	headers["Authorization"] = string();
	headers["Content-Length"] = string();
	headers["Content-Type"] = string();
	headers["Date"] = string();
	headers["Host"] = string();
	headers["Referer"] = string();
	headers["Transfer-Encoding"] = string();//also called TE
	headers["User-Agent"] = string();
}

map_str_str		getRequestHeaders(const string requestStr)
{
	map_str_str	headers;

	initHeaders(headers);
	getHeaderValue(headers, requestStr, "Accept-Charset");
	getHeaderValue(headers, requestStr, "Accept-Language");
	getHeaderValue(headers, requestStr, "Authorization");
	getHeaderValue(headers, requestStr, "Content-Length");
	getHeaderValue(headers, requestStr, "Content-Type");
	getHeaderValue(headers, requestStr, "Date");
	getHeaderValue(headers, requestStr, "Host");
	getHeaderValue(headers, requestStr, "Referer");
	getHeaderValue(headers, requestStr, "Transfer-Encoding");
	getHeaderValue(headers, requestStr, "User-Agent");

	return headers;
}

string	getBody(string request)
{
	size_t pos;
	if ((pos = request.find("\n\n")) != string::npos)
	{
		return string(request, pos + 2, string::npos);//substring de request apres \n\n
	}
	else	
		return string();
}

map_str_str		initMetaMap()
{
	map_str_str metaMap;
	metaMap["AUTH_TYPE"] = string();
	metaMap["CONTENT_LENGTH"] = string();
	metaMap["CONTENT_TYPE"] = string();
	metaMap["GATEWAY_INTERFACE"] = "CGI/1.1";//choix statique à confirmer
	metaMap["PATH_INFO"] = string();
	metaMap["PATH_TRANSLATED"] = string();
	metaMap["QUERY_STRING"] = string();
	metaMap["REMOTE_ADDR"] = string();
	metaMap["REMOTE_IDENT"] = string();
	metaMap["REMOTE_USER"] = string();
	metaMap["REQUEST_METHOD"] = string();
	metaMap["REQUEST_URI"] = string();
	metaMap["SCRIPT_NAME"] = "cgi-bin/myscript.cgi";//choix statique à confirmer
	metaMap["SERVER_NAME"] = string();
	metaMap["SERVER_PORT"] = string();
	metaMap["SERVER_PROTOCOL"] = string();
	metaMap["SERVER_SOFTWARE"] = "Our webserv";//choix statique à confirmer
	return metaMap;
}

int		parseStartLine(map_str_str &metaMap, string startLine)
{
	string::iterator it_begin = startLine.begin();
	string::iterator it_end = it_begin + startLine.find(" ");
	metaMap["REQUEST_METHOD"] = string(it_begin, it_end++);
	std::cout << metaMap["REQUEST_METHOD"];
	it_begin = it_end;
	while (*it_end && *it_end != ' ')
		it_end++;
	metaMap["REQUEST_URI"] = string(it_begin, it_end++);
	std::cout << metaMap["REQUEST_URI"];

	it_begin = it_end;
	while (*it_end && *it_end != '\n')
		it_end++;
	metaMap["SERVER_PROTOCOL"] = string(it_begin, it_end);
	std::cout << metaMap["SERVER_PROTOCOL"];
	size_t pos;
	int len = 0;
	if ((pos = startLine.find(metaMap["SCRIPT_NAME"])) != string::npos)
	{
		pos += metaMap["SCRIPT_NAME"].size() + 1;
		while(startLine[pos + len] && startLine[pos + len] != ' ' && startLine[pos + len] != '?')
		len++;
		metaMap["PATH_INFO"] = string(startLine, pos, len);
	}
	if ((pos = startLine.find('?')) != string::npos)
	{
		pos += 1;
		len = 0;
		while(startLine[pos + len] && startLine[pos + len] != ' ')
			len++;
		metaMap["QUERY_STRING"] = string(startLine, pos, len);
	}		
	return 0;
}

int		getMetaValue(map_str_str &metaMap, string request, string metaVar, string toFind)
{
	size_t pos = 0;
	int len = 0;
	if ((pos = request.find(toFind)) != string::npos)
	{
		pos += toFind.size();
		while(request[pos + len] && request[pos + len] != '\n')
			len++;
		metaMap[metaVar] = string(request, pos, len);
	}
	return 0;//choix éditorial a confirmer
}

map_str_str		getCGIEnv(const string request, const sockaddr_in *client_addr)
{
	map_str_str metaMap = initMetaMap();//choix d'initialisation à confirmer
	parseStartLine(metaMap, string(request, 0, request.find('\n')));
	getMetaValue(metaMap, request, "SERVER_NAME", "Host: ");
	metaMap["SERVER_PORT"] = metaMap["SERVER_NAME"];
	metaMap["SERVER_NAME"].erase(metaMap["SERVER_NAME"].find(':'));
	metaMap["SERVER_PORT"].erase(0, metaMap["SERVER_PORT"].find(':') + 1);
	if (request.find("Authorization") != string::npos)
	{
		getMetaValue(metaMap, request, "AUTH_TYPE", "WWW-Authenticate: ");//not possible WWW cest un header de reponse pas de requete
		if (metaMap["AUTH_TYPE"] == "Basic")	
			getMetaValue(metaMap, request, "REMOTE_USER", "//idk yet");//a creuser, a prendre dans QUERY_STRING maybe ?
	}
	getMetaValue(metaMap, request, "CONTENT_LENGTH", "Content-Length: ");
	getMetaValue(metaMap, request, "CONTENT_TYPE", "Content-Type: ");
//	char *client_IP = inet_ntoa(client_addr->sin_addr);//fonction non autorisee... peut-etre peut-on envoyer sin_addr ou client_addr directement au CGI (mais pas string donc il faut struct plutot que map) + verifier si on envoie le pointeur ou son deferencement
//	metaMap["REMOTE_ADDR"] = string(client_IP);
	(void)client_addr;
	return metaMap;
}

/* sources (variables CGI):
	metavariables:
	http://www.wijata.com/cgi/cgispec.html#4.0
	https://fr.wikipedia.org/wiki/Variables_d%27environnement_CGI
	https://www.oreilly.com/openbook/cgi/ch02_02.html#CGI-CHP-2-TBL-1
	request headers:
	https://en.wikipedia.org/wiki/List_of_HTTP_header_fields 
	https://developer.mozilla.org/en-US/docs/Web/HTTP/Messages

LEGEND:
//: var done
/ : halfway done - to be checked
  : not done

*variables spécifiques au serveur
	// SERVER_NAME
	/ SERVER_SOFTWARE: Le nom et la version du serveur HTTP répondant à la requête. (Format : nom/version)
	// GATEWAY_INTERFACE: La révision de la spécification CGI que le serveur utilise. (Format : CGI/révision)

*variables spécifiques à la requête
	/ SERVER_PROTOCOL
	// SERVER_PORT
	// PATH_INFO:Le chemin supplémentaire du script tel que donné par le client. Par exemple, si le serveur héberge le script « /cgi-bin/monscript.cgi » [=SCRIPT_NAME]et que le client demande l'url « http://serveur.org/cgi-bin/monscript.cgi/marecherche », alors PATH_INFO contiendra « marecherche ».[il faut donc déterminer avant le ou les chemins du ou des scripts]
	// AUTH_TYPE
	// CONTENT_LENGTH
	// CONTENT_TYPE
	   PATH_TRANSLATED: Contient le chemin demandé par le client après que les conversions virtuel → physique ont été faites par le serveur. [=chemin du contenu dans le serveur]
	// QUERY_STRING:Contient tout ce qui suit le « ? » dans l'URL envoyée par le client. Toutes les variables provenant d'un formulaire envoyé avec la méthode « GET » seront contenues dans le QUERY_STRING sous la forme « var1=val1&var2=val2&... ».
	   REMOTE_ADDR: adresse IP du client
	   REMOTE_IDENT: Nom d'utilisateur (distant) du client faisant la requête. Le serveur doit supporter l'identification RFC 931. Cette variable devrait être utilisée à des fins de journaux seulement.
	/  REMOTE_USER: Le nom d'utilisateur du client seulement  si le script est protégé si on a "AUTH_TYPE" set to Basic
	// REQUEST_METHOD
	// REQUEST_URI
	// SCRIPT_NAME: chemin virtuel vers le script executé cf également PATH_INFO
 */
