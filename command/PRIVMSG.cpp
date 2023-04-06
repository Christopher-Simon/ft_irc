#include "Command.hpp"

void Command::PRIVMSG(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt)
{
	(void)cmd;
	if (clt._identified < 3)
	{
		serv.send_msg(ircrep->ERR_NOTREGISTERED(clt),clt.getfd());
		return;
	}
	if (vect.size() > 3)
	{
		serv.send_msg(ircrep->ERR_TOOMANYTARGETS(clt, vect[1]), clt.getfd());
		return ;
	}
	if (vect[2].size() == 1)
	{
		serv.send_msg(ircrep->ERR_NOTEXTTOSEND(clt), clt.getfd());
		return ;
	}
	// std::cout << vect.size();
	// print_vect(vect);
	std::string reply = "PRIVMSG ";
	std::string space = " ";
	std::string identifier = ":" + clt._nickname + "!" + clt._username + "@" + clt._hotsname;
	// std::string sender = ":" + clt._servername + space + reply + space + "#moi" + space + vect[2] + "\r\n";
	std::string receivers = identifier + space + reply + vect[1] + space + vect[2];
 	// std::cout << "sender : " << sender << std::endl;
 	std::cout << "receivers : " << receivers <<std::endl;
	// serv.send_msg(sender, clt.getfd());
	if (vect[1][0] == '#')
	{
		if (!serv.channel_exist(vect[1]))
		{
			std::cout << RED << "channel doesn't exists" << RESET << std::endl;
			serv.send_msg(ircrep->ERR_NOSUCHCHANNEL(clt, vect[1]), clt.getfd());
		}
		else
		{
			std::cout << GREEN << "channel exists" << RESET << std::endl;
			serv.send_channel_msg(receivers, vect[1], clt.getfd());
		}
	}
	else
	{
		int fd = serv.check_nick_exist(vect[1]);
		if (fd == 0)
			serv.send_msg(ircrep->ERR_NOSUCHNICK(clt), clt.getfd());
		else
			serv.send_msg(receivers, fd);
	}
}
