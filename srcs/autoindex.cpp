#include "../includes/webserv.hpp"
#include <dirent.h>
#include <algorithm>

string	generate_tr(const char *path, string original_path, unsigned char d_type)
{
	string size;
	string full_path = original_path + "/" + path;
	string date = get_last_modified(full_path.c_str());
	if (d_type != DT_DIR)
	{
		std::ostringstream ss;
		ss << get_file_size(full_path.c_str());
		size = ss.str();
	}
	else
		size = "-";
	string tr = "            <tr>\n                <th><a href=\""+ full_path +"\">" + string(path) + "</a></th>\n                <th>" + date + "</th>\n                <th>" + size + "</th>\n            </tr>\n";
	return (tr);
}

bool alpha_sort(struct dirent* a, struct dirent* b) 
{
	int test = string(a->d_name).compare(string(b->d_name));
	return (test > 0);
}

bool type_sort(struct dirent* a, struct dirent* b)
{
	if (a->d_type == b->d_type)
		return (false);
	else if (a->d_type == DT_DIR)
		return (false);
	else
		return (true);
}

string	generate_autoindex(string path, string display_path)
{	
	string start = "<!doctype html>\n<html>\n    <head>\n        <style>\n            table, hr { width:800px; margin-left: 0; text-align: left; }\n        </style>\n        <title>Index of " + path +"</title>\n    </head>\n    <body>\n        <h1>Index of " + display_path + "</h1>\n        <hr/>\n        <table>            <tr>\n                <th><a href=\"..\">..</a></th>\n                <th></th>\n                <th></th>\n            </tr>\n";
	
	string end = "        </table>\n        <hr/>\n    </body>\n</html>";
	
	string middle;

	std::vector<struct dirent*> files;

	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir ((path).c_str())) != NULL) {
		while ((ent = readdir (dir)) != NULL) {
			files.push_back(ent);
		}
		
		std::cout << std::endl << std::endl;
		std::sort(files.begin(), files.end(), alpha_sort);
		
		files.pop_back();
		files.pop_back();

		std::cout << std::endl << std::endl;
		std::sort(files.begin(), files.end(), type_sort);

		for (std::vector<struct dirent*>::reverse_iterator it = files.rbegin(); it != files.rend(); ++it)
			middle += generate_tr((*it)->d_name, path, (*it)->d_type);
		
		closedir (dir);

		return (start + middle + end);
	
	} else
		return ("");
}