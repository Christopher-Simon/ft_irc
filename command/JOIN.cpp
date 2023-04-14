#include "Command.hpp"

void Command::JOIN(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt)
{
	if (clt._identified < 3)
	{
		serv.store_msg(ircrep->ERR_NOTREGISTERED(clt),clt.getfd());
		return;
	}
	if (vect.size() == 1)
	{
		serv.store_msg(ircrep->ERR_NEEDMOREPARAMS(cmd, clt),clt.getfd());
		return;
	}
	std::vector<std::string> list_channel = ft_split(vect[1], ',');
	if (vect.size() >= 3)
	{
		serv.store_msg(ircrep->ERR_UNKNOWNCOMMAND(cmd, clt),clt.getfd());
		return;
	}
	for(size_t i = 0; i < list_channel.size(); i++)
	{
		if (serv.channel_exist(list_channel[i]) == 1 && serv.chan_has_mod(list_channel[i], 'i') == 0) //Channel trouve
		{
			std::string reply = ":" + clt.get_nick() + "!" + clt._username + "@" + clt._hotsname + " JOIN :";
			serv.get_chan(list_channel[i])->add_member(clt.getfd(), serv, *this);
			serv.store_msg(reply + list_channel[i], clt.getfd());
			if (serv.get_chan(list_channel[i])->_topic != "")
				serv.store_msg(ircrep->RPL_TOPIC(clt, list_channel[i], serv.get_chan(list_channel[i])->_topic), clt.getfd());
			serv.store_msg(ircrep->RPL_NAMREPLY(clt, serv, list_channel[i]), clt.getfd());
			serv.store_msg(ircrep->RPL_ENDOFNAMES(clt, list_channel[i]),clt.getfd());
		}
		else if (serv.channel_exist(list_channel[i]) == 1 && serv.chan_has_mod(list_channel[i], 'i')==1)
		{
			if (serv.get_chan(list_channel[i])->invited_clients.find(clt.getfd()) == serv.get_chan(list_channel[i])->invited_clients.end())
				serv.store_msg(ircrep->ERR_INVITEONLYCHAN(clt, list_channel[i]),clt.getfd());
			else
			{
				serv.get_chan(list_channel[i])->add_member(clt.getfd(), serv, *this);
				std::string reply = ":" + clt.get_nick() + "!" + clt._username + "@" + clt._hotsname + " JOIN :";
				serv.store_msg(reply + list_channel[i], clt.getfd());
				if (serv.get_chan(list_channel[i])->_topic != "")
					serv.store_msg(ircrep->RPL_TOPIC(clt, list_channel[i], serv.get_chan(list_channel[i])->_topic), clt.getfd());
				serv.store_msg(ircrep->RPL_NAMREPLY(clt, serv, list_channel[i]), clt.getfd());
				serv.store_msg(ircrep->RPL_ENDOFNAMES(clt, list_channel[i]),clt.getfd());
				
			}
		}
		else //channel pas existante
		{
			if (list_channel[i][0] != '#' || list_channel[i].find_first_of(" \a,") != std::string::npos)
				serv.store_msg(ircrep->ERR_INVALIDCHANNELNAME(clt, list_channel[i]),clt.getfd());
			else if (serv.pool_client[clt.getfd()]->get_his_channels(serv).size() >= 10)
				serv.store_msg(ircrep->ERR_TOOMANYCHANNELS(*serv.pool_client[clt.getfd()], list_channel[i]), clt.getfd());
			else
			{
				serv.pool_channel[serv.toupper(list_channel[i])] = new Channel(serv.toupper(list_channel[i]),list_channel[i]);
				serv.pool_channel.find(serv.toupper(list_channel[i]))->second->_members[clt.getfd()] = "o";
				std::string reply = ":" + clt.get_nick() + "!" + clt._username + "@" + clt._hotsname + " JOIN :";
				serv.store_msg(reply + list_channel[i], clt.getfd());
				if (serv.get_chan(list_channel[i])->_topic != "")
					serv.store_msg(ircrep->RPL_TOPIC(clt, list_channel[i], serv.get_chan(list_channel[i])->_topic), clt.getfd());
				serv.store_msg(ircrep->RPL_NAMREPLY(clt, serv, list_channel[i]), clt.getfd());
				serv.store_msg(ircrep->RPL_ENDOFNAMES(clt, list_channel[i]),clt.getfd());
			}
		}
	}
}
