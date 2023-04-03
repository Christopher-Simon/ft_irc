#include "Client.hpp"

Client::Client()
{
	_fd = -1;
	_identified = 0;
}

Client::Client(Server &serv, int opt)
{
	this->_fd = 0;
	if (makeSocketNonBlocking(0) == -1)
		throw (std::runtime_error("fcntl failed"));
	serv._event.events = EPOLLIN | EPOLLET;
	serv._event.data.fd = 0;
	if (epoll_ctl(serv.get_epollfd(), EPOLL_CTL_ADD, 0, &serv._event))
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

std::string Client::get_msg()
{
	int count;
	char buf[BUFFER_SIZE];
	std::string msg;

	while (1)
	{
		count = recv(_fd, buf, BUFFER_SIZE - 1, 0);
		if (count == -1 || count == 0)
			break;
		buf[count] = '\0';
		msg += buf;
	}
	return (msg);
}

void Client::check_registered(Server &serv, Command &cmd)
{
	if (_identified != 3)
		return;
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

void Client::add_mod(char c)
{
	if (_mods.find(c) != std::string::npos)
		return;
	_mods = _mods + c;
	return;
}

void Client::rem_mod(char c)
{
	if (_mods.find(c) == std::string::npos)
		return;
	_mods.erase(_mods.find(c), 1);
	return;
}
