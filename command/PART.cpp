#include "Command.hpp"

void Command::PART(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt)
{
	if (clt._identified < 3)
		serv.store_msg(ircrep->ERR_NOTREGISTERED(clt),clt.getfd());
	else if (vect.size() == 1)
		serv.store_msg(ircrep->ERR_NEEDMOREPARAMS(cmd, clt),clt.getfd());
	else if (vect.size() > 3)
		serv.store_msg(ircrep->ERR_UNKNOWNCOMMAND(cmd, clt),clt.getfd());
	else
	{
		std::vector<std::string> list_channel = ft_split(vect[1], ',');
		for(size_t i = 0; i < list_channel.size(); i++)
		{
			if (serv.pool_channel.find(list_channel[i]) == serv.pool_channel.end())
				serv.store_msg(ircrep->ERR_NOSUCHCHANNEL(clt, list_channel[i]),clt.getfd());
			else if (serv.pool_channel.find(list_channel[i])->second->_members.find(clt.getfd()) == serv.pool_channel.find(list_channel[i])->second->_members.end()) //client pas dans le channel
				serv.store_msg(ircrep->ERR_NOTONCHANNEL(clt, list_channel[i]),clt.getfd());
			else
			{
				serv.pool_channel.find(list_channel[i])->second->remove_memb(clt.getfd(), serv);
				serv.store_msg(":" + clt.get_nick() + "!" + clt._username + "@" + clt._hotsname + " PART " + list_channel[i], clt.getfd());
			}
		}
	}
}