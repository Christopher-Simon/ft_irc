#ifndef BOT_HPP
#define BOT_HPP

#include "Client.hpp"
#include "Server.hpp"

class Client;

class Bot
{
public :
	Bot();
	~Bot();

	//std::map<std::string, int [7][7]> save;
	std::string welcome_msg;
};

#endif