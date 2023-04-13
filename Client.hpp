#ifndef CLIENT_HPP
# define CLIENT_HPP


#include "Server.hpp"
#include "Command.hpp"
#include "irc.hpp"
#include "Channel.hpp"

class Server;
class Command;
class Channel;

class Client
{
private:
	int _fd;
	std::string _buffer;

public:
	std::string _nickname;
	std::string _intern_nick;
	std::string _username; // is the username of the client on the local machin
	std::string _hotsname; // is the hostname of the client's computer
	std::string _servername; // is the name of the server that the client is connecting to
	std::string _realname;
	std::string _mods;

	bool		epollout;
	int _user_ok;
	int _nick_ok;
	int _pass_ok;
	int _identified;
	int _todel;
	//int countdown_unregister;
	
	Client();
	Client(int fd); // createur avec std map en parametre pour surveiller les nickname des autres clients
	~Client();

	class	LostConnExceptions : public std::exception {
		public:
			virtual const char*	what() const throw();
	};
	int getfd();
	bool get_status();
	std::string get_nick();
	std::vector<Channel *> get_his_channels(Server &serv);

	void check_registered(Server &, Command &);
	void add_mod(std::string);
	void rem_mod(std::string);

//modes
// USER MODES :
// - i : invisible
// - o : operator

	void get_msg();
	std::string & get_buffer();
	void	set_buffer(std::string & new_buffer);
};

#endif
