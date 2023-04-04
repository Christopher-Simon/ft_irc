#include "Command.hpp"

void Command::PASS(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt)
{
	//cas ou le mdp possede des espaces -> verifier la validite dans la doc
	if (clt._identified == 3)
	{
		serv.send_msg(ircrep->ERR_ALREADYREGISTERED(clt),clt.getfd());
		return;
	}
	if (vect.size() != 2)
	{
		serv.send_msg(ircrep->ERR_NEEDMOREPARAMS(cmd, clt),clt.getfd());
		return;
	}
	if (vect[1] != serv.password)
	{
		serv.send_msg(ircrep->ERR_PASSWDMISMATCH(clt),clt.getfd());
		return;
	}
	clt._identified++;
	std::cout<<clt._identified<<std::endl;
	clt.check_registered(serv, *this);
}
