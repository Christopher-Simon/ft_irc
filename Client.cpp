#include "Client.hpp"

Client::Client()
{
	_fd = -1;
	_identified = 0;
}

Client::Client(Server &serv, int opt):_buffer()
{
	this->_fd = 0;
	//countdown_unregister = 0;
	if (makeSocketNonBlocking(0) == -1)
		throw (std::runtime_error("fcntl failed"));
	serv._event.events = EPOLLIN | EPOLLET;
	serv._event.data.fd = 0;
	if (epoll_ctl(serv.get_epollfd(), EPOLL_CTL_ADD, 0, &serv._event)==-1)
		throw (std::runtime_error("epoll fail"));
	_identified = opt;
}

Client::Client(int fd): _fd(fd), _identified(0)
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
	_buffer += buf;
	if (count == 0)
		throw(LostConnExceptions());
}

void Client::check_registered(Server &serv, Command &cmd)
{
	if (_identified != 3)
		return;
	int nb_reg = 0;
	for (size_t i = 0; i < serv.pool_client.size(); i++)
	{
		if (serv.pool_client[i]->_identified >= 3)
			nb_reg++;
	}
	if (nb_reg > 20)
		return;
	//countdown_unregister = 0;
	serv.send_msg(cmd.ircrep->RPL_WELCOME(*this), _fd);
	serv.send_msg(cmd.ircrep->RPL_YOURHOST(*this), _fd);
	serv.send_msg(cmd.ircrep->RPL_CREATED(*this), _fd);
	serv.send_msg(cmd.ircrep->RPL_MYINFO(*this), _fd);
	serv.send_msg(cmd.ircrep->RPL_ISUPPORT(*this), _fd);
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

const char* Client::LostConnExceptions::what() const throw()
{
	return("Connection with client lost");
}

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
