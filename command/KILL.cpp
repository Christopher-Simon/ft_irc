#include "../Command.hpp"


//kill sans comment ?
void Command::KILL(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt)
{
	(void)cmd;
	if (clt._identified < 3)
		serv.send_msg(ircrep->ERR_NOTREGISTERED(clt),clt.getfd());
	else if (clt._mods.find('o') == std::string::npos)
		serv.send_msg(ircrep->ERR_NOPRIVILEGES(clt),clt.getfd());
	else if (vect.size() < 3)
		serv.send_msg(ircrep->ERR_NEEDMOREPARAMS(cmd, clt),clt.getfd());
	else if (vect.size() > 3)
		serv.send_msg(ircrep->ERR_UNKNOWNCOMMAND(cmd, clt),clt.getfd());
	else if (serv.check_nick_exist(vect[1]) == 0)
		serv.send_msg(ircrep->ERR_NOSUCHNICK(clt),clt.getfd());
	else
	{
		std::string kill_msg = "KILL from " + clt.get_nick() + "!" + clt._username + "@" + clt._hotsname;
		serv.send_msg(kill_msg, serv.check_nick_exist(vect[1]));
		//sending quit notifications
		Client *target = serv.pool_client[serv.check_nick_exist(vect[1])];
		std::vector<Channel *> list_channel = target->get_his_channels(serv);
		std::string quit_msg  = ":" + target->get_nick() + "!" + target->_username + "@" + target->_hotsname + " QUIT :Killed (" + clt.get_nick() +" (" + vect[2].substr(1, vect[2].size()) + "))";
		for (size_t i = 0; i < list_channel.size(); i++)
		{
			serv.send_channel_msg(quit_msg, list_channel[i]->_name, 0);
		}
		// modif du message erreur
		std::string error_msg = "ERROR :Closing Link:" + target->_hotsname;
		serv.send_msg(error_msg, target->getfd());
		target->_todel = 1;
	}
}