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
		std::string kill_msg = "KILL from " + clt.get_nick() + "!" + clt._username + "@" + clt._hotsname + " (" + vect[2] + ")";
		serv.send_msg(kill_msg, serv.check_nick_exist(vect[1]));

	}
}