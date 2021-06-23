#include "./../includes/webserv.hpp"

int	parse_chunked_body(Request request)
{
	std::string old_body = request.get_body();
	std::cout << "size : " << old_body.size() << std::endl;
	if (old_body.size() == 5)
	{
		if (old_body[0] == '0')
		{
			request.set_unchunked_body("");
			return 0;
		}
		return 1;
	}
	//split toutes les lignes dans un vector de string et ajouter les lignes qui nous interessent l'un apres l'autre ?
	//on peut verifier si chaque ligne correspond bien au chiffre

	std::vector<string> token;
	string del = "\r\n";

	size_t pos_find, start_find = 0;
	while ((pos_find = old_body.find(del, start_find)) != string::npos)
	{
		std::cout << "start = " << start_find << " end = " << pos_find << std::endl;
		std::string line = old_body.substr(start_find, pos_find);
		std::cout << "size line = " << line.size() << std::endl;
		token.push_back(line);
		std::cout << "size token = " << token.back().size() << std::endl;
		start_find = pos_find + del.size();
	}
//	std::cout << "///////" << token[1] << std::endl;
	std::cout << "size line = " << token[1].size() << std::endl;
	request.set_unchunked_body("a parser");
	std::cout << "New body :" << request.get_body() << std::endl;
	return 0;
}
