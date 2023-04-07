#ifndef CLIENT_HPP
# define CLIENT_HPP


#include "Server.hpp"
#include "Command.hpp"
#include "irc.hpp"

class Server;
class Command;

class Client
{
private:
	int _fd;
	std::string _buffer;

public:
	std::string _nickname;
	std::string _intern_nick;
	std::string _username; // is the username of the client on the local machin
	std::string _hotsname; //is the hostname of the client's computer
	std::string _servername; // is the name of the server that the client is connecting to
	std::string _realname;
	std::string _mods;
	int _identified;
	
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
	std::string get_nick();

	void check_registered(Server &, Command &);
	void add_mod(char);
	void rem_mod(char);

//modes
// USER MODES :
// - i : invisible
// - o : operator

	void get_msg();
	std::string & get_buffer();
};

#endif
