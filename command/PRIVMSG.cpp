#include "Command.hpp"

void Command::PRIVMSG(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt)
{
	(void)cmd;
	if (clt._identified < 3)
		serv.store_msg(ircrep->ERR_NOTREGISTERED(clt),clt.getfd());
	else if (vect.size() > 3)
		serv.store_msg(ircrep->ERR_TOOMANYTARGETS(clt, vect[1]), clt.getfd());
	else if (vect.size() < 3)
		serv.store_msg(ircrep->ERR_NEEDMOREPARAMS(cmd, clt),clt.getfd());
	else if (vect[2].size() == 1)
		serv.store_msg(ircrep->ERR_NOTEXTTOSEND(clt), clt.getfd());
	else
	{
		std::vector<std::string> list_target = ft_split(vect[1], ',');
		for (size_t i = 0; i < list_target.size(); i++)
		{
			std::string identifier = ":" + clt._nickname + "!" + clt._username + "@" + clt._hotsname;
			std::string receivers = identifier + " PRIVMSG " + list_target[i] + " "+ vect[2];
			if (list_target[i][0] == '#')
			{
				if (!serv.channel_exist(list_target[i]))
					serv.store_msg(ircrep->ERR_NOSUCHCHANNEL(clt, list_target[i]), clt.getfd());
				else if (!serv.client_in_channel(list_target[i], clt))
					serv.store_msg(ircrep->ERR_CANNOTSENDTOCHAN(clt, list_target[i]), clt.getfd());
				else
					serv.store_channel_msg(receivers, list_target[i], clt.getfd());
			}
			else if (list_target[i] == "BOT")
			{
				serv.jo->handler(clt, serv, vect[2]);
			}
			else
			{
				int fd = serv.check_nick_exist(list_target[i]);
				if (fd == 0)
					serv.store_msg(ircrep->ERR_NOSUCHNICK(clt), clt.getfd());
				else
					serv.store_msg(receivers, fd);
			}
		}
	}
}
