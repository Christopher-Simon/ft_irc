#include "Channel.hpp"

Channel::Channel()
{

}

Channel::~Channel()
{
	
}

Channel::Channel(std::string up_name, std::string name)
{
	_name = up_name;
	//_iskey = 0;
	//std::transform(name.begin(), name.end(), name.begin(), ::toupper);
	_intern_name = name;
	nb_memb = 1;
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

void Channel::add_member(int fd, Server &serv, Command &cmd)
{
	if (serv.pool_client[fd]->get_his_channels(serv).size() >= 10)
	{
		serv.store_msg(cmd.ircrep->ERR_TOOMANYCHANNELS(*serv.pool_client[fd], _intern_name), fd);
		return;
	}
	_members[fd] = "";
	nb_memb++;
	std::string reply = ":" + serv.pool_client[fd]->get_nick() + "!" + serv.pool_client[fd]->_username + "@" + serv.pool_client[fd]->_hotsname + " JOIN :";
	std::map<int, std::string>::iterator it;
	for (it = _members.begin(); it != _members.end(); it++)
		serv.store_msg(reply + _intern_name, it->first);
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

