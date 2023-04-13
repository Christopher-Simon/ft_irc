#include "Command.hpp"

void Command::KICK(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt)
{
	if (vect.size() < 3)
	{
		serv.store_msg(ircrep->ERR_NEEDMOREPARAMS(cmd, clt), clt.getfd());
		std::cerr << RED << "ERR_NEEDMOREPARAMS" << RESET << std::endl;
	}
	else if (!serv.channel_exist(vect[1]))
	{
		serv.store_msg(ircrep->ERR_NOSUCHCHANNEL(clt, vect[1]), clt.getfd());
		std::cerr << RED << "ERR_NOSUCHCHANNEL" << RESET << std::endl;
	}
	else if (!serv.pool_channel[vect[1]]->userInChannel(clt.getfd()))
	{
		serv.store_msg(ircrep->ERR_NOTONCHANNEL(clt, vect[1]), clt.getfd());
		std::cerr << RED << "ERR_NOTONCHANNEL" << RESET << std::endl;
	}
	else if (!serv.user_in_channel(vect[1], vect[2]))
	{
		serv.store_msg(ircrep->ERR_USERNOTINCHANNEL(clt, vect[2], vect[1]), clt.getfd());
		std::cerr << RED << "ERR_USERNOTINCHANNEL" << RESET << std::endl;
	}
	else if (serv.pool_channel[vect[1]]->_members[clt.getfd()].find("o"))
	{
		serv.store_msg(ircrep->ERR_CHANOPRIVSNEEDED(clt, vect[1]), clt.getfd());
		std::cerr << RED << "ERR_CHANOPRIVSNEEDED" << RESET << std::endl;
	}
	else
	{
		std::string kicker = clt.get_nick();
		std::string kicked = vect[2];
		std::string channel = vect[1];
		std::string reason = vect[3];
		std::string reply = ":"+ kicker + " " + "KICK" + " " + channel + " " + kicked + " " + reason;
		serv.store_channel_msg(reply, vect[1], clt.getfd());
		serv.store_msg(reply, clt.getfd());
		std::cout << serv.pool_channel[vect[1]]->_name << std::endl;
		serv.pool_channel[vect[1]]->remove_memb(serv.check_nick_exist(vect[2]), serv);
	}
}
