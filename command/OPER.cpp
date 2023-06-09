#include "../Command.hpp"

void Command::OPER(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt)
{
	(void)cmd;
	if (clt._identified < 3)
		serv.store_msg(ircrep->ERR_NOTREGISTERED(clt),clt.getfd());
	else if (vect.size() < 3)
		serv.store_msg(ircrep->ERR_NEEDMOREPARAMS(cmd, clt),clt.getfd());
	else if (vect.size() > 3)
		serv.store_msg(ircrep->ERR_UNKNOWNCOMMAND(cmd, clt),clt.getfd());
	else if (vect[2] != "admin")
		serv.store_msg(ircrep->ERR_PASSWDMISMATCH(clt),clt.getfd());
	else if (serv.check_nick_exist(vect[1]) == 0)
		serv.store_msg(ircrep->ERR_NOOPERHOST(clt), clt.getfd());
	else
	{
		if (serv.pool_client[serv.check_nick_exist(vect[1])]->_mods.find('o') == std::string::npos)
			serv.pool_client[serv.check_nick_exist(vect[1])]->add_mod("trueop", serv, *this);
		serv.store_msg(ircrep->RPL_YOUREOPER(*serv.pool_client[serv.check_nick_exist(vect[1])]), serv.check_nick_exist(vect[1]));
		serv.store_msg(ircrep->RPL_UMODEIS(*serv.pool_client[serv.check_nick_exist(vect[1])]), serv.check_nick_exist(vect[1]));
	}
}