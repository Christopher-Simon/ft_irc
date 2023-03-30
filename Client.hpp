#ifndef CLIENT_HPP
# define CLIENT_HPP


#include "Server.hpp"
#include "irc.hpp"

class Client
{
private:
	int _fd;
	std::string _nickname;
	std::string _user;
	bool _identified;

public:
	Client(); // createur avec std map en parametre pour surveiller les nickname des autres clients
	~Client();

};

#endif

