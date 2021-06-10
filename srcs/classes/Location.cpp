#include "Location.hpp"
#include "../../webserv.hpp"


string	Location::get_configuration(const string server_config, const string label, bool optional)
{
	size_t label_pos = server_config.find(label);

	if (label_pos == string::npos && optional == false)
		error_bad_config();
	else if (label_pos == string::npos && optional == true)
		return ("");

	size_t end_line_pos = server_config.find(";", label_pos);
	if (end_line_pos == string::npos)
		error_bad_config();

	string line = server_config.substr(label_pos, end_line_pos - label_pos);
	
	// Check si doublon
	if (server_config.find(label, end_line_pos + 1) != string::npos)
		error_bad_config();
	return (line);
}

// ********** LOCATION CLASS ************ //

Location::Location(const string location_config, int id)
{
	size_t split_pos;
	// std::cout << "In location = " << location_config << std::endl;

	// Checking if not another block inside location block
	size_t start_block = location_config.find('{', 0);
	if (location_config.find('{', start_block + 1) != string::npos)
		error_bad_config();
	
	this->_id = id;

	// Setting path
	this->_path = location_config.substr(0, location_config.find(" "));
	
	// Setting root
	string root_line = get_configuration(location_config, "root", true);
	if (root_line != "")
	{
		split_pos = root_line.find(' ');
		if (split_pos == string::npos)
			error_bad_config();
		this->_root = root_line.substr(split_pos + 1);
	}
	
	// Setting index
	string index_line = get_configuration(location_config, " index ", true);
	if (index_line != "")
	{
		split_pos = 0;
		while ((split_pos = index_line.find(' ', split_pos + 1)) != string::npos)
		{
			if (index_line.find(' ', split_pos + 1) == string::npos)
				this->_index.push_back(index_line.substr(split_pos + 1));
			else
				this->_index.push_back(index_line.substr(split_pos + 1, index_line.find(' ', split_pos + 1) - split_pos - 1));
		}
		if (this->_index.empty() == true)
			error_bad_config();
	}
	
	// Setting methods
	string methods_line = get_configuration(location_config, "methods", true);
	this->_allowed_methods["GET"] = false;
	this->_allowed_methods["POST"] = false;
	this->_allowed_methods["DELETE"] = false;
	if (methods_line != "")
	{
		split_pos = 0;
		while ((split_pos = methods_line.find(' ', split_pos + 1)) != string::npos)
		{
			string method;
			if (methods_line.find(' ', split_pos + 1) == string::npos)
				method = methods_line.substr(split_pos + 1);
			else
				method = methods_line.substr(split_pos + 1, methods_line.find(' ', split_pos + 1) - split_pos - 1);
			if (method == "GET")
				this->_allowed_methods["GET"] = true;
			else if (method == "POST")
				this->_allowed_methods["POST"] = true;
			else if (method == "DELETE")
				this->_allowed_methods["DELETE"] = true;
			// else
				// error_bad_config("Bad method");
		}
	}
	else
	{
		this->_allowed_methods["GET"] = true;
		this->_allowed_methods["POST"] = true;
		this->_allowed_methods["DELETE"] = true;
	}
	
	// Setting auto index
	string auto_index_line = get_configuration(location_config, "autoindex", true);
	if (auto_index_line != "")
	{
		split_pos = auto_index_line.find(' ');
		if (split_pos == string::npos)
			error_bad_config();
		string boolean = auto_index_line.substr(split_pos + 1);
		if (boolean == "on")
			this->_auto_index = true;
		else
			this->_auto_index = false;
	}
	else
		this->_auto_index = false;

	// Setting redirection
	string redirect_line = get_configuration(location_config, "redirect", true);
	if (redirect_line != "")
	{
		split_pos = redirect_line.find(' ');
		if (split_pos == string::npos)
			error_bad_config();
		size_t second_split_pos = redirect_line.find(' ', split_pos + 1);
		if (second_split_pos == string::npos)
			error_bad_config();
		this->_redirect.first = redirect_line.substr(split_pos + 1, second_split_pos - split_pos - 1);
		this->_redirect.second = redirect_line.substr(second_split_pos + 1);
	}

	// Setting cgi_path
	string cgi_line = get_configuration(location_config, "cgi_path", true);
	if (cgi_line != "")
	{
		split_pos = cgi_line.find(' ');
		if (split_pos == string::npos)
			error_bad_config();
		this->_cgi_path = cgi_line.substr(split_pos + 1);
	}
}

Location::~Location()
{}





// ********************** GETTERS ********************** //

int Location::get_id() const
{ return (this->_id); }

string Location::get_path() const
{ return (this->_path); }

string Location::get_root() const
{ return (this->_root); }

std::vector<string> Location::get_index() const
{ return (this->_index); }

bool	Location::auto_index_is_on() const
{ return (this->_auto_index); }

bool	Location::method_is_allowed(const string method) const
{
	if ( this->_allowed_methods.find(method) == this->_allowed_methods.end() )
		return (false);
	else
		return (this->_allowed_methods.at(method));
}

string Location::get_cgi_path() const
{ return (this->_cgi_path); }

pair		Location::get_redirect() const 
{ return (this->_redirect); }





// ************ << OPERATOR **************** //

std::ostream &			operator<<( std::ostream & o, Location const & i )
{
	o << std::endl << "- Location " << i.get_path() << std::endl;
	o << "    Id:\t" << i.get_id() << std::endl;
	if (i.get_root() != "")
		o << "    Root:\t" << i.get_root() << std::endl;
	std::vector<string> loc_index = i.get_index();
	if (loc_index.empty() == false)
	{
		o << "    Index:\t";
		for (std::vector<string>::iterator it_index = loc_index.begin(); it_index != loc_index.end(); ++it_index)
		{
			o << *it_index << " ";
		}
		o << std::endl;
	}
	o << "    Autoindex:\t" << i.auto_index_is_on() << std::endl;
	if (i.get_cgi_path() != "")
		o << "    Cgi:\t" << i.get_cgi_path() << std::endl;
	if (i.get_redirect().first != "")
		o << "    Redirect:\t" << i.get_redirect().first << " " << i.get_redirect().second << std::endl;
	o << "    Available methods:";
	if (i.method_is_allowed("GET"))
		o << "  GET";
	if (i.method_is_allowed("POST"))
		o << "  POST";
	if (i.method_is_allowed("DELETE"))
		o << "  DELETE";
	o << std::endl;
	return o;
}