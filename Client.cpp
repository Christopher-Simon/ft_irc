#include "Client.hpp"

Client::Client()
{
	_fd = -1;
	_identified = 0;
}

Client::Client(Server &serv, int opt):_buffer()
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

void Client::get_msg()
{
	int count;
	char buf[BUFFER_SIZE];

	count = recv(_fd, buf, BUFFER_SIZE - 1, 0);
	if (count < 0)
		throw (std::runtime_error("recv failed"));
	// std::cout << "count : " << count << std::endl;
	buf[count] = '\0';
	_buffer += buf;
	if (count == 0)
		throw(LostConnExceptions());
}

bool Client::get_status()
{
	return (this->_identified);
}

std::string & Client::get_buffer() {
	return (_buffer);
}

const char* Client::LostConnExceptions::what() const throw()
{
	return("Connection with client lost");
}
