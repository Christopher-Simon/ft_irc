#include "../Command.hpp"

// void Command::WHO(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt)
// {
// 	//don't support the mask
// 	if (clt._identified < 3)
// 		serv.send_msg(ircrep->ERR_NOTREGISTERED(clt),clt.getfd());
// 	else if (vect.size() != 2)
// 		serv.send_msg(ircrep->RPL_ENDOFWHO(clt),clt.getfd());
// 	else if (vect[1][0] == '#' && serv.channel_exist(vect[1]) == 0)
// 		serv.send_msg(ircrep->ERR_NOSUCHCHANNEL(clt, vect[1]),clt.getfd());
// 	else if ()

// }