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
	std::string op = " :           __________                                 \n         .'----------`.                              \n         | .--------. |                             \n         | |########| |       __________              \n         | |########| |      /__________\\             \n.--------| `--------' |------|    --=-- |-------------.\n|        `----,-.-----'      |o ======  |             | \n|       ______|_|_______     |__________|             | \n|      /  %%%%%%%%%%%%  \\                             | \n|     /  %%%%%%%%%%%%%%  \\                            | \n|     ^^^^^^^^^^^^^^^^^^^^                            | \n+-----------------------------------------------------+\n^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ ";
	std::cout<<Base(clt, "001") + clt.get_nick() + op<<std::endl;
	return (Base(clt, "001") + clt.get_nick() + op);
}

std::string Code::RPL_YOURHOST(Client &clt)
{
	std::string op = " :Your host is ..., running version";
	std::cout<<Base(clt, "002") + clt.get_nick() + op<<std::endl;
	return (Base(clt, "002") + clt.get_nick() + op);
}

std::string Code::RPL_CREATED(Client &clt)
{
	std::string op = " :Your host is ..., running version";
	std::cout<<Base(clt, "003") + clt.get_nick() + op<<std::endl;
	return (Base(clt, "003") + clt.get_nick() + op);
}

std::string Code::RPL_INVITING(Client &clt, std::string title)
{
	std::string op = " inviteduser ";
	//std::cout<<Base(clt, "003") + clt.get_nick() + op<<std::endl;
	return (Base(clt, "321") + clt.get_nick() + op + title);
}

std::string Code::RPL_LISTSTART(Client &clt)
{
	std::string op = " Channel :Users Name";
	return (Base(clt, "321") + clt.get_nick() + op);
}

std::string Code::RPL_LIST(Client &clt, std::string name, int nb)
{
	std::string op = " ";
	std::string space = " ";
	std::stringstream stream;
	stream<<nb;
	std::string nbr = stream.str();
	std::cout<<Base(clt, "322") + clt.get_nick() + op + name + space + nbr<<std::endl;
	return (Base(clt, "322") + clt.get_nick() + op + name + space + nbr);
}

std::string Code::RPL_LISTEND(Client &clt)
{
	std::string op = " :End of /LIST";
	return (Base(clt, "323") + clt.get_nick() + op);
}

std::string Code::RPL_NAMREPLY(Client &clt, Server &serv, std::string title)
{
	std::string op = " :End of /LIST";
	std::string mod_chan;
	std::string rep = " :";
	// if (serv.chan_has_mod(title, C_SECRET) == 1)
	// 	mod_chan = " @ ";
	// else if (serv.chan_has_mod(title, C_PRIVATE) == 1)
	// 	mod_chan = " * ";
	mod_chan = " = ";
	std::map<int, std::string>::iterator it;
	for (it = serv.pool_channel.find(title)->second->_members.begin(); it != serv.pool_channel.find(title)->second->_members.end(); it++)
	{
		std::string mod_user;
		if(serv.get_userinchan_mods(title, clt).find(CU_CREATOR) != std::string::npos)
			mod_user = "~";
		// else if (clt._mods.find(U_AWAY) != std::string::npos)
		// 	mod_user = "&";
		else if (serv.get_userinchan_mods(title, clt).find(CU_OPERATOR) != std::string::npos)
			mod_user = "@";
		// else if (serv.get_userinchan_mods(title, clt).find(CU_VOICE) != std::string::npos)
		// 	mod_user = "+";
		else
			mod_user = "";
		rep = rep + " " + mod_user + serv.pool_client[it->first]->get_nick();
	}
	return (Base(clt, "353") + clt.get_nick() + mod_chan + title + rep);
}

std::string Code::RPL_ENDOFWHO(Client &clt)
{
	std::string op = " :End of WHO list";
	std::string space = " ";
	return (Base(clt, "315") + clt.get_nick() + op);
}

std::string Code::RPL_ENDOFNAMES(Client &clt, std::string title)
{
	std::string op = " :End of /NAMES";
	std::string space = " ";
	return (Base(clt, "366") + clt.get_nick() + space + title + op);
}

//a reprendre 
std::string Code::RPL_UMODEIS(Client &clt)
{
	std::string op = " :Your host is ..., running version";
	std::string sign = " +";
	std::cout<<"mode user : "<<clt._mods<<std::endl;
	return (Base(clt, "221") + clt.get_nick() + sign + clt._mods);
}

//AJOUTER le + au debut de la string des mods
std::string Code::RPL_CHANNELMODEIS(Client &clt, std::string title, std::string mods)
{
	std::string op = " ";
	std::string space = "";
	std::cout<<"mode channel : "<<clt._mods<<std::endl;
	return (Base(clt, "324") + clt.get_nick() + op + title + op + mods + space);
}

std::string Code::RPL_MYINFO(Client &clt)
{
	std::string space = " ";
	std::string op = "IRC_perso_maggle";
	std::cout<<Base(clt, "004")+ clt.get_nick() +space+ clt._servername +space+ op<<std::endl;
	return (Base(clt, "004")+ clt.get_nick() +space+ clt._servername +space+ op);
}

std::string Code::RPL_ISUPPORT(Client &clt)
{
	std::string space = " ";
	std::string op = "MODES=4";
	std::cout<<Base(clt, "005")+ clt.get_nick() +space+ op<<std::endl;
	return (Base(clt, "005")+ clt.get_nick() +space+ op);
}

std::string Code::ERR_NEEDMOREPARAMS(std::string cmd, Client &clt)
{
	std::string reply = " :Not enough parameters";
	return (Base(clt, "461") + cmd + reply);
}

std::string Code::ERR_UNKNOWNCOMMAND(std::string cmd, Client &clt)
{
	std::string reply = " :Too many parameters or unknown command";
	return (Base(clt, "421") + cmd + reply);
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

//a revoir // envoi a tous les clients ?
std::string Code::ERR_CHANOPRIVSNEEDED(Client &clt, std::string title)
{
	std::string space = " ";
	std::string reply = " :You're not a channel operator";
	return (Base(clt, "482") + space + clt._nickname + space + title + reply);
}

std::string Code::ERR_NOSUCHCHANNEL(Client &clt, std::string name)
{
	std::string space = " ";
	std::string reply = " :No such channel";
	return (Base(clt, "403") + space + clt._nickname +space + name+ reply);
}

std::string Code::ERR_USERONCHANNEL(Client &clt, std::string name)
{
	std::string space = " ";
	std::string reply = " :is already on channel";
	return (Base(clt, "443") + space + clt._nickname +space + name+ reply);
}

std::string Code::ERR_NOSUCHNICK(Client &clt)
{
	std::string space = " ";
	std::string reply = " :No such nick";
	return (Base(clt, "401") + space + clt._nickname +reply);
}

std::string	Code::ERR_CANNOTSENDTOCHAN(Client &clt, std::string channel)
{
	return (Base(clt, "404") + " " + \
		clt._nickname + " " + channel + " :Cannot send to channel");
}

std::string	Code::ERR_TOOMANYTARGETS(Client &clt, std::string channel)
{
	return (Base(clt, "407") + " " + \
		clt._nickname + " " + channel + " :Too many targets");
}

std::string	Code::ERR_NOTEXTTOSEND(Client &clt)
{
	return (Base(clt, "412") + " " + \
		clt._nickname + " :No text to send");
}

std::string Code::ERR_NOTONCHANNEL(Client &clt, std::string name)
{
	std::string space = " ";
	std::string reply = " :You're not on that channel";
	return (Base(clt, "442") + space + clt._nickname +space + name+ reply);
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