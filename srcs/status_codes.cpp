#include "../includes/webserv.hpp"

map_str_str		statusCodes()
{
	map_str_str	statuscode;

	statuscode["100"] = "Continue";
	statuscode["101"] = "Switching Protocols";
	statuscode["102"] = "Processing";
	statuscode["103"] = "Early Hints";
	statuscode["200"] = "OK";
	statuscode["201"] = "Created";
	statuscode["202"] = "Accepted";
	statuscode["203"] = "Non-Authoritative Information";
	statuscode["204"] = "No Content";
	statuscode["205"] = "Reset Content";
	statuscode["206"] = "Partial Content";
	statuscode["207"] = "Multi-Status";
	statuscode["208"] = "Already Reported";
	statuscode["210"] = "Content Different";
	statuscode["226"] = "IM Used";
	statuscode["300"] = "Multiple Choices";
	statuscode["301"] = "Moved Permanently";
	statuscode["302"] = "Found";
	statuscode["303"] = "See Other";
	statuscode["304"] = "Not Modified";
	statuscode["305"] = "Use Proxy";
	statuscode["306"] = "Switch Proxy";
	statuscode["307"] = "Temporary Redirect";
	statuscode["308"] = "Permanent Redirect";
	statuscode["310"] = "Too many Redirects";
	statuscode["400"] = "Bad Request";
	statuscode["401"] = "Unauthorized";
	statuscode["402"] = "Payment Required";
	statuscode["403"] = "Forbidden";
	statuscode["404"] = "Not Found";
	statuscode["405"] = "Method Not Allowed";
	statuscode["406"] = "Not Acceptable";
	statuscode["407"] = "Proxy Authentication Required";
	statuscode["408"] = "Request Time-out";
	statuscode["409"] = "Conflict";
	statuscode["410"] = "Gone";
	statuscode["411"] = "Length Required";
	statuscode["412"] = "Precondition Failed";
	statuscode["413"] = "Request Entity Too Large";
	statuscode["414"] = "Request-URI Too Long";
	statuscode["415"] = "Unsupported Media Type";
	statuscode["416"] = "Requested range unsatisfiable";
	statuscode["417"] = "Expectation failed";
	statuscode["418"] = "I’m a teapot";
	statuscode["421"] = "Bad mapping / Misdirected Request";
	statuscode["422"] = "Unprocessable entity";
	statuscode["423"] = "Locked";
	statuscode["424"] = "Method failure";
	statuscode["425"] = "Too Early";
	statuscode["426"] = "Upgrade Required";
	statuscode["428"] = "Precondition Required";
	statuscode["429"] = "Too Many Requests";
	statuscode["431"] = "Request Header Fields Too Large";
	statuscode["449"] = "Retry With";
	statuscode["450"] = "Blocked by Windows Parental Controls";
	statuscode["451"] = "Unavailable For Legal Reasons";
	statuscode["456"] = "Unrecoverable Error";
	statuscode["444"] = "No Response";
	statuscode["495"] = "SSL Certificate Error";
	statuscode["496"] = "SSL Certificate Required";
	statuscode["497"] = "HTTP Request Sent to HTTPS Port";
	statuscode["498"] = "Token expired/invalid";
	statuscode["499"] = "Client Closed Request";
	statuscode["500"] = "Internal Server Error";
	statuscode["501"] = "Not Implemented";
	statuscode["502"] = "Bad Gateway ou Proxy Error";
	statuscode["503"] = "Service Unavailable";
	statuscode["504"] = "Gateway Time-out";
	statuscode["505"] = "HTTP Version not supported";
	statuscode["506"] = "Variant Also Negotiates";
	statuscode["507"] = "Insufficient storage";
	statuscode["508"] = "Loop detected";
	statuscode["509"] = "Bandwidth Limit Exceeded";
	statuscode["510"] = "Not extended";
	statuscode["511"] = "Network authentication required";
	statuscode["520"] = "Unknown Error";
	statuscode["521"] = "Web Server Is Down";
	statuscode["522"] = "Connection Timed Out";
	statuscode["523"] = "Origin Is Unreachable";
	statuscode["524"] = "A Timeout Occurred";
	statuscode["525"] = "SSL Handshake Failed";
	statuscode["526"] = "Invalid SSL Certificate";
	statuscode["527"] = "Railgun Error";

	return statuscode;
}