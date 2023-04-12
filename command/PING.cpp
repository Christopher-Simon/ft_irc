#include "Command.hpp"

void Command::PING(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt)
{
	if (clt._identified < 3)
		serv.send_msg(ircrep->ERR_NOTREGISTERED(clt),clt.getfd());
	else if (vect.size() != 2)
		serv.send_msg(ircrep->ERR_NOORIGIN(clt),clt.getfd());
	else
	{
		std::string reply = ":" + clt.get_nick() + "!" + clt._username + "@" + clt._servername + " PONG :" + vect[1];
		serv.send_msg(reply, clt.getfd());
	}
	cmd.empty();
}
