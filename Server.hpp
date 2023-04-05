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
#include "Channel.hpp"

//Ajouter la gestion de la destruction des classes channel

class Client;
class Channel;

class Server 
{
	public:
		typedef	std::map<int, Client *>  mapClient;
		typedef std::map<std::string, Channel *> mapChannel;
		Server();
		Server(char *port);
		Server(Server const & raw);
		~Server();
		Server & operator=(Server const & rhs);

		int get_sockfd();
		int get_epollfd();

		void add_client();
		int check_nick_exist(std::string);
		void send_all_msg(int);
		void send_msg(std::string, int);
		void print_client();
		void del_client(int del_fd);
		void del_channel(Channel &chan);
		int channel_exist(std::string);
		int client_in_channel(std::string, Client &);

		std::string get_chan_mods(std::string);
		std::string get_userinchan_mods(std::string, Client &);

		struct epoll_event	_event;
		struct epoll_event	_events[MAX_EVENTS];
		struct sockaddr_in	_address;
		int					_addrlen;
		mapClient			pool_client;
		mapChannel			pool_channel;
		std::string			password;

	private:
		//std::vector<int>	client;
		//std::map<int, Client> client;
		int					_sockfd;
		int 				_epoll_fd;

		void	initSocket(char *port);

};

int	makeSocketNonBlocking(int sfd);

#endif