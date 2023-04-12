#include "Channel.hpp"

Channel::Channel()
{

}

Channel::~Channel()
{
	
}

Channel::Channel(std::string name)
{
	_name = name;
	//_iskey = 0;
	nb_memb = 1;
	_topic = "";
}

// Channel::Channel(std::string name, std::string key)
// {
// 	_name = name;
// 	//_key = key;
// 	//_iskey = 1;
// 	nb_memb = 1;
// 	_channel_mods = "k";
// 	_topic = "";
// }

void Channel::remove_memb(int fd, Server &serv)
{
	_members.erase(fd);
	nb_memb--;
	if (nb_memb == 0)
		serv.del_channel(*this);
}

void Channel::add_mod(char c)
{
	if (_channel_mods.find(c) != std::string::npos)
		return;
	_channel_mods = _channel_mods + c;
	return;
}

void Channel::rem_mod(char c)
{
	if (_channel_mods.find(c) == std::string::npos)
		return;
	_channel_mods.erase(_channel_mods.find(c), 1);
	return; 
}

void Channel::add_member(int fd, Server &serv, Command &cmd)
{
	// if (_members.find(fd) != _members.end())
	// 	return;
	if (serv.pool_client[fd]->get_his_channels(serv).size() >= 10)
	{
		serv.store_msg(cmd.ircrep->ERR_TOOMANYCHANNELS(*serv.pool_client[fd], _name), fd);
		return;
	}
	_members[fd] = "";
	nb_memb++;
}

int Channel::userInChannel(int fd)
{
	for (std::map<int, std::string>::iterator it = _members.begin();
		it != _members.end();
		++it
	)
	{
		if (it->first == fd)
			return (1);
	}
	return (0);
}

