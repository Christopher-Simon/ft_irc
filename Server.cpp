#include "Server.hpp" 

int makeSocketNonBlocking(int sfd)
{
	int	s;

	s = fcntl (sfd, F_SETFL, O_NONBLOCK);
	if (s == -1)
		return -1;
	return 0;
}

Server::Server():
_event(),
_events(),
_address(),
_addrlen(),
pool_client(),
pool_channel(),
password(),
msg_map()
{
	throw (std::invalid_argument("Invalid argument"));
}

Server::Server(char *port, char *pwd):
_event(),
_events(),
_address(),
_addrlen(),
pool_client(),
pool_channel(),
password(),
msg_map()
{
	initSocket(port);
	password = pwd;
	jo = new Bot();
}

Server::Server(Server const & raw)
{
	*this = raw;
}


Server::~Server()
{
	if (VERBOSE)
		std::cout << "Destroying Server" << std::endl;
	close(_sockfd);
	close(_epoll_fd);
	mapClient::iterator it;
	for (it = pool_client.begin(); it != pool_client.end(); ++it)
		delete it->second;
	mapChannel::iterator it2;
	delete jo;
	for (it2 = pool_channel.begin(); it2 != pool_channel.end(); ++it2)
		delete it2->second;
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
	if (VERBOSE)
		std::cout << GREEN << "Socket creation successful" << RESET << std::endl;
	int optval = 1;
	if (setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)))
	{
		close(_sockfd);
		throw (std::runtime_error("Socket option set failed"));
	}

	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = INADDR_ANY;
	_address.sin_port = htons(std::atoi(port));
	if (makeSocketNonBlocking(_sockfd) == -1)
	{
		close(_sockfd);
		throw (std::runtime_error("fcntl failed"));
	}
	std::cout << GREEN << "fcntl on sockfd success" << RESET << std::endl;

	if (bind(_sockfd, (struct sockaddr *)&_address, sizeof(_address)) < 0)
	{
		close(_sockfd);
		throw (std::runtime_error("Socket bind failed"));
	}
	std::cout << GREEN << "Socket bind successful" << RESET << std::endl;

	if (listen(_sockfd, 2) < 0)
	{
		close(_sockfd);
		throw (std::runtime_error("Socket listen failed"));
	}
	std::cout << GREEN << "Socket listen successful" << RESET << std::endl;

	_epoll_fd = epoll_create1(0);
	if (_epoll_fd == -1)
	{
		close(_sockfd);
		throw (std::runtime_error("Failed to create epoll fd"));
	}
	std::cout<< GREEN << "Success to create epoll fd" << RESET << std::endl;

	// _event.events = EPOLLIN | EPOLLET;
	_event.events = EPOLLIN;
	_event.data.fd = _sockfd;
	if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, _sockfd, &_event)==-1)
	{
		close(_sockfd);
		close(_epoll_fd);
		throw (std::runtime_error("epoll fail"));
	}
}

int	Server::get_sockfd()
{
	return (this->_sockfd);
}

int	Server::get_epollfd()
{
	return (this->_epoll_fd);
}

void	Server::add_client()
{
	//std::cout<<"client trying connect"<<std::endl;
	int newFd = accept(_sockfd, (struct sockaddr *)&_address, (socklen_t *)&_addrlen);
	if (newFd < 0)
		throw (std::runtime_error("Socket listen failed"));
	if (pool_client.size() >= 50)
	{
		close(newFd);
		std::cerr << "Too many clients" << std::endl;
	}
	std::cout << GREEN << "Client accepted successful : FD = " << newFd<< RESET << std::endl;
	if (makeSocketNonBlocking(newFd) == -1)
	{
		close(newFd);
		throw (std::runtime_error("fcntl failed"));
	}
	_event.events = EPOLLIN | EPOLLHUP | EPOLLERR | EPOLLRDHUP;
	_event.data.fd = newFd;
	if (epoll_ctl(get_epollfd(), EPOLL_CTL_ADD, newFd, &_event) == -1)
	{
		close(newFd);
		throw (std::runtime_error("epoll_ctl fail"));
	}
	try
	{
		pool_client[newFd] = new Client(newFd);
	}
	catch(const std::exception& e)
	{
		close(newFd);
		throw (std::bad_alloc());
	}
	
}

