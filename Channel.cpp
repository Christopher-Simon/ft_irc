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
	_iskey = 0;
	nb_memb = 1;
	_topic = "";
}

Channel::Channel(std::string name, std::string key)
{
	_name = name;
	_key = key;
	_iskey = 1;
	nb_memb = 1;
	_channel_mods = "k";
	_topic = "";
}

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

void Channel::add_member(int fd)
{
	if (_members.find(fd) != _members.end())
		return;
	_members[fd] = "";
	nb_memb++;
}