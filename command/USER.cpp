#include "Command.hpp"

void Command::USER(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt)
{
	if (clt._identified == 3)
	{
		serv.store_msg(ircrep->ERR_ALREADYREGISTERED(clt),clt.getfd());
		return;
	}
	if (vect.size() != 5)
	{
		serv.store_msg(ircrep->ERR_NEEDMOREPARAMS(cmd, clt),clt.getfd());
		return;
	}
	clt._username = vect[1];
	clt._hotsname = vect[2];
	clt._servername = vect[3];
	clt._realname = vect[4].substr(1, vect[4].length());
	if (clt._user_ok == 0)
		clt._user_ok = 1;
	clt.check_registered(serv, *this);
	cmd.empty();
}