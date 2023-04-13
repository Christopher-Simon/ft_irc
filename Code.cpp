#include "Code.hpp"


Code::Code()
{

}

Code::~Code()
{

}

std::string Code::Base(Client &clt, std::string nb)
{
	std::string space = " ";
	return (":" + clt.get_nick() + "!" + clt._username + "@"+ clt._hotsname + space + nb + space);
}

std::string Code::RPL_WELCOME(Client &clt)
{
	std::string op = " :           __________                                 \n         .'----------`.                              \n         | .--------. |                             \n         | |########| |       __________              \n         | |########| |      /__________\\             \n.--------| `--------' |------|    --=-- |-------------.\n|        `----,-.-----'      |o ======  |             | \n|       ______|_|_______     |__________|             | \n|      /  %%%%%%%%%%%%  \\                             | \n|     /  %%%%%%%%%%%%%%  \\                            | \n|     ^^^^^^^^^^^^^^^^^^^^                            | \n+-----------------------------------------------------+\n^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ ";
	std::cout<<Base(clt, "001") + clt.get_nick() + op<<std::endl;
	return (Base(clt, "001") + clt.get_nick() + op);
}

std::string Code::RPL_YOURHOST(Client &clt)
{
	std::string op = " :Your host is CHSIMON, running the homemade version";
	std::cout<<Base(clt, "002") + clt.get_nick() + op<<std::endl;
	return (Base(clt, "002") + clt.get_nick() + op);
}

std::string Code::RPL_CREATED(Client &clt)
{
	std::string op = " :Supported commands : INVITE - JOIN - KICK - KILL - LIST - MODE - NAMES - NICK - NOTICE - OPER - PART - PASS - PING - PRIVMSG - QUIT - TOPIC - USER";
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
	return (Base(clt, "321") + clt.get_nick() + " Channel :Users Topic");
}

std::string Code::RPL_LIST(Client &clt, std::string name, int nb, std::string topic)
{
	std::stringstream stream;
	stream<<nb;
	std::string nbr = stream.str();
	return (Base(clt, "322") + clt.get_nick() + " " + name + " " + nbr + " :" + topic);
}

std::string Code::RPL_LISTEND(Client &clt)
{
	return (Base(clt, "323") + clt.get_nick() + " :End of /LIST");
}

std::string Code::RPL_NAMREPLY(Client &clt, Server &serv, std::string title)
{
	std::string mod_chan;
	std::string rep = " ";
	std::map<int, std::string>::iterator it;
	for (it = serv.pool_channel.find(title)->second->_members.begin(); it != serv.pool_channel.find(title)->second->_members.end(); it++)
	{
		std::string mod_user;
		if (it == serv.pool_channel.find(title)->second->_members.begin())
			rep = rep + ":";
		else
			rep = rep + " ";
		if (it->second.find(CU_OPERATOR) != std::string::npos)
			mod_user = "@";
		else
			mod_user = "";
		rep = rep + mod_user + serv.pool_client[it->first]->get_nick();
	}
	//if (serv.get_chan(title)->_channel_mods.find('i') != std::string::npos)
	return (Base(clt, "353") + clt.get_nick() + " = " + title + rep);
}

std::string Code::RPL_ENDOFWHO(Client &clt)
{
	return (Base(clt, "315") + clt.get_nick() + " :End of WHO list");
}

std::string Code::RPL_NOTOPIC(Client &clt, std::string chan_name)
{
	return (Base(clt, "331") + clt.get_nick() + " " + chan_name + " :No topic is set");
}

std::string Code::RPL_YOUREOPER(Client &clt)
{
	return (Base(clt, "381") + clt.get_nick() + " :You are now an IRC operator");
}

std::string Code::RPL_TOPIC(Client &clt, std::string chan_name, std::string topic)
{
	return (Base(clt, "332") + clt.get_nick() + " " + chan_name + " :" + topic);
}

std::string Code::RPL_ENDOFNAMES(Client &clt, std::string title)
{
	std::string op = " :End of /NAMES list";
	return (Base(clt, "366") + clt.get_nick() + " " + title + op);
}

std::string Code::RPL_UMODEIS(Client &clt)
{
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
	std::string op = "^^^^^^^^^^^^^^^^^^THIS IS THE IRC^^^^^^^^^^^^^^^^^^^^^^";
	std::cout<<Base(clt, "004")+ clt.get_nick() +space+ clt._servername +space+ op<<std::endl;
	return (Base(clt, "004")+ clt.get_nick() +space+ op);
}

std::string Code::RPL_ISUPPORT(Client &clt)
{
	std::string space = " ";
	std::string op = "PREFIX=(o)@ MODES=2 CHANLIMIT=#:10 MAXTARGETS=20 :are supported by this server";
	std::cout<<Base(clt, "005")+ clt.get_nick() +space+ op<<std::endl;
	return (Base(clt, "005")+ clt.get_nick() +space+ op);
}

std::string Code::ERR_NEEDMOREPARAMS(std::string cmd, Client &clt)
{
	return (Base(clt, "461") + cmd + " :Not enough parameters");
}

