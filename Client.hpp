#ifndef CLIENT_HPP
# define CLIENT_HPP


#include "Server.hpp"
#include "irc.hpp"

class Server;

class Client
{
private:
	int _fd;
	std::string _nickname;
	std::string _intern_nick;
	std::string _user;
	bool _identified;

public:
	Client();
	Client(Server &serv, int);
	Client(int fd); // createur avec std map en parametre pour surveiller les nickname des autres clients
	~Client();

	int getfd();
	bool get_status();
	std::string get_nick();
	void identify(std::string &, Server &);
	std::string get_msg();

};

#endif