void Server::store_all_msg(int fd_avoid)
{
	std::string msg = pool_client[fd_avoid]->get_buffer();
	for (std::map<int, Client *>::iterator ok = pool_client.begin();ok != pool_client.end();ok++)
	{
		if (ok->first != fd_avoid && ok->first != 0)
			msg_map[ok->first] += msg;
	}
	pool_client[fd_avoid]->get_buffer().clear();
}

void Server::store_all_msg(std::string msg, int fd_avoid)
{
	size_t i(0);
	size_t pos(0);
	while ((pos = msg.find("\r\n", pos + 1)) != std::string::npos)
		i++;
	if (i == 0 || i >= 2)
		std::cerr << RED << "\\r\\n " << i << " times  in : " << msg << RESET << std::endl;
	for (std::map<int, Client *>::iterator ok = pool_client.begin();ok != pool_client.end();ok++)
	{
		if (ok->first != fd_avoid && ok->first != 0)
			msg_map[ok->first] += msg;
	}
}

void Server::store_channel_msg(std::string msg2, std::string channel, int fd_avoid)
{
	std::string msg = msg2 + "\r\n";
	std::map<int, std::string> clients = pool_channel[toupper(channel)]->_members;
	for (std::map<int, std::string>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->first != fd_avoid)
			msg_map[it->first] += msg;
	}
}


void	Server::del_client(int del_fd)
{
	mapClient::iterator	it;

	it = pool_client.find(del_fd);
	if (it == pool_client.end())
	{
		std::cerr << "error : del_client, fd doesn't exist";
		return ;
	}
	std::cout << "delete client fd : " << it->first << std::endl;
	if (epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, del_fd, NULL) ==-1)
		throw (std::runtime_error("epoll DEL fail"));
	mapChannel::iterator it2;
	for (it2 = pool_channel.begin(); it2 != pool_channel.end(); ++it2)
	{
		if (it2->second->_members.find(del_fd) != it2->second->_members.end()) // si le client a delete fait partie du channel
			it2->second->_members.erase(del_fd);
	}
	if (msg_map.find(del_fd) != msg_map.end())
		msg_map.erase(del_fd);
	delete	it->second;
	pool_client.erase(it);
}

void Server::del_channel(Channel &chan)
{
	//if (chan.nb_memb != 0 || chan._members.size() != 0)
		//std::cout<<"Warning : deleting non empty channel"<<std::endl;
	std::string name = toupper(chan._name);
	if (pool_channel.find((name)) == pool_channel.end())
	{
		//std::cout<<"Error : channel to delete not found"<<std::endl;
		return;
	}
	delete(pool_channel.find((name))->second);
	//std::cout<<"Info : channel "<< name << " deleted"<<std::endl;

	pool_channel.erase((name));
}

void Server::store_msg(std::string msg2, int fd)
{
	size_t i(0);
	size_t pos(0);
	std::string msg = msg2 + "\r\n";
	while ((pos = msg.find("\r\n", pos + 1)) != std::string::npos)
		i++;
	if (i == 0 || i >= 2)
		std::cerr << RED << "\\r\\n " << i << " times  in : " << msg << RESET << std::endl;
	msg_map[fd] += msg;

}

void Server::print_client()
{
	std::cout<<"----------------"<<std::endl;
	std::cout<<"Nb de client enregistre:"<<pool_client.size()<<std::endl;
	for (std::map<int, Client *>::iterator ok = pool_client.begin();ok != pool_client.end();ok++)
	{
		std::cout << "Client sur fd " << ok->second->getfd() << std::endl;
	}
	std::cout<<"----------------"<<std::endl;
}

