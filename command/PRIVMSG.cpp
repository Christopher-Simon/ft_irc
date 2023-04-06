#include "Command.hpp"

void Command::PRIVMSG(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt)
{
	(void)cmd;
	if (clt._identified < 3)
	{
		serv.send_msg(ircrep->ERR_NOTREGISTERED(clt),clt.getfd());
		return;
	}
	// std::cout << vect.size();
	print_vect(vect);
	std::string reply = "PRIVMSG ";
	std::string space = " ";
	std::string identifier = ":" + clt._nickname + "!" + clt._username + "@" + clt._hotsname;
	// std::string sender = ":" + clt._servername + space + reply + space + "#moi" + space + vect[2] + "\r\n";
	std::string receivers = identifier + space + reply + vect[1] + space + vect[2] + "\r\n";
 	// std::cout << "sender : " << sender << std::endl;
 	std::cout << "receivers : " << receivers <<std::endl;
	// serv.send_msg(sender, clt.getfd());
	serv.send_all_msg(receivers, clt.getfd());
}
