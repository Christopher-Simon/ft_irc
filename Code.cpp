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
	//:irc.example.com 001 mynick :Welcome to the example IRC Network mynick!user@host
	//std::string op1 = ":";
	//std::string space = " ";
	//std::string op2 = "001";
	std::string op = " :Welcome to the ... Network,\n";
	std::cout<<Base(clt, "001") + clt.get_nick() + op<<std::endl;
	return (Base(clt, "001") + clt.get_nick() + op);
}

std::string Code::RPL_YOURHOST(Client &clt)
{
	//"<client> :Your host is <servername>, running version <version>"
	// std::string op1 = ":";
	// std::string space = " ";
	// std::string op2 = "002";
	std::string op = " :Your host is ..., running version\n";
	std::cout<<Base(clt, "002") + clt.get_nick() + op<<std::endl;
	return (Base(clt, "002") + clt.get_nick() + op);
}

std::string Code::RPL_CREATED(Client &clt)
{
	// std::string op1 = ":";
	// std::string space = " ";
	// std::string op2 = "003";
	std::string op = " :Your host is ..., running version\n";
	std::cout<<Base(clt, "003") + clt.get_nick() + op<<std::endl;
	return (Base(clt, "003") + clt.get_nick() + op);
}

std::string Code::RPL_MYINFO(Client &clt)
{
	// std::string op1 = ":";
	std::string space = " ";
	// std::string op2 = "004";
	std::string op = "IRC_perso_maggle\n";
	std::cout<<Base(clt, "004")+ clt.get_nick() +space+ clt._servername +space+ op<<std::endl;
	return (Base(clt, "004")+ clt.get_nick() +space+ clt._servername +space+ op);
}

std::string Code::RPL_ISUPPORT(Client &clt)
{
	// std::string op1 = ":";
	std::string space = " ";
	// std::string op2 = "005";
	std::string op = "MODES=4\n";
	std::cout<<Base(clt, "005")+ clt.get_nick() +space+ op<<std::endl;
	return (Base(clt, "005")+ clt.get_nick() +space+ op);
}

std::string Code::ERR_NEEDMOREPARAMS(std::string cmd, Client &clt)
{
	std::string reply = " :Not enough parameters";
	return (Base(clt, "461") + cmd + reply);
}