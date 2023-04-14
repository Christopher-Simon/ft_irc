#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "Client.hpp"
#include "Server.hpp"
#include "Command.hpp"

class Client;
class Server;
class Command;

class Channel
{
public:
	Channel();
	Channel(std::string);
	~Channel();

	std::map<int, std::string> _members;
	std::string _channel_mods;
	unsigned int nb_memb;
	std::string _name;
	std::string _topic;
	std::map<int, Client *> invited_clients;

	void remove_memb(int fd, Server &serv);
	int userInChannel(int fd);
	void add_mod(char);
	void rem_mod(char);

	void add_member(int fd, Server &serv, Command &cmd);

};

#endif

//modes
// for users :
//     - o : channel operator
// for channels :
//     - i : invite only GARDER