#include "../Command.hpp"

// void Command::NAMES(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt)
// {
// 	//not dealing with a list of channels for now
// 	if (clt._identified < 3)
// 	{
// 		serv.send_msg(ircrep->ERR_NOTREGISTERED(clt),clt.getfd());
// 		return;
// 	}
// 	else if (vect.size() > 2)
// 		serv.send_msg(ircrep->ERR_UNKNOWNCOMMAND(cmd, clt),clt.getfd());

// }
