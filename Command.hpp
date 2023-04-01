#ifndef COMMAND_HPP
# define COMMAND_HPP

#include "Server.hpp"
#include "Client.hpp"
#include "Code.hpp"

#include <algorithm>

class Server;
class Client;
class Code;

class Command
{
public:
	std::map<std::string, void(Command::*)(std::string cmd, std::string msg, Server &serv, Client &clt)> cmd_repertory;
	Code *ircrep;

	Command();
	~Command();

	void exec(std::string &, Server &, Client &);
	void exec_line(std::string msg, Server &serv, Client &clt);
	void identify(std::string &, Server &, Client &);

	void ping(std::string cmd,std::string msg, Server &serv, Client &clt);
};

#endif