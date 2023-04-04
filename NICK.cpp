#include "Command.hpp"

//integrer le cas chiant du changement de nick une fois enregistre
void Command::NICK(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt)
{
	if (vect.size() != 2) //le code erreur renvoie le nick actuel, quid dans le cas du setup initial ?
	{
		serv.send_msg(ircrep->ERR_NONICKNAMEGIVEN(clt),clt.getfd());
		return;
	}
	std::string ptl_nick = vect[1];
	std::transform(ptl_nick.begin(), ptl_nick.end(), ptl_nick.begin(), ::toupper);
	Server::mapClient::iterator it;
	// for (it = serv.pool_client.begin(); it != serv.pool_client.end(); ++it)
	// {
	// 	if (ptl_nick == it->second->_intern_nick)
	// 	{
	// 		serv.send_msg(ircrep->ERR_NICKNAMEINUSE(vect[1], clt),clt.getfd());
	// 		return;
	// 	}
	// }
	if (serv.check_exist(ptl_nick) == 1)
	{
		serv.send_msg(ircrep->ERR_NICKNAMEINUSE(vect[1], clt),clt.getfd());
		return;
	}
	clt._nickname = vect[1];
	clt._intern_nick = ptl_nick;
	if (clt._identified < 3)
	{
		clt._identified++;
		std::cout<<clt._identified<<std::endl;
		clt.check_registered(serv, *this);
	}
	cmd.empty(); 
	//Ajout du check pour les caracteres interdits
}
