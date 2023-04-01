#ifndef CLIENT_HPP
# define CLIENT_HPP


#include "Server.hpp"
#include "irc.hpp"

class Server;

class Client
{
private:
	int _fd;


public:
	std::string _nickname;
	//std::string _intern_nick;
	std::string _username; // is the username of the client on the local machin
	std::string _hotsname; //is the hostname of the client's computer
	std::string _servername; // is the name of the server that the client is connecting to
	std::string _realname;
	bool _identified;
	
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

