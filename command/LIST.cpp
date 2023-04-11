#include "../Command.hpp"

void Command::LIST(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt)
{
	if (clt._identified < 3)
	{
		serv.send_msg(ircrep->ERR_NOTREGISTERED(clt),clt.getfd());
		return;
	}
	if (vect.size() == 1)
	{
		serv.send_msg(ircrep->RPL_LISTSTART(clt), clt.getfd());
		Server::mapChannel::iterator it2;
		for (it2 = serv.pool_channel.begin(); it2 != serv.pool_channel.end(); ++it2)
		{
			if (it2->second->_channel_mods.find('i') == std::string::npos)
				serv.send_msg(ircrep->RPL_LIST(clt, it2->second->_name, it2->second->nb_memb, it2->second->_topic), clt.getfd());
		}
		serv.send_msg(ircrep->RPL_LISTEND(clt), clt.getfd());
	}
	cmd.empty();
}