#include "Code.hpp"


Code::Code()
{

}

Code::~Code()
{

}

std::string Code::Base(Client &clt, std::string nb)
{
	std::string sep = ":";
	std::string space = " ";
	return (sep + clt._servername + space + nb + space);
}

std::string Code::RPL_WELCOME(Client &clt)
{
	std::string op = " :Welcome to the ... Network,\n";
	std::cout<<Base(clt, "001") + clt.get_nick() + op<<std::endl;
	return (Base(clt, "001") + clt.get_nick() + op);
}

std::string Code::RPL_YOURHOST(Client &clt)
{
	std::string op = " :Your host is ..., running version\n";
	std::cout<<Base(clt, "002") + clt.get_nick() + op<<std::endl;
	return (Base(clt, "002") + clt.get_nick() + op);
}

std::string Code::RPL_CREATED(Client &clt)
{
	std::string op = " :Your host is ..., running version\n";
	std::cout<<Base(clt, "003") + clt.get_nick() + op<<std::endl;
	return (Base(clt, "003") + clt.get_nick() + op);
}

std::string Code::RPL_MYINFO(Client &clt)
{
	std::string space = " ";
	std::string op = "IRC_perso_maggle\n";
	std::cout<<Base(clt, "004")+ clt.get_nick() +space+ clt._servername +space+ op<<std::endl;
	return (Base(clt, "004")+ clt.get_nick() +space+ clt._servername +space+ op);
}

std::string Code::RPL_ISUPPORT(Client &clt)
{
	std::string space = " ";
	std::string op = "MODES=4\n";
	std::cout<<Base(clt, "005")+ clt.get_nick() +space+ op<<std::endl;
	return (Base(clt, "005")+ clt.get_nick() +space+ op);
}

std::string Code::ERR_NEEDMOREPARAMS(std::string cmd, Client &clt)
{
	std::string reply = " :Not enough parameters";
	return (Base(clt, "461") + cmd + reply);
}

// std::string Code::ERR_NICKNAMEINUSE(std::string cmd, Client &clt)
// std::string Code::ERR_PASSWDMISMATCH(std::string cmd, Client &clt)
// std::string Code::ERR_UNKNOWNCOMMAND(std::string cmd, Client &clt)
// std::string Code::ERR_NOMOTD(std::string cmd, Client &clt)
// std::string Code::ERR_NONICKNAMEGIVEN(std::string cmd, Client &clt)
// std::string Code::ERR_ERRONEUSNICKNAME(std::string cmd, Client &clt)
// std::string Code::ERR_NICKCOLLISION(std::string cmd, Client &clt)
// std::string Code::ERR_USERNOTINCHANNEL(std::string cmd, Client &clt)
// std::string Code::ERR_NOTONCHANNEL(std::string cmd, Client &clt)
// std::string Code::ERR_NOTREGISTERED(std::string cmd, Client &clt)
// std::string Code::ERR_ALREADYREGISTERED(std::string cmd, Client &clt)
// std::string Code::ERR_CHANOPRIVSNEEDED(std::string cmd, Client &clt)
// std::string Code::ERR_NOOPERHOST(std::string cmd, Client &clt)
// std::string Code::ERR_UMODEUNKNOWNFLAG(std::string cmd, Client &clt)
// std::string Code::ERR_USERSDONTMATCH(std::string cmd, Client &clt)