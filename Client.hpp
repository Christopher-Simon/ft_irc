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
	std::string _buffer;
	bool _identified;

public:
	Client();
	Client(Server &serv, int);
	Client(int fd); // createur avec std map en parametre pour surveiller les nickname des autres clients
	~Client();

	class	LostConnExceptions : public std::exception {
		public:
			virtual const char*	what() const throw();
	};
	int getfd();
	bool get_status();
	void get_msg();
	std::string & get_buffer();
};

#endif
