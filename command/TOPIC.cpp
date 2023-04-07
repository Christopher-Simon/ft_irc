#include "Command.hpp"

void Command::TOPIC(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt)
{
	(void)cmd;
	if (clt._identified < 3)
		serv.send_msg(ircrep->ERR_NOTREGISTERED(clt),clt.getfd());
	else if (vect.size() < 2)
		serv.send_msg(ircrep->ERR_NEEDMOREPARAMS(cmd, clt),clt.getfd());
	else if (vect.size() > 3)
		serv.send_msg(ircrep->ERR_UNKNOWNCOMMAND(cmd, clt),clt.getfd());
	else if (vect.size() == 2)
	{
		if (serv.channel_exist(vect[1]) == 0)
			serv.send_msg(ircrep->ERR_NOSUCHCHANNEL(clt, vect[1]), clt.getfd());
		//RPL_TOPIC 
	}
}