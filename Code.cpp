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
	std::string op = " :           __________                                 \n         .'----------`.                              \n         | .--------. |                             \n         | |########| |       __________              \n         | |########| |      /__________\\             \n.--------| `--------' |------|    --=-- |-------------.\n|        `----,-.-----'      |o ======  |             | \n|       ______|_|_______     |__________|             | \n|      /  %%%%%%%%%%%%  \\                             | \n|     /  %%%%%%%%%%%%%%  \\                            | \n|     ^^^^^^^^^^^^^^^^^^^^                            | \n+-----------------------------------------------------+\n^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ \n";
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

std::string Code::RPL_UMODEIS(Client &clt)
{
	std::string op = " :Your host is ..., running version\n";
	std::string sign = " +";
	//std::cout<<Base(clt, "003") + clt.get_nick() + op<<std::endl;
	std::cout<<"mode user : "<<clt._mods<<std::endl;
	return (Base(clt, "221") + clt.get_nick() + sign + clt._mods);
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

std::string Code::ERR_NONICKNAMEGIVEN(Client &clt)
{
	std::string space = " ";
	std::string reply = " :No nickname given";
	return (Base(clt, "431") + space + clt._nickname + reply);
}

std::string Code::ERR_NICKNAMEINUSE(std::string &nick, Client &clt)
{
	std::string space = " ";
	std::string reply = " :Nickname is already in use";
	return (Base(clt, "433") + space + nick + reply);
}

std::string Code::ERR_ALREADYREGISTERED(Client &clt)
{
	std::string space = " ";
	std::string reply = " :You may not reregister";
	return (Base(clt, "462") + space + clt._nickname + reply);
}

std::string Code::ERR_NOTREGISTERED(Client &clt)
{
	std::string space = " ";
	std::string reply = " :You have not registered";
	return (Base(clt, "451") + space + clt._nickname + reply);
}

std::string Code::ERR_PASSWDMISMATCH(Client &clt)
{
	std::string space = " ";
	std::string reply = " :Password incorrect";
	return (Base(clt, "464") + space + clt._nickname + reply);
}

std::string Code::ERR_UMODEUNKNOWNFLAG(Client &clt)
{
	std::string space = " ";
	std::string reply = " :Unknown MODE flag";
	return (Base(clt, "501") + space + clt._nickname + reply);
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