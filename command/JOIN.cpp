#include "Command.hpp"

//ajout du cas des channel invite
void Command::JOIN(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt)
{
	if (clt._identified < 3)
	{
		serv.send_msg(ircrep->ERR_NOTREGISTERED(clt),clt.getfd());
		return;
	}
	if (vect.size() == 1)
	{
		serv.send_msg(ircrep->ERR_NEEDMOREPARAMS(cmd, clt),clt.getfd());
		return;
	}
	std::vector<std::string> list_channel = ft_split(vect[1], ',');
	if (vect.size() >= 3)
	{
		serv.send_msg(ircrep->ERR_UNKNOWNCOMMAND(cmd, clt),clt.getfd());
		return;
	}
	//gerer le cas ou les noms sont les memes avec des majuscules
	//faire gaffe a ce que les messages d'erreur ne bloquent pas l'execution du reste de la liste
	for(size_t i = 0; i < list_channel.size(); i++)
	{
		if (serv.channel_exist(list_channel[i]) == 1) //Channel trouve
			serv.get_chan(list_channel[i])->add_member(clt.getfd());
		// else if (serv.channel_exist(list_channel[i]) == 1 && serv.get_chan(list_channel[i])->_iskey == 1)
		// {
		// 	serv.send_msg(ircrep->ERR_BADCHANNELKEY(clt, list_channel[i]), clt.getfd());
		// }
		else //channel pas existante
		{
			if (list_channel[i][0] != '#' || list_channel[i].find_first_of(" \a,") != std::string::npos)
			{
				serv.send_msg(ircrep->ERR_INVALIDCHANNELNAME(clt, list_channel[i]),clt.getfd());
			}
			//AJOUTER le mod du channel
			else
			{
				serv.pool_channel[list_channel[i]] = new Channel(list_channel[i]);
				serv.pool_channel.find(list_channel[i])->second->_members[clt.getfd()] = "o";
			}
		}
	}
}
