#include "Command.hpp"

void Command::KICK(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt)
{
	if (vect.size() < 3)
		serv.store_msg(ircrep->ERR_NEEDMOREPARAMS(cmd, clt), clt.getfd());
	else if (!serv.channel_exist(vect[1]))
		serv.store_msg(ircrep->ERR_NOSUCHCHANNEL(clt, vect[1]), clt.getfd());
	else if (!serv.pool_channel[vect[1]]->userInChannel(clt.getfd()))
		serv.store_msg(ircrep->ERR_NOTONCHANNEL(clt, vect[1]), clt.getfd());
	else if (!serv.user_in_channel(vect[1], vect[2]))
		serv.store_msg(ircrep->ERR_USERNOTINCHANNEL(clt, vect[2], vect[1]), clt.getfd());
	else if (serv.pool_channel[vect[1]]->_members[clt.getfd()].find("o"))
		serv.store_msg(ircrep->ERR_CHANOPRIVSNEEDED(clt, vect[1]), clt.getfd());
	else
	{
		std::string prefix = ":" + clt._nickname + "!" + clt._username + "@" + clt._hotsname;
		std::string kicked = vect[2];
		std::string channel = vect[1];
		std::string reason = vect[3];
		std::string reply = prefix +  + " " + "KICK" + " " + channel + " " + kicked + " " + reason;
		serv.store_channel_msg(reply, vect[1], clt.getfd());
		serv.store_msg(reply, clt.getfd());
		std::cout << serv.pool_channel[vect[1]]->_name << std::endl;
		serv.pool_channel[vect[1]]->remove_memb(serv.check_nick_exist(vect[2]), serv);
	}
}
