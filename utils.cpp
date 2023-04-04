#include "Command.hpp"

void print_vect(std::vector<std::string> vect)
{
	for (size_t i = 0; i < vect.size(); i++)
		std::cout << "vect["<<i<<"]: "<<vect[i]<<std::endl;
}

std::vector<std::string> ft_split(std::string &msg, char sep)
{
	std::vector<std::string> vect;
	std::istringstream f(msg);
	std::string s;
	if (msg.find(sep) == std::string::npos)
		return (vect.push_back(msg), vect);
	while (getline(f, s, sep))
		vect.push_back(s);
	print_vect(vect);
	return (vect);
}