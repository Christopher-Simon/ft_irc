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
	serverLoop();
}
// int makeSocketNonBlocking(int sfd)
// {
// 	int flags, s;

// 	flags = fcntl(sfd, F_GETFL, 0);
// 	if (flags == -1)
// 	{
// 		return -1;
// 	}
// 	flags |= O_NONBLOCK;
// 	s = fcntl (sfd, F_SETFL, flags);
// 	if (s == -1)
// 	{
// 		return -1;
// 	}
// 	return 0;
// }

void	Server::serverLoop() {
	int event_count;
	int nb_client = 4;
	(void)_events;
	//SET UP THE ADMIN WINDOW
	std::cout<<"Set up server window"<<std::endl;
	//int newfd;
	//newfd = accept(_sockfd, (struct sockaddr *)&_address, (socklen_t *)&_addrlen);
	// if (newfd < 0)
	// {
	// 	std::cout << RED << "Socket listen failed" << RESET << std::endl;
	// }
	// else
	// {
	// 	std::cout << GREEN << "Client server accepted successful : FD = " << newfd<< RESET << std::endl;
	// }
	if (makeSocketNonBlocking(0) == -1)
	{
		std::cout << RED << "fcntl failed" << RESET << std::endl;
	}
	_event.events = EPOLLIN | EPOLLET;
	_event.data.fd = 0;
	if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, 0, &_event))
		throw (std::runtime_error("epoll fail"));
	nb_client++;
	//LOOP
	while (1)
	{
		event_count = epoll_wait(_epoll_fd, _events, MAX_EVENTS, 30000);
		std::cout << event_count << std::endl;
		// char	str[512];
		// if (read(0, str, sizeof(str)) > 0)
		// std::cout << str << std::endl;
		for (int i = 0; i < event_count; i++) 
		{
			std::cout << _events[i].data.fd << std::endl;
			if (_events[i].data.fd == _sockfd)
			{
				std::cout<<"client trying connect"<<std::endl;
				//constructeur client
				int newfd;
				newfd = accept(_sockfd, (struct sockaddr *)&_address, (socklen_t *)&_addrlen);
				if (newfd < 0)
				{
					std::cout << RED << "Socket listen failed" << RESET << std::endl;
				}
				else
				{
					std::cout << GREEN << "Client accepted successful : FD = " << newfd<< RESET << std::endl;
				}
				if (makeSocketNonBlocking(newfd) == -1)
				{
					std::cout << RED << "fcntl failed" << RESET << std::endl;
				}
				_event.events = EPOLLIN | EPOLLET;
				_event.data.fd = newfd;
				if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, newfd, &_event))
					throw (std::runtime_error("epoll fail"));
				//IDENTIFICATION CLIENT
				// while (1)
				// {
				// 	ssize_t counti;
				// 	char bufi[512];

				// 	counti = read (_events[i].data.fd, bufi, sizeof bufi);
				// 	if (counti == -1 || counti == 0)
				// 	{
				// 		//done = 1;
				// 		break;
				// 	}
				// 	std::string id = bufi;
				// 	std::cout<<"ID a dit: "<<id.substr(0,counti);
				// }
				nb_client++;
			}
			else if (_events[i].data.fd == 0)
			{
				ssize_t countserv;
				char bufs[512];

				countserv = read (_events[i].data.fd, bufs, sizeof bufs);
				if (countserv == -1 || countserv == 0)
				{
					//done = 1;
					break;
				}
				std::string msg = bufs;
				std::cout<<"Server a dit: "<<msg.substr(0,countserv);
				if (strcmp(msg.substr(0,countserv).c_str(), "exit\n") == 0)
				{
					// close all clients
					close(_sockfd);
					exit(0);
				}
			}
			else
			{
				//int done = 0;
				while (1)
				{
					ssize_t count;
					char buf[512];

					count = read (_events[i].data.fd, buf, sizeof buf);
					if (count == -1 || count == 0)
					{
						//done = 1;
						break;
					}
					for (int ok = 4;ok <= nb_client;ok++)
					{
						if (ok != _events[i].data.fd)
							send(ok, buf, count, 0);
					}
				}
			}
		}
	}
	close(_sockfd);
}