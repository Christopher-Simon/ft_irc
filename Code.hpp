#ifndef CODE_HPP
# define CODE_HPP

#include "Server.hpp"
#include "Client.hpp"

class Server;
class Client;

class Code
{
public:
	Code();
	~Code();

	std::string Base(Client &clt, std::string nb);

	std::string RPL_WELCOME(Client &clt);
	std::string RPL_YOURHOST(Client &clt);
	std::string RPL_CREATED(Client &clt);
	std::string RPL_MYINFO(Client &clt);
	std::string RPL_ISUPPORT(Client &clt);

	std::string ERR_NEEDMOREPARAMS(std::string, Client &);
};

#endif