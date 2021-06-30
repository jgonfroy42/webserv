#include "Location.hpp"
#include "../../includes/webserv.hpp"


// **************** LOCATION CONSTRUCTOR / DESTRUCTOR **************** //

Location::Location() { this->_id = -1; }

Location::Location(const string location_config, int id)
{
	// Checking if not another block inside location block
	size_t start_block = location_config.find('{', 0);
	if (location_config.find('{', start_block + 1) != string::npos)
		error_bad_config("Forbidden block inside location block.");
	
	this->_id = id;
	this->_path = location_config.substr(0, location_config.find(" "));
	set_root(location_config);
	set_indexes(location_config);
	set_methods(location_config);
	set_autoindex(location_config);
	set_redirection(location_config);
	set_cgi_path(location_config);
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
	if (this->is_empty())
		return true;
	if ( this->_allowed_methods.find(method) == this->_allowed_methods.end() )
		return (false);
	else
		return (this->_allowed_methods.at(method));
}

string Location::get_cgi_path() const
{ return (this->_cgi_path); }

string Location::get_cgi_extension() const
{ return (this->_cgi_extension); }

string Location::get_upload_path() const
{ return (this->_upload_path); }

pair		Location::get_redirect() const 
{ return (this->_redirect); }

bool		Location::is_empty() const
{
	if (this->_id == -1)
		return (true);
	return (false);
}

bool		Location::is_cgi() const
{
	if (this->_cgi_path == "")
		return (false);
	return (true);
}


bool		Location::root_is_set() const
{
	return this->_root_set;
}


// ******************** SETTERS / PARSER ****************** //

string	Location::get_configuration(const string server_config, const string label, bool optional)
{
	size_t label_pos = server_config.find(label);

	if (label_pos == string::npos && optional == false)
		error_bad_config(label + " missing.");
	else if (label_pos == string::npos && optional == true)
		return ("");

	size_t end_line_pos = server_config.find(";", label_pos);
	if (end_line_pos == string::npos)
		error_bad_config("Missing ; at end of line.");

	string line = server_config.substr(label_pos, end_line_pos - label_pos);
	
	// Check si doublon
	if (server_config.find(label, end_line_pos + 1) != string::npos)
		error_bad_config("Double " + label + ".");
	return (line);
}

void	Location::set_root(const string location_config)
{
	string root_line = get_configuration(location_config, "root", true);
	if (root_line != "")
	{
		_root_set = true;
		size_t split_pos = root_line.find(' ');
		if (split_pos == string::npos)
			error_bad_config("Invalid instruction. (root)");
		this->_root = root_line.substr(split_pos + 1);
	}
	else
		_root_set = false;

}

void	Location::set_indexes(const string location_config)
{
	string index_line = get_configuration(location_config, " index ", true);
	if (index_line != "")
	{
		size_t split_pos = 0;
		while ((split_pos = index_line.find(' ', split_pos + 1)) != string::npos)
		{
			if (index_line.find(' ', split_pos + 1) == string::npos)
				this->_index.push_back(index_line.substr(split_pos + 1));
			else
				this->_index.push_back(index_line.substr(split_pos + 1, index_line.find(' ', split_pos + 1) - split_pos - 1));
		}
		if (this->_index.empty() == true)
			error_bad_config("Invalid instruction. (index)");
	}
}

void	Location::set_methods(const string location_config)
{
	string methods_line = get_configuration(location_config, "methods", true);
	this->_allowed_methods["GET"] = false;
	this->_allowed_methods["POST"] = false;
	this->_allowed_methods["DELETE"] = false;
	if (methods_line != "")
	{
		size_t split_pos = 0;
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
		}
	}
	else
	{
		this->_allowed_methods["GET"] = true;
		this->_allowed_methods["POST"] = true;
		this->_allowed_methods["DELETE"] = true;
	}
}

void	Location::set_autoindex(const string location_config)
{
	string auto_index_line = get_configuration(location_config, "autoindex", true);
	if (auto_index_line != "")
	{
		size_t split_pos = auto_index_line.find(' ');
		if (split_pos == string::npos)
			error_bad_config("Invalid instruction. (autoindex)");
		string boolean = auto_index_line.substr(split_pos + 1);
		if (boolean == "on")
			this->_auto_index = true;
		else
			this->_auto_index = false;
	}
	else
		this->_auto_index = false;
}

void	Location::set_redirection(const string location_config)
{
	string redirect_line = get_configuration(location_config, "redirect", true);
	if (redirect_line != "")
	{
		size_t split_pos = redirect_line.find(' ');
		if (split_pos == string::npos)
			error_bad_config("Invalid instruction. (redirect)");
		size_t second_split_pos = redirect_line.find(' ', split_pos + 1);
		if (second_split_pos == string::npos)
			error_bad_config("Invalid instruction. (redirect)");
		this->_redirect.first = redirect_line.substr(split_pos + 1, second_split_pos - split_pos - 1);
		this->_redirect.second = redirect_line.substr(second_split_pos + 1);
	}
	else
		_redirect = pair_str_str(string(), string());
}

void	Location::set_cgi_path(const string location_config)
{
	string cgi_line = get_configuration(location_config, "cgi_path", true);
	if (cgi_line != "")
	{
		size_t split_pos = cgi_line.find(' ');
		if (split_pos == string::npos)
			error_bad_config("Invalid instruction. (cgi_path)");
		this->_cgi_path = cgi_line.substr(split_pos + 1);
	}
	else
		this->_cgi_path = "";
	
	cgi_line = get_configuration(location_config, "cgi_extension", true);
	if (cgi_line != "")
	{
		size_t split_pos = cgi_line.find(' ');
		if (split_pos == string::npos)
			error_bad_config("Invalid instruction. (cgi_extension)");
		this->_cgi_extension = cgi_line.substr(split_pos + 1);
	}
	else
		this->_cgi_extension = "";
	
	cgi_line = get_configuration(location_config, "upload", true);
	if (cgi_line != "")
	{
		size_t split_pos = cgi_line.find(' ');
		if (split_pos == string::npos)
			error_bad_config("Invalid instruction. (upload_path)");
		this->_upload_path = cgi_line.substr(split_pos + 1);
	}
	else
		this->_upload_path = "";
}





// ************ << OPERATOR **************** //

std::ostream &			operator<<( std::ostream & o, Location const & i )
{
	if (i.is_empty())
	{
		o << "\n>empty location<" << std::endl;
		return o;
	}
	o << std::endl << "- Location " << i.get_path() << std::endl;
	o << "    Id:\t" << i.get_id() << std::endl;
	if (i.get_root() != "")
		o << "    Root:\t" << i.get_root() << std::endl;
	std::vector<string> indexes = i.get_index();
	if (indexes.empty() == false)
	{
		o << "    Index:\t";
		for (std::vector<string>::iterator it = indexes.begin(); it != indexes.end(); ++it)
		{
			o << *it << " ";
		}
		o << std::endl;
	}
	o << "    Autoindex:\t" << i.auto_index_is_on() << std::endl;
	if (i.get_cgi_path() != "")
		o << "    Cgi:\t" << i.get_cgi_path() << std::endl;
	if (i.get_cgi_extension() != "")
		o << "    Cgi ext:\t" << i.get_cgi_extension() << std::endl;
	if (i.get_upload_path() != "")
		o << "    Upload:\t" << i.get_upload_path() << std::endl;
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
