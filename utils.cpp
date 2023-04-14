#include "Command.hpp"

void print_vect(std::vector<std::string> vect)
{
	for (size_t i = 0; i < vect.size(); i++)
		std::cout << "vect["<<i<<"]: "<<vect[i]<<std::endl;
}

std::vector<std::string> ft_split(std::string &msg, char sep)
{
	std::vector<std::string> vect;
	
	size_t end;
	if (msg.find(":", 1) == std::string::npos)
		end = msg.length();
	else
		end = msg.find(":", 1);
	std::string s;
	std::istringstream f(msg.substr(0, end));
	if (msg.find(sep) == std::string::npos)
		return (vect.push_back(msg), vect);
	while (getline(f, s, sep))
		vect.push_back(s);
	if (msg.find(":", 1) != std::string::npos)
		vect.push_back(msg.substr(end, msg.length()));
	if (VERBOSE)
		print_vect(vect);
	return (vect);
}