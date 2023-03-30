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

Client::Client(Server &serv)
{
	this->_fd = accept(serv.get_sockfd(), (struct sockaddr *)&serv._address, (socklen_t *)&serv._addrlen);
	if (this->_fd < 0)
		throw (std::runtime_error("Socket listen failed"));
	else
		std::cout << GREEN << "Client accepted successful : FD = " << _fd<< RESET << std::endl;
	if (makeSocketNonBlocking(_fd) == -1)
		throw (std::runtime_error("fcntl failed"));
	serv._event.events = EPOLLIN | EPOLLET;
	serv._event.data.fd = _fd;
	if (epoll_ctl(serv.get_epollfd(), EPOLL_CTL_ADD, _fd, &serv._event))
		throw (std::runtime_error("epoll fail"));
	_identified = 0;
}

Client::~Client()
{

}

int Client::getfd()
{
	return (this->_fd);
}

std::string Client::get_msg()
{
	int count;
	char buf[512];

	while (1)
	{
		count = read(_fd, buf, sizeof buf);
		if (count == -1 || count == 0)
			break;
	}
	std::string msg = buf;
	//std::cout<<"msg:"<<msg<<std::endl;
	return (msg.substr(0, count));
}

bool Client::get_status()
{
	return (this->_identified);
}