int Server::check_nick_exist(std::string nick)
{
	mapClient::iterator it;
	std::string ptl_nick = nick;
	std::transform(ptl_nick.begin(), ptl_nick.end(), ptl_nick.begin(), ::toupper);
	if (ptl_nick == "BOT")
		return (-1);
	for (it = pool_client.begin(); it != pool_client.end(); ++it)
	{
		if (ptl_nick == it->second->_intern_nick && pool_client[it->second->getfd()]->_identified == 3)
			return (it->second->getfd());
	}
	return (0);
}

// int Server::channel_exist(std::string title)
// {
// 	mapChannel::iterator it;
// 	std::string ptl_name = title;
// 	std::string ref;
// 	std::transform(ptl_name.begin(), ptl_name.end(), ptl_name.begin(), ::toupper);
// 	for (it = pool_channel.begin(); it != pool_channel.end(); it++)
// 	{
// 		ref = it->first;
// 		std::transform(ref.begin(), ref.end(), ref.begin(), ::toupper);
// 		if (ref == ptl_name)
// 			return 1;
// 	}
// 	return 0;
// }

int Server::channel_exist(std::string title)
{
	if (pool_channel.find(toupper(title)) != pool_channel.end())
		return 1;
	return 0;
}

int Server::client_in_channel(std::string title, Client &clt)
{
	if (pool_channel.find((toupper(title)))->second->_members.find(clt.getfd()) != pool_channel.find((toupper(title)))->second->_members.end())
		return (1);
	return 0;
}

int	Server::user_in_channel(std::string title, std::string user)
{
	std::string channel = toupper(title);
	if (pool_channel.find((channel)) == pool_channel.end())
	{
		std::cerr<<RED<<"Error user_in_channel : channel not found"<<RESET<<std::endl;
		return (0);
	}
	int fduser = check_nick_exist(user);
	if (fduser == 0)
	{
		std::cerr<<RED<<"Error check_nick_exist : user not found"<<RESET<<std::endl;
		return (0);
	}
	std::map<int, std::string> clients = pool_channel[(channel)]->_members;
	for (std::map<int, std::string>::iterator it = clients.begin(); it != clients.end(); ++it)
		if (it->first == fduser)
			return (1);
	return (0);
}

std::string Server::get_chan_mods(std::string title)
{
	return (pool_channel.find((toupper(title)))->second->_channel_mods);
}

std::string Server::get_userinchan_mods(std::string title, Client &clt)
{
	return (pool_channel.find((toupper(title)))->second->_members[clt.getfd()]);
}

void Server::print_status()
{
	std::cout<<"--------------------------------------"<<std::endl;
	std::cout<<"|	Number of connected clients : "<<pool_client.size()<<std::endl;
	std::cout<<"|"<<std::endl;
	std::cout<<"|	List of clients and mods :"<<std::endl;
	mapClient::iterator it;
	for (it = pool_client.begin(); it != pool_client.end(); ++it)
	{
		std::cout<<"|		- "<<it->second->get_nick()<<" = "<<it->second->_mods;
		if (it->second->_identified < 3)
			std::cout<<" (Not registered)";
		std::cout<<std::endl;
	}
	std::cout<<"|"<<std::endl;
	std::cout<<"|	List of channels and mods :"<<std::endl;
	mapChannel::iterator it2;
	for (it2 = pool_channel.begin(); it2 != pool_channel.end(); ++it2)
	{
		std::cout<<"|		- "<<it2->second->_intern_name <<" = "<<it2->second->_channel_mods<<std::endl;
		if (it2->second->_members.size() != 0)
		{
			std::map<int, std::string>::iterator it3;
			for (it3 = it2->second->_members.begin(); it3 != it2->second->_members.end(); ++it3)
				std::cout<<"|			> "<<pool_client[it3->first]->get_nick()<<" = "<< it3->second<<std::endl;
			std::cout<<"| liste d'attente : "<<std::endl;
			std::map<int, Client *>::iterator it4;
			for (it4 = it2->second->invited_clients.begin(); it4 != it2->second->invited_clients.end(); ++it4)
				std::cout<<"|			x "<<it4->second->get_nick()<<std::endl;
		}
	}
	std::cout<<"--------------------------------------"<<std::endl;
	std::cout<<std::endl;
}

