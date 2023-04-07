#include "Command.hpp"

//Ajouter l'option reason 
void Command::PART(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt)
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
	for(size_t i = 0; i < list_channel.size(); i++)
	{
		if (serv.pool_channel.find(list_channel[i]) == serv.pool_channel.end())
			serv.send_msg(ircrep->ERR_NOSUCHCHANNEL(clt, list_channel[i]),clt.getfd());
		else if (serv.pool_channel.find(list_channel[i])->second->_members.find(clt.getfd()) == serv.pool_channel.find(list_channel[i])->second->_members.end()) //client pas dans le channel
			serv.send_msg(ircrep->ERR_NOTONCHANNEL(clt, list_channel[i]),clt.getfd());
		else
		{
			std::cout<<"Temp : leaving channel"<<std::endl;
			// serv.pool_channel.find(list_channel[i])->second->_members.erase(&clt);
			// serv.pool_channel.find(list_channel[i])->second->nb_memb--;
			serv.pool_channel.find(list_channel[i])->second->remove_memb(clt.getfd(), serv);
			std::string reply = ":";
			std::string r2 = "!";
			std::string r3 = "@";
			std::string r5 = " PART ";
			std::cout<<reply + clt.get_nick() + r2 + clt.get_nick() + r3 + clt._servername + r5 + list_channel[i]<<std::endl;
			serv.send_msg(reply + clt.get_nick() + r2 + clt.get_nick() + r3 + clt._servername + r5 + list_channel[i], clt.getfd());
		}
	}

}