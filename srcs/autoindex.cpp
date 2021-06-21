#include "../includes/webserv.hpp"

string	generate_tr(const char *path)
{
	std::ostringstream ss;
	ss << get_file_size(path);
	string size = ss.str();
	string tr = "            <tr>\n\
                <th><a href=\"../\">" + string(path) + "</a></th>\n\
                <th>" + get_last_modified(path) + "</th>\n\
                <th>" + size + "</th>\n\
            </tr>\n";
	return (tr);
}

void	generate_autoindex(string path, string root)
{
	(void)root;
	string start = "<!doctype html>\n\
<html>\n\
    <head>\n\
        <style>\n\
            table, hr { width:800px; margin-left: 0; text-align: left; }\n\
        </style>\n\
        <title>Index of " + path +"</title>\n\
    </head>\n\
    <body>\n\
        <h1>Index of /</h1>\n\
        <hr/>\n\
        <table>";
	std::cout << start << std::endl;
	std::cout << generate_tr("Makefile");
	string end = "        </table>\n\
        <hr/>\n\
    </body>\n\
</html>";
	std::cout << end << std::endl;

}