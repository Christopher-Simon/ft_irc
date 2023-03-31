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
		// std::cout << "count : " << count << std::endl;
		if (count == -1 || count == 0)
			break;
		buf[count] = '\0';
		msg += buf;
	}
	// std::cout << msg;
	return (msg);
}

bool Client::get_status()
{
	return (this->_identified);
}

std::string RPL_WELCOME(Client &clt)
{
	std::string op1 = "<";
	std::string op = "> :Welcome to the Network";
	return (op1 + clt.get_nick() + op);
}

void Client::identify(std::string &msg, Server &serv)
{
	if (msg.substr(0, 6) != "CAP LS")
		return;
	size_t index = msg.find_first_of("\r\n", 0);
	std::cout<<"good 1 -"<<index<<std::endl;
	//while (index < msg.size() && (msg[index] == '\r' ||)
	index++;
	if (index == std::string::npos)
		return;
	//std::cout<<msg.substr(index + 1, 4)<<std::endl;
	if (msg.substr(index + 1, 4) != "NICK")
		return;
	_nickname = msg.substr(index + 6, msg.find_first_of("\r\n", index + 6)- index - 6);
	std::cout<<"NICK = "<<_nickname<<std::endl;
	index = msg.find_first_of("\r\n", index + 6)+ 1;
	if (index == std::string::npos)
		return;
	if (msg.substr(index + 1, 4) != "USER")
		return;
	_user = msg.substr(index + 6, msg.find_first_of(" \r\n", index + 6)- index - 6);
	std::cout<<"USER = "<<_user<<std::endl;

	//serv.send_msg("<client> :Welcome to the <networkname> Network, <nick>[!<user>@<host>]", _fd);
	serv.send_msg(RPL_WELCOME(*this), _fd);
	_identified = 1;
	return;
}

std::string Client::get_nick()
{
	return (this->_nickname);
}
