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

#include "Client.hpp"

class Client;

class Server 
{
	public:
		Server();
		Server(char *port);
		Server(Server const & raw);
		~Server();
		Server & operator=(Server const & rhs);

		int get_sockfd();
		int get_epollfd();

		void add_client();
		void send_msg(std::string, int);
		void print_client();

		struct epoll_event	_event;
		struct epoll_event	_events[MAX_EVENTS];
		struct sockaddr_in	_address;
		int					_addrlen;
		std::map<int, Client> pool_client;

	private:
		//std::vector<int>	client;
		//std::map<int, Client> client;
		int					_sockfd;
		int 				_epoll_fd;

		void	initSocket(char *port);

};

int	makeSocketNonBlocking(int sfd);

#endif