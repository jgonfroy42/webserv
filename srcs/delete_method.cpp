#include "../includes/webserv.hpp"

size_t response_to_DELETE(Request &request, string &response)
{
	std::cout << "Delete request handler\n";
	if( remove( "myfile.txt" ) != 0 )
    	perror( "Error deleting file" );
	else
		puts( "File successfully deleted" );
	return 0;
	return (42); // a clean later
}