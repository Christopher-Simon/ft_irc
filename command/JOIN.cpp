#include "Command.hpp"

//ajout du droit de channel operator quand le client cree un channel qui existait pas
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
	if (vect.size() == 3)
	{
		std::vector<std::string> list_key = ft_split(vect[2], ',');
		// for(size_t i = 0; i < list_channel.size(); i++)
		// {

		// }
		//TBC
		return;
	}
	//gerer le cas ou les noms sont les memes avec des majuscules
	//faire gaffe a ce que les messages d'erreur ne bloquent pas l'execution du reste de la liste
	for(size_t i = 0; i < list_channel.size(); i++)
	{
		if (serv.channel_exist(list_channel[i]) == 1) //Channel trouve
		{
			if (serv.client_in_channel(list_channel[i], clt) == 1) //client deja present dans le channel
				return;
			serv.pool_channel.find(list_channel[i])->second->_members[clt.getfd()] = "i";
			serv.pool_channel.find(list_channel[i])->second->nb_memb++;
			std::cout<<clt.get_nick()<<" added to channel "<<list_channel[i]<<std::endl;
		}
		else //channel pas existante
		{
			if (list_channel[i][0] != '#')
			{
				serv.send_msg(ircrep->ERR_INVALIDCHANNELNAME(clt, list_channel[i]),clt.getfd());
				return;
			}
			//AJOUTER le mod du channel
			serv.pool_channel[list_channel[i]] = new Channel(list_channel[i]);
			serv.pool_channel.find(list_channel[i])->second->_members[clt.getfd()] = "o";
			std::cout<<clt.get_nick()<<" added to created channel "<<list_channel[i]<<std::endl;
		}
	}
	return;
}
