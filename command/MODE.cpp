#include "../Command.hpp"


//Modifier pour traiter le cas ou on veut ajouter plusieurs modes en meme temps
void Command::MODE(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt)
{
	if (clt._identified < 3)
	{
		serv.send_msg(ircrep->ERR_NOTREGISTERED(clt),clt.getfd());
		return;
	}
	if (vect.size() < 2)
	{
		serv.send_msg(ircrep->ERR_NEEDMOREPARAMS(cmd, clt),clt.getfd());
		return;
	}
	if (vect[1][0] == '#')
	{
		std::string target = vect[1];
		if (serv.pool_channel.find(target) == serv.pool_channel.end())
		{
			serv.send_msg(ircrep->ERR_NOSUCHCHANNEL(clt, target),clt.getfd());
			return;
		}
		if (vect.size() == 2)
		{
			serv.send_msg(ircrep->RPL_CHANNELMODEIS(clt, target, serv.pool_channel.find(target)->second->_channel_mods),clt.getfd());
			return;
		}
		if (serv.pool_channel.find(target)->second->_members.find(clt.getfd())->second.find('o', 0) == std::string::npos)
		{
			serv.send_msg(ircrep->ERR_CHANOPRIVSNEEDED(clt, target),clt.getfd());
			return;
		}
		std::string cmods = "ik";
		if (!((vect[2][0] == '+' || vect[2][0] == '-') && vect[2].length() == 2 && cmods.find(vect[2][1]) != std::string::npos))
		{
			return;
		}
		if (vect[2][0] == '+')
			serv.pool_channel.find(target)->second->add_mod(vect[2][1]);
		else if (vect[2][0] == '-')
			serv.pool_channel.find(target)->second->rem_mod(vect[2][1]);
	}
	else
	{
		//question si le mode sort un nickname deja existant mais sans maj par exemple
		std::string ptl_nick = vect[1].substr(0, vect[1].find_first_of(" \r\n",0));
		std::transform(ptl_nick.begin(), ptl_nick.end(), ptl_nick.begin(), ::toupper);
		if (serv.check_nick_exist(ptl_nick)== 0)
		{
			return; //ERR_NOSUCHNICK
		}
		if (ptl_nick != clt._intern_nick)
		{
			//ERR_USERSDONTMATCH
			return;
		}
		if (vect.size() == 2)
		{
			serv.send_msg(ircrep->RPL_UMODEIS(clt), clt.getfd());
			return;
		}
		//aucun flag gere pour le moment
		else if (vect.size() > 3)
		{
			serv.send_msg(ircrep->RPL_UMODEIS(clt), clt.getfd());
			return;
		}
		std::string mods = "airwro";
		if (!((vect[2][0] == '+' || vect[2][0] == '-') && vect[2].length() == 2 && mods.find(vect[2][1]) != std::string::npos))
		{
			return;
		}
		if (vect[2][0] == '+')
			clt.add_mod(vect[2][1]);
		else if (vect[2][0] == '-')
			clt.rem_mod(vect[2][1]);
	}
	cmd.empty();
	return;
}