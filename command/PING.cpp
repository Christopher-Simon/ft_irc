#include "Command.hpp"

//A REVOIR COMPLETEMENT
void Command::PING(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt)
{
	if (clt._identified < 3)
	{
		serv.send_msg(ircrep->ERR_NOTREGISTERED(clt),clt.getfd());
		return;
	}
	if (vect.size() != 2) //a tester
		serv.send_msg(ircrep->ERR_NEEDMOREPARAMS(cmd, clt),clt.getfd()); //ajouter l'erreur no origin ?
	//std::string arg = msg.substr(cmd.size()+1, msg.size());
	// if (arg != clt._servername)
	// 	serv.send_msg
	std::string reply = "PONG ";
	std::string space = " :";
	//std::cout<<reply + clt._servername + space + vect[1]<<std::endl;
	serv.send_msg(reply + clt._servername + space + vect[1], clt.getfd());
	//std::cout<<cmd<<serv.get_sockfd()<<std::endl;
}
