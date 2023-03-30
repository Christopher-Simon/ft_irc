#include "Server.hpp" 

int makeSocketNonBlocking(int sfd)
{
	int flags, s;

	flags = fcntl(sfd, F_GETFL, 0);
	if (flags == -1)
	{
		return -1;
	}
	flags |= O_NONBLOCK;
	s = fcntl (sfd, F_SETFL, flags);
	if (s == -1)
	{
		return -1;
	}
	return 0;
}

Server::Server()
{
	throw (std::invalid_argument("Invalid argument"));
}

Server::Server(char *port)
{
	initSocket(port);
	// int	sockfd;
	// struct sockaddr_in address;
	// int addrlen = sizeof(address);

	// sockfd = socket(AF_INET, SOCK_STREAM, 0);
	// if (sockfd == -1)
	// 	throw (std::runtime_error("Socket creation failed"));
	// else
	// 	std::cout << GREEN << "Socket creation successful" << RESET << std::endl;
	// address.sin_family = AF_INET;
	// address.sin_addr.s_addr = INADDR_ANY;
	// address.sin_port = htons(std::atoi(port));
	// if (makeSocketNonBlocking(sockfd) == -1)
	// 	throw (std::runtime_error("fcntl failed"));
	// std::cout << GREEN << "fcntl on sockfd success" << RESET << std::endl;
	// if (bind(sockfd, (struct sockaddr *)&address, sizeof(address)) < 0)
	// 	throw (std::runtime_error("Socket bind failed"));else
	// std::cout << GREEN << "Socket bind successful" << RESET << std::endl;
	// if (listen(sockfd, 2) < 0)
	// 	throw (std::runtime_error("Socket listen failed"));
	// std::cout << GREEN << "Socket listen successful" << RESET << std::endl;
	// struct epoll_event event;
	// struct epoll_event events[MAX_EVENTS];
	// int epoll_fd = epoll_create1(0);
	// if (epoll_fd == -1)
	// 	throw (std::runtime_error("Failed to create epoll fd"));
	// std::cout<< GREEN << "Success to create epoll fd" << RESET << std::endl;
	// event.events = EPOLLIN;
	// event.data.fd = sockfd;
	// if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sockfd, &event))
	// 	throw (std::runtime_error("epoll fail"));
	// if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, 0, &event))
	// 	throw (std::runtime_error("epoll fail"));
	
}

Server::Server(Server const & raw)
{
	*this = raw;
}


Server::~Server()
{
}
//******************ACCESSORS*****************//


//**************OVERLOAD OPERATORS**************//

Server & Server::operator=(Server const & rhs)
{
	if (this != &rhs) {
	}
	return (*this);
}

//*****************EXCEPTIONS*****************//


//*****************FONCTIONS*****************//

void	Server::initSocket(char *port) {
	_addrlen = sizeof(_address);

	_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (_sockfd == -1)
		throw (std::runtime_error("Socket creation failed"));
	else
		std::cout << GREEN << "Socket creation successful" << RESET << std::endl;
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = INADDR_ANY;
	_address.sin_port = htons(std::atoi(port));
	if (makeSocketNonBlocking(_sockfd) == -1)
		throw (std::runtime_error("fcntl failed"));
	std::cout << GREEN << "fcntl on sockfd success" << RESET << std::endl;
	if (bind(_sockfd, (struct sockaddr *)&_address, sizeof(_address)) < 0)
		throw (std::runtime_error("Socket bind failed"));else
	std::cout << GREEN << "Socket bind successful" << RESET << std::endl;
	if (listen(_sockfd, 2) < 0)
		throw (std::runtime_error("Socket listen failed"));
	std::cout << GREEN << "Socket listen successful" << RESET << std::endl;
	_epoll_fd = epoll_create1(0);
	if (_epoll_fd == -1)
		throw (std::runtime_error("Failed to create epoll fd"));
	std::cout<< GREEN << "Success to create epoll fd" << RESET << std::endl;
	_event.events = EPOLLIN;
	_event.data.fd = _sockfd;
	if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, _sockfd, &_event))
		throw (std::runtime_error("epoll fail"));
	// if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, 0, &event))
	// 	throw (std::runtime_error("epoll fail"));
	
}

int Server::get_sockfd()
{
	return (this->_sockfd);
}

int Server::get_epollfd()
{
	return (this->_epoll_fd);
}

void Server::add_client(Client &nouv)
{
	pool_client[nouv.getfd()] = nouv;
}

void Server::send_msg(std::string msg, int fd_avoid)
{
	for (std::map<int, Client>::iterator ok = pool_client.begin();ok != pool_client.end();ok++)
	{
		if (ok->first != fd_avoid && ok->first != 0)
			send(ok->first, msg.c_str(), msg.size(), 0);
	}
}

void Server::print_client()
{
	std::cout<<"----------------"<<std::endl;
	std::cout<<"Nb de client enregistre:"<<pool_client.size()<<std::endl;
	for (std::map<int, Client>::iterator ok = pool_client.begin();ok != pool_client.end();ok++)
	{
		std::cout<<"Client sur fd "<<ok->second.getfd()<<std::endl;
	}
	std::cout<<"----------------"<<std::endl;
}