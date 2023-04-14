#include "../Command.hpp"

void Command::INVITE(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt)
{
	if (clt._identified < 3)
		serv.store_msg(ircrep->ERR_NOTREGISTERED(clt),clt.getfd());
	else if (vect.size() > 3)
		serv.store_msg(ircrep->ERR_UNKNOWNCOMMAND(cmd, clt),clt.getfd());
	else if (vect.size() < 3)
		serv.store_msg(ircrep->ERR_NEEDMOREPARAMS(cmd, clt),clt.getfd());
	else if (serv.channel_exist(vect[2]) == 0)
		serv.store_msg(ircrep->ERR_NOSUCHCHANNEL(clt, vect[2]),clt.getfd());
	else if (serv.client_in_channel(vect[2], clt) == 0)
		serv.store_msg(ircrep->ERR_NOTONCHANNEL(clt, vect[2]),clt.getfd());
	else if (serv.get_chan_mods(vect[2]).find(C_INVITE) != std::string::npos && serv.get_userinchan_mods(vect[2], clt).find(CU_OPERATOR) == std::string::npos)
		serv.store_msg(ircrep->ERR_CHANOPRIVSNEEDED(clt, vect[2]),clt.getfd());
	else if (serv.check_nick_exist(vect[1]) == 0)
		serv.store_msg(ircrep->ERR_NOSUCHNICK(clt),clt.getfd());
	else if (serv.client_in_channel(vect[2], *serv.pool_client[serv.check_nick_exist(vect[1])]) ==1)
		serv.store_msg(ircrep->ERR_USERONCHANNEL(clt, vect[2]),clt.getfd());
	else
	{
		int fd_receiver = serv.check_nick_exist(vect[1]);
		serv.store_msg(ircrep->RPL_INVITING(clt, vect[2]), clt.getfd());
		if (serv.get_chan(vect[2])->invited_clients.find(fd_receiver) == serv.get_chan(vect[2])->invited_clients.end())
			serv.get_chan(vect[2])->invited_clients[fd_receiver] = serv.pool_client[fd_receiver];
		std::string reply = ":" + clt.get_nick() + "!" + clt._username + "@" + serv.pool_client[fd_receiver]->_servername + " INVITE " + serv.pool_client[fd_receiver]->get_nick() + " " + vect[2];
		serv.store_msg(reply, fd_receiver);
	}
}