int Server::chan_has_mod(std::string title, char wanted_mod)
{
	if (get_chan_mods(toupper(title)).find(wanted_mod) != std::string::npos)
		return 1;
	return 0;
}

Channel *Server::get_chan(std::string title)
{
	return (pool_channel.find((toupper(title)))->second);
}

void Server::check_channels()
{
	size_t at_start = pool_channel.size();
	mapChannel::iterator it2;
	for (it2 = pool_channel.begin(); it2 != pool_channel.end(); ++it2)
	{
		if (it2->second->_members.size() == 0)
		{
			std::string sup = it2->first;
			delete it2->second;
			pool_channel.erase((sup));
			break;
		}
	}
	if (at_start != pool_channel.size())
		check_channels();
}

void Server::check_clients()
{
	size_t at_start = pool_client.size();
	mapClient::iterator it;
	for (it = pool_client.begin(); it != pool_client.end(); ++it)
	{
		if (it->second->_todel == 1)
		{
			del_client(it->first);
			break;
		}
	}
	if (at_start != pool_client.size())
		check_clients();
}

void	Server::switch_pollout() {
	struct epoll_event tmp_event;

	for (std::map<int, std::string>::iterator it = msg_map.begin(); it != msg_map.end(); ++it)
	{
		if (pool_client.empty() == false
			&& pool_client.find(it->first) != pool_client.end())
		{
			if (pool_client[it->first]->epollout == false){
				tmp_event.events = EPOLLIN | EPOLLOUT | EPOLLHUP | EPOLLERR | EPOLLRDHUP;
				tmp_event.data.fd = it->first;
				if (epoll_ctl(get_epollfd(), EPOLL_CTL_MOD, it->first, &tmp_event)==-1)
					throw std::runtime_error("epoll_ctl");
				pool_client[it->first]->epollout = true;
			}
		}
		// else {
		// 	std::cout << RED << it->first << " not found in pool_client" << RESET << std::endl;
		// 	getwchar();
		// }
	}
}

void	Server::switch_pollin(int fd_client) {
	struct epoll_event tmp_event;

	if (msg_map.find(fd_client) != msg_map.end())
	{
		if (send(fd_client, msg_map[fd_client].c_str(), msg_map[fd_client].size(), 0) == -1)
			throw std::runtime_error("send");
		//std::cout << "msg sent to" << "[" << fd_client << "]" << " : " << msg_map[fd_client] << std::endl;
		msg_map.erase(fd_client);
		tmp_event.events = EPOLLIN | EPOLLHUP | EPOLLERR | EPOLLRDHUP;
		tmp_event.data.fd = fd_client;
		if (epoll_ctl(get_epollfd(), EPOLL_CTL_MOD, fd_client, &tmp_event) == -1)
			throw std::runtime_error("epoll_ctl");
		pool_client[fd_client]->epollout = false;
		if (pool_client[fd_client]->_todel == true)
		{
			del_client(fd_client);
			check_channels();
		}
	} 
	// else {
	// 	std::cout << RED << fd_client << " not found in msg_map" << RESET << std::endl;
	// 	getwchar();
	// }
}

// std::string Server::getname(std::string title)
// {
// 	std::string ptl_name = title;
// 	std::transform(ptl_name.begin(), ptl_name.end(), ptl_name.begin(), ::toupper);
// 	std::map<std::string, Channel *>::iterator it;
// 	for (it = pool_channel.begin(); it != pool_channel.end(); it++)
// 	{
// 		if (it->second->_intern_name == ptl_name)
// 			return it->second->_name;
// 	}
// 	return (title);
// }

std::string Server::toupper(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::toupper);
	return str;
}