#include "Client.hpp"

Client::Client():epollout(false)
{
	_fd = -1;
	_identified = 0;
}

Client::Client(int fd):
_fd(fd),
_buffer(),
_nickname(),
_intern_nick(),
_username(),
_hotsname(),
_servername(),
_realname(),
_mods(),
epollout(false),
_identified(0),
_pass_ok(),
_todel(0)
{
}

Client::~Client()
{
	std::cout << "destroy client fd : " << _fd << std::endl;
	close(_fd);
}

int Client::getfd()
{
	return (this->_fd);
}

void Client::get_msg()
{
	int count;
	char buf[BUFFER_SIZE];

	count = recv(_fd, buf, BUFFER_SIZE - 1, 0);
	if (count < 0)
		throw (std::runtime_error("recv failed"));
	buf[count] = '\0';
	std::cout << "________buffer_________ " << buf << std::endl;
	_buffer += buf;
	if (count == 0)
		throw(LostConnExceptions("Connection with client lost"));
}

void Client::check_registered(Server &serv, Command &cmd)
{
	if (_identified != 3)
		return;
	int nb_reg = 0;
	if (serv.pool_client.size() > 50)
		return;
	std::map<int, Client *>::iterator it;
	for (it = serv.pool_client.begin(); it != serv.pool_client.end(); it++)
	{
		if (it->second->_identified >= 3)
			nb_reg++;
	}
	if (nb_reg > 20)
		return;
	//countdown_unregister = 0;
	serv.store_msg(cmd.ircrep->RPL_WELCOME(*this), _fd);
	serv.store_msg(cmd.ircrep->RPL_YOURHOST(*this), _fd);
	serv.store_msg(cmd.ircrep->RPL_CREATED(*this), _fd);
	serv.store_msg(cmd.ircrep->RPL_MYINFO(*this), _fd);
	serv.store_msg(cmd.ircrep->RPL_ISUPPORT(*this), _fd);
}

bool Client::get_status()
{
	return (this->_identified);
}

std::string Client::get_nick()
{
	return (this->_nickname);
}

void Client::add_mod(std::string c)
{
	if (c == "trueop")
	{
		if (_mods.find('o') == std::string::npos)
			_mods = _mods + 'o';
		return;
	}
	for (size_t i = 0; i < c.size(); i++)
	{
		if (c[i] != 'o' && _mods.find(c[i]) == std::string::npos)
			_mods = _mods + c[i];
	}
	return;
}

void Client::rem_mod(std::string c)
{
	for (size_t i = 0; i < c.size(); i++)
	{
		if (_mods.find(c[i]) != std::string::npos)
			_mods.erase(_mods.find(c[i]), 1);
	}
	return; 
}

std::string & Client::get_buffer() {
	return (_buffer);
}

	void	Client::set_buffer(std::string & new_buffer){
		_buffer = new_buffer;
	}


// const char* Client::LostConnExceptions::what() const throw()
// {
// 	return("Connection with client lost");
// }

std::vector<Channel *> Client::get_his_channels(Server &serv)
{
	std::vector<Channel *> pers_chan;
	std::map<std::string, Channel *>::iterator it;
	for (it = serv.pool_channel.begin(); it != serv.pool_channel.end(); it++)
	{
		if (serv.client_in_channel(it->first, *this) == 1)
			pers_chan.push_back(it->second);
	}
	return (pers_chan);
}
