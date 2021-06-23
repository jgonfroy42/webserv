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
	request.set_unchunked_body("a parser");
	std::cout << "New body :" << request.get_body() << std::endl;
}
