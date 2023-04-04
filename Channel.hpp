#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "Client.hpp"

class Client;

class Channel
{
public:
	Channel();
	Channel(std::string);
	Channel(std::string, std::string);
	~Channel();

	std::map<Client *, std::string> _members; //la string detaille les mods des utilisateurs dans le channel
	std::string _channel_mods;
	unsigned int nb_memb;
	std::string _name;
	bool _iskey;
	std::string _key;
};

#endif

//modes
// for users :
//     - O : channel creator
//     - o : channel operator
//     - v : voice
// for channels :
//     - i : invite only
//     - m : moderated
//     - n : outside privmsg disabled
//     - p : private
//     - t : topic locked
//     - k : key locked
//     - l : user limit