std::string Code::ERR_UNKNOWNCOMMAND(std::string cmd, Client &clt)
{
	return (Base(clt, "421") + cmd + " :Too many parameters or unknown command");
}

std::string Code::ERR_NONICKNAMEGIVEN(Client &clt)
{
	return (Base(clt, "431") + clt._nickname + " :No nickname given");
}

std::string Code::ERR_NICKNAMEINUSE(std::string &nick, Client &clt)
{
	std::string reply = " :Nickname is already in use";
	return (Base(clt, "433") + "* " + nick + reply);
}

std::string Code::ERR_ALREADYREGISTERED(Client &clt)
{
	std::string reply = " :You may not reregister";
	return (Base(clt, "462") + clt._nickname + reply);
}

std::string Code::ERR_NOTREGISTERED(Client &clt)
{
	std::string reply = " :You have not registered";
	return (Base(clt, "451") + clt._nickname + reply);
}

std::string Code::ERR_PASSWDMISMATCH(Client &clt)
{
	std::string reply = " :Password incorrect";
	return (Base(clt, "464") + clt._nickname + reply);
}

std::string Code::ERR_UMODEUNKNOWNFLAG(Client &clt)
{
	std::string reply = " :Unknown MODE flag";
	return (Base(clt, "501") + clt._nickname + reply);
}

std::string Code::ERR_NOORIGIN(Client &clt)
{
	std::string reply = " :No origin specified";
	return (Base(clt, "409") + clt._nickname + reply);
}

//a revoir // envoi a tous les clients ?
std::string Code::ERR_CHANOPRIVSNEEDED(Client &clt, std::string title)
{
	std::string reply = " :You're not a channel operator";
	return (Base(clt, "482") + clt._nickname + " " + title + reply);
}

std::string Code::ERR_NOSUCHCHANNEL(Client &clt, std::string name)
{
	std::string reply = " :No such channel";
	return (Base(clt, "403") + clt._nickname + " "+ name+ reply);
}

std::string Code::ERR_INVALIDCHANNELNAME(Client &clt, std::string name)
{
	std::string reply = " :Illegal channel name";
	return (Base(clt, "479") + clt._nickname + " " + name+ reply);
}

std::string Code::ERR_USERONCHANNEL(Client &clt, std::string name)
{
	std::string reply = " :is already on channel";
	return (Base(clt, "443") + clt._nickname + " " + name+ reply);
}

std::string Code::ERR_INVITEONLYCHAN(Client &clt, std::string name)
{
	std::string reply = " :Cannot join channel (+i)";
	return (Base(clt, "473") + clt._nickname + " " + name+ reply);
}

std::string Code::ERR_NOSUCHNICK(Client &clt)
{
	std::string reply = " :No such nick";
	return (Base(clt, "401") + clt._nickname +reply);
}

std::string Code::ERR_NOOPERHOST(Client &clt)
{
	std::string reply = " :No O-lines for your host";
	return (Base(clt, "491") + clt._nickname +reply);
}

std::string	Code::ERR_CANNOTSENDTOCHAN(Client &clt, std::string channel)
{
	return (Base(clt, "404") + \
		clt._nickname + " " + channel + " :Cannot send to channel");
}

std::string	Code::ERR_TOOMANYTARGETS(Client &clt, std::string channel)
{
	return (Base(clt, "407") + \
		clt._nickname + " " + channel + " :Too many targets");
}

std::string	Code::ERR_NOTEXTTOSEND(Client &clt)
{
	return (Base(clt, "412") + \
		clt._nickname + " :No text to send");
}


std::string Code::ERR_NOTONCHANNEL(Client &clt, std::string name)
{
	std::string reply = " :You're not on that channel";
	return (Base(clt, "442") + clt._nickname + " "+ name+ reply);
}
 
std::string Code::ERR_USERNOTINCHANNEL(Client &clt, std::string target, std::string channel)
{
	return (Base(clt, "441")  + \
	clt._servername + " " + target + " " + channel +\
	 " :They aren't on that channel");

}

std::string Code::ERR_BADCHANNELKEY(Client &clt, std::string name)
{
	std::string reply = " :Cannot join channel (+k)";
	return (Base(clt, "475") + clt.get_nick() + " "+ name+ reply);
}

std::string Code::ERR_NOPRIVILEGES(Client &clt)
{
	return (Base(clt, "481") + clt.get_nick() + " :Permission Denied- You're not an IRC operator");
}


std::string Code::ERR_TOOMANYCHANNELS(Client &clt, std::string titlechan)
{
	return (Base(clt, "405") + clt.get_nick() +" " + titlechan + " :You have joined too many channels");
}

std::string Code::ERR_UNKNOWNMODE(Client &clt, std::string mods)
{
	std::string mod;
	if (mods.length() > 1 && (mods[0] == '+' || mods[0] == '-'))
		mod = mods.substr(1, mods.length());
	else
		mod = mods;
	return (Base(clt, "472") + clt.get_nick() + " " + mod + " :is unknown mode char to me");
}

std::string Code::ERR_USERSDONTMATCH(Client &clt)
{
	return (Base(clt, "502") + clt.get_nick() + " :Cant change mode for other users");
}
