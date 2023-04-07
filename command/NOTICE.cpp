#include "../Command.hpp"

void Command::NOTICE(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt)
{
	(void)cmd;
	if (clt._identified < 3)
		serv.send_msg(ircrep->ERR_NOTREGISTERED(clt),clt.getfd());
	else if (vect.size() < 3)
		serv.send_msg(ircrep->ERR_NEEDMOREPARAMS(cmd, clt),clt.getfd());
	else if (vect.size() > 3)
		serv.send_msg(ircrep->ERR_UNKNOWNCOMMAND(cmd, clt),clt.getfd());
	else
	{
		std::vector<std::string> list_target = ft_split(vect[1], ',');
		std::string identifier = ":" + clt._nickname + "!" + clt._username + "@" + clt._hotsname;
		std::string reply;
		for (size_t i = 0; i < list_target.size(); i++)
		{
			reply = identifier + " NOTICE " + list_target[i] + " " + vect[2];
			if (list_target[i][0] == '#' && serv.channel_exist(list_target[i]))
				serv.send_channel_msg(reply, list_target[i], clt.getfd());
			else if (serv.check_nick_exist(list_target[i]) != 0)
			{
				int fd = serv.check_nick_exist(list_target[i]);
				serv.send_msg(reply, fd);
			}
		}
	}
}
