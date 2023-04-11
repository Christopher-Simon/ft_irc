#include "../Command.hpp"

void Command::MODE(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt)
{
	if (clt._identified < 3)
		serv.send_msg(ircrep->ERR_NOTREGISTERED(clt),clt.getfd());
	else if (vect.size() < 2)
		serv.send_msg(ircrep->ERR_NEEDMOREPARAMS(cmd, clt),clt.getfd());
	else if (vect.size() > 4)
		serv.send_msg(ircrep->ERR_UNKNOWNCOMMAND(cmd, clt),clt.getfd());
	else if (vect[1][0] == '#')
	{
		std::string target = vect[1];
		if (serv.pool_channel.find(target) == serv.pool_channel.end())
			serv.send_msg(ircrep->ERR_NOSUCHCHANNEL(clt, target),clt.getfd());
		else if (vect.size() == 2)
			serv.send_msg(ircrep->RPL_CHANNELMODEIS(clt, target, serv.get_chan(target)->_channel_mods),clt.getfd());
		else if (serv.get_chan(target)->_members.find(clt.getfd())->second.find('o', 0) == std::string::npos)
			serv.send_msg(ircrep->ERR_CHANOPRIVSNEEDED(clt, target),clt.getfd());
		else if (vect.size() == 4)
		{
			std::string ptl_nick2 = vect[1].substr(0, vect[1].find_first_of(" \r\n",0));
			std::transform(ptl_nick2.begin(), ptl_nick2.end(), ptl_nick2.begin(), ::toupper);
			if (vect[3] != "+o" && vect[3] != "-o")
				serv.send_msg(ircrep->ERR_UNKNOWNMODE(clt, target),clt.getfd());
			else if (ptl_nick2 != clt._intern_nick)
				serv.send_msg(ircrep->ERR_USERSDONTMATCH(clt),clt.getfd());
			else
			{
				if (vect[3][0] == '+')
					serv.get_chan(target)->_members.find(clt.getfd())->second = "o";
				else if (vect[3][0] == '-')
					serv.get_chan(target)->_members.find(clt.getfd())->second = "";
			}
		}
		else if (vect.size() == 3)
		{
			if (vect[2] != "+i" && vect[2] != "-i")
				serv.send_msg(ircrep->ERR_UNKNOWNMODE(clt, target),clt.getfd());
			else
			{
				if (vect[2][0] == '+')
					serv.get_chan(target)->_members.find(clt.getfd())->second = "i";
				else if (vect[2][0] == '-')
					serv.get_chan(target)->_members.find(clt.getfd())->second = "";
				serv.send_msg(ircrep->RPL_CHANNELMODEIS(clt, target, serv.get_chan(target)->_channel_mods),clt.getfd());
			}
		}
		return;
	}
	else
	{
		std::string ptl_nick = vect[1].substr(0, vect[1].find_first_of(" \r\n",0));
		std::transform(ptl_nick.begin(), ptl_nick.end(), ptl_nick.begin(), ::toupper);
		std::string mods = "io";
		if (serv.check_nick_exist(ptl_nick)== 0)
			serv.send_msg(ircrep->ERR_NOSUCHNICK(clt),clt.getfd());
		else if (ptl_nick != clt._intern_nick)
			serv.send_msg(ircrep->ERR_USERSDONTMATCH(clt),clt.getfd());
		else if (vect.size() == 2)
			serv.send_msg(ircrep->RPL_UMODEIS(clt), clt.getfd());
		else if (vect[2][0] == '+' || vect[2][0] == '-')
		{
			if (vect[2][0] == '+')
				clt.add_mod(vect[2].substr(1, vect[2].size()));
			else if (vect[2][0] == '-')
				clt.rem_mod(vect[2].substr(1, vect[2].size()));
			serv.send_msg(ircrep->RPL_UMODEIS(clt), clt.getfd());
		}
		else
			serv.send_msg(ircrep->ERR_UNKNOWNMODE(clt, vect[2]),clt.getfd());
	}
	cmd.empty();
	return;
}