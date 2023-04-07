#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "Client.hpp"
#include "Server.hpp"

class Client;
class Server;

class Channel
{
public:
	Channel();
	Channel(std::string);
	Channel(std::string, std::string);
	~Channel();

	std::map<int, std::string> _members; //la string detaille les mods des utilisateurs dans le channel
	std::string _channel_mods;
	unsigned int nb_memb;
	std::string _name;
	std::string _topic;
	bool _iskey;
	std::string _key;

	void remove_memb(int fd, Server &serv);
	void add_mod(char);
	void rem_mod(char);

	void add_member(int fd);

};

#endif

//modes
// for users :
//     - o : channel operator
// for channels :
//     - i : invite only GARDER
//     - k : key locked