#include "../Command.hpp"

void Command::TOPIC(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt)
{
	(void)cmd;
	if (clt._identified < 3)
		serv.store_msg(ircrep->ERR_NOTREGISTERED(clt),clt.getfd());
	else if (vect.size() < 2)
		serv.store_msg(ircrep->ERR_NEEDMOREPARAMS(cmd, clt),clt.getfd());
	else if (vect.size() > 3)
		serv.store_msg(ircrep->ERR_UNKNOWNCOMMAND(cmd, clt),clt.getfd());
	else if (vect.size() == 2)
	{
		if (serv.channel_exist(vect[1]) == 0)
			serv.store_msg(ircrep->ERR_NOSUCHCHANNEL(clt, vect[1]), clt.getfd());
		else if (serv.get_chan(vect[1])->_topic == "")
			serv.store_msg(ircrep->RPL_NOTOPIC(clt, vect[1]), clt.getfd());
		else
			serv.store_msg(ircrep->RPL_TOPIC(clt, vect[1], serv.get_chan(vect[1])->_topic), clt.getfd());
	}
	else
	{
		if (serv.channel_exist(vect[1]) == 0)
			serv.store_msg(ircrep->ERR_NOSUCHCHANNEL(clt, vect[1]), clt.getfd());
		else if (vect[2] == ":")
		{
			serv.get_chan(vect[1])->_topic = "";
			serv.store_channel_msg(ircrep->RPL_NOTOPIC(clt, vect[1]), vect[1], 0);
		}
		else
		{
			serv.get_chan(vect[1])->_topic = vect[2].substr(1, vect[2].length());
			serv.store_channel_msg(ircrep->RPL_TOPIC(clt, vect[1], serv.get_chan(vect[1])->_topic), vect[1], 0);
		}
	}
}