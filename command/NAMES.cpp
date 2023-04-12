#include "../Command.hpp"

void Command::NAMES(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt)
{
	if (clt._identified < 3)
		serv.store_msg(ircrep->ERR_NOTREGISTERED(clt),clt.getfd());
	else if (clt._mods.find('o') == std::string::npos)
		serv.store_msg(ircrep->ERR_NOPRIVILEGES(clt),clt.getfd());
	else if (vect.size() > 2)
		serv.store_msg(ircrep->ERR_UNKNOWNCOMMAND(cmd, clt),clt.getfd());
	else if (vect.size() == 1)
	{
		std::map<std::string, Channel *>::iterator it;
		std::string total;
		for (it = serv.pool_channel.begin(); it != serv.pool_channel.end(); it++)
		{
			serv.store_msg(ircrep->RPL_NAMREPLY(clt, serv, it->first), clt.getfd());
			if (it == serv.pool_channel.begin())
				total = it->first;
			else
				total = total + "," + it->first;
		}
		serv.store_msg(ircrep->RPL_ENDOFNAMES(clt, vect[2]),clt.getfd());
	}
	else if (serv.channel_exist(vect[2]) == 0)
		serv.store_msg(ircrep->RPL_ENDOFNAMES(clt, vect[2]),clt.getfd());
	else
	{
		serv.store_msg(ircrep->RPL_NAMREPLY(clt, serv, vect[2]), clt.getfd());
		serv.store_msg(ircrep->RPL_ENDOFNAMES(clt, vect[2]),clt.getfd());
	}
}
