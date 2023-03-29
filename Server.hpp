#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <exception>
#include <string>
#include <algorithm>
#include <vector>
#include <list>
#include <map>

#include "irc.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <fstream>
#include <unistd.h>
#include <sys/epoll.h>
#include <string.h> 
#include <fcntl.h>

#define MAX_EVENTS 1

class Server 
{
	public:
		Server();
		Server(char *port);
		Server(Server const & raw);
		~Server();
		Server & operator=(Server const & rhs);

	private:
		std::vector<int>	client;
		int					_sockfd;
		struct sockaddr_in	_address;
		int					_addrlen;
		struct epoll_event	_event;
		struct epoll_event	_events[MAX_EVENTS];
		int 				_epoll_fd;

		void	initSocket();
		int		makeSocketNonBlocking(int sfd);
		void	serverLoop();
};

#endif