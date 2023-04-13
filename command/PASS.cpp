#include "Command.hpp"

void Command::PASS(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt)
{
	if (clt._identified == 3)
		serv.send_msg(ircrep->ERR_ALREADYREGISTERED(clt),clt.getfd());
	else if (clt._identified < 3)
	{
		if (clt._pass_ok == 1)
			serv.send_msg(ircrep->ERR_ALREADYREGISTERED(clt),clt.getfd());
		else if (vect[1] != serv.password)
			serv.send_msg(ircrep->ERR_PASSWDMISMATCH(clt),clt.getfd());
		else
		{
			clt._pass_ok = 1;
			clt.check_registered(serv, *this);
		}
	}
	else if (vect.size() != 2)
		serv.send_msg(ircrep->ERR_NEEDMOREPARAMS(cmd, clt),clt.getfd());
	else if (vect[1] != serv.password)
		serv.send_msg(ircrep->ERR_PASSWDMISMATCH(clt),clt.getfd());
	else if (clt._identified == 0)
	{
		clt._pass_ok = 1;
		clt.check_registered(serv, *this);
	}
}
