#include "Command.hpp"

void Command::NICK(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt)
{
	if (vect.size() != 2)
	{
		serv.send_msg(ircrep->ERR_NONICKNAMEGIVEN(clt),clt.getfd());
		return;
	}
	std::string ptl_nick = vect[1];
	std::transform(ptl_nick.begin(), ptl_nick.end(), ptl_nick.begin(), ::toupper);
	Server::mapClient::iterator it;
	if (serv.check_nick_exist(ptl_nick) != 0
		|| vect[1].find_first_of(" ,*?!@", 0) != std::string::npos 
		|| vect[1][0] == '$' || vect[1][0] == ':' || vect[1][0] == '#' || vect[1][0] == '&')
	{
		serv.send_msg(ircrep->ERR_NICKNAMEINUSE(vect[1], clt),clt.getfd());
		return;
	}
	std::string prev_nick = clt._nickname;
	clt._nickname = vect[1];
	clt._intern_nick = ptl_nick;
	if (clt._identified == 1)
		clt._identified++;
	else if (clt._identified == 3)
	{
		std::string reply = ":" + prev_nick + "!" + clt._username + "@" + clt._servername + " NICK :" + vect[1];
		//serv.send_msg(reply,clt.getfd());
		//prevention des autres clients
		std::map<int, Client *>::iterator it;
		for (it = serv.pool_client.begin(); it != serv.pool_client.end(); it++)
		{
			serv.send_msg(reply, it->second->getfd());
		}
	}
	cmd.empty(); 
}
