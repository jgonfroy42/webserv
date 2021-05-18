#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <map>

typedef std::string string;
typedef std::map<std::string, std::string> map;

Accept-Charsets
Accept-Language
Allow
Authorization
Content-Language
Content-Length
Content-Location
Content-Type
Date
Host
Last-Modified
Location
Referer
Retry-After
Server
Transfer-Encoding
User-Agent
WWW-Authenticate



map		initRequestMap()
{

}

map		parseRequest(string requestStr)
{
	map	requestMap;


	return requestMap;
}
