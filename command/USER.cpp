#include "Command.hpp"

void Command::USER(std::string cmd, std::string line, Server &serv, Client &clt)
{
	if (clt._identified == 3)
	{
		serv.send_msg(ircrep->ERR_ALREADYREGISTERED(clt),clt.getfd());
		return;
	}
	//PARSING PAS PARFAIT
	std::cout<<line<<std::endl;
	std::vector<std::string> vect;
	size_t index = 0;
	vect.push_back(line.substr(index + 5, line.find_first_of(" ", index + 5)- index - 5));
	index = line.find_first_of(" ", index + 6) + 1;
	vect.push_back(line.substr(index, line.find_first_of(" ", index + 1)- index));
	index = line.find_first_of(" ", index + 1) + 1;
	vect.push_back(line.substr(index, line.find_first_of(" ", index + 1)- index));
	index = line.find_first_of(" ", index + 1) + 2; //check l'histoire du :
	vect.push_back(line.substr(index, line.find_first_of("\r\n", index + 1)- index));
	if (vect.size() != 4)
	{
		serv.send_msg(ircrep->ERR_NEEDMOREPARAMS(cmd, clt),clt.getfd());
		return;
	}
	clt._username = vect[0];
	std::cout<<clt._username<<std::endl;
	clt._hotsname = vect[1];
	std::cout<<clt._hotsname<<std::endl;
	clt._servername = vect[2];
	std::cout<<clt._servername<<std::endl;
	clt._realname = vect[3];
	std::cout<<clt._realname<<std::endl;
	clt._identified++;
	std::cout<<clt._identified<<std::endl;
	clt.check_registered(serv, *this);
	std::cout<<cmd<<std::endl;
}