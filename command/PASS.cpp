#include "Command.hpp"

void Command::PASS(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt)
{
	//cas ou le mdp possede des espaces -> verifier la validite dans la doc
	if (clt._identified == 3)
		serv.send_msg(ircrep->ERR_ALREADYREGISTERED(clt),clt.getfd());
	else if (vect.size() != 2) // comment gerer si plus de parametres
		serv.send_msg(ircrep->ERR_NEEDMOREPARAMS(cmd, clt),clt.getfd());
	else if (vect[1] != serv.password)
		serv.send_msg(ircrep->ERR_PASSWDMISMATCH(clt),clt.getfd());
	else if (clt._identified == 0)
		clt._identified++;
	std::cout << YELLOW <<  "PASS" << clt._identified << RESET << std::endl;
}
