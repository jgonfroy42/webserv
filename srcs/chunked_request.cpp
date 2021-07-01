#include "./../includes/webserv.hpp"

bool	is_hexa(std::string str)
{
	std::string::iterator	it;

	for (it = str.begin(); it != str.end(); ++it)
	{
		if (!isdigit(*it) && !(*it > 96 && *it < 103))
			return false;
	}
	return true;
}

//base base 10;
unsigned long ft_stoi(std::string str)
{
	int num;
	std::stringstream ss;

	ss << str;
	ss >> num;
	return num;
}

Request	parse_chunked_body(Request request)
{
	unsigned long	size_line;
	std::string new_body = "";
	std::string old_body = request.get_body();

	request.set_chunked_error(true);

	if (old_body.size() == 5)
	{
		if (old_body[0] == '0')
		{
			request.set_unchunked_body("");
			request.set_chunked_error(false);
			return request;
		}
		return request;
	}
	//split toutes les lignes dans un vector de string et ajouter les lignes qui nous interessent l'un apres l'autre ?
	//on peut verifier si chaque ligne correspond bien au chiffre

	std::vector<string> token;
	string del = "\r\n";

	size_t pos_find, start_find = 0;
	while ((pos_find = old_body.find(del, start_find)) != string::npos)
	{
		std::string line = old_body.substr(start_find, (pos_find - start_find));
		token.push_back(line);
		start_find = pos_find + del.size();
	}
	for (size_t i = 0; i < token.size(); ++i)
	{
		if (i % 2 != 0)
			continue;
		if (is_hexa(token[i]) == false)
				return request;
		size_line = strtol(token[i].c_str(), NULL, 16);
		if (size_line == 0)
			break;
		if (i + 1 > token.size() || size_line != token[i + 1].size())
			return request;
		new_body = new_body + token[i + 1];
	}
	//ajouter EOF donc "\r\n" ?
	new_body = new_body + "\r\n";
	request.set_unchunked_body(new_body);
	request.set_chunked_error(false);
	return request;
}
