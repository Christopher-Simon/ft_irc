#include "../Command.hpp"

void Command::QUIT(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt)
{
	std::vector<Channel *> list_channel = clt.get_his_channels(serv);
	std::string reply;
	if (vect.size() == 2)
		reply = ":" + clt.get_nick() + "!" + clt._username + "@" + clt._hotsname + " QUIT :QUIT " + vect[1];
	else
		reply = ":" + clt.get_nick() + "!" + clt._username + "@" + clt._hotsname + " QUIT :QUIT :Client Quit";
	for (size_t i = 0; i < list_channel.size(); i++)
	{
		serv.store_channel_msg(reply, list_channel[i]->_name, 0);
	}
	clt._todel = 1;
	cmd.empty();
}