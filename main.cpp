#include "irc.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Command.hpp"
#include <iomanip>
#include <iterator>
#include "Code.hpp"

bool gtrl_c(1);

void	sighandler(int signum) {
	if (signum == SIGINT)
		gtrl_c = false;
}

bool isInEnvent(struct epoll_event *events, int fd, int event_count)
{
	for (int i = 0; i < event_count; i++)
	{
		if (events[i].data.fd == fd)
			return true;
	}
	return false;
}

int	main(int argc, char *argv[])
{
	if (argc != 3)
	{
		std::cerr << RED << "Wrong argument number." << RESET << std::endl;
		return 1;
	}
	try {
		Server	serv(argv[1]);
		Command cmd;
		std::string tmp = argv[2];
		serv.password = tmp;//temp
		std::set<int> fdset;

		int event_count;
		signal(SIGINT, &sighandler);
		while (gtrl_c)
		{
			fdset.clear();
			std::cout << GREEN << "####### On epoll wait #######" << RESET << std::endl;
			// serv.print_status();
			serv.check_channels(); //A GARDER ? 
			event_count = epoll_wait(serv.get_epollfd(), serv._events, MAX_EVENTS, 30000);
			if (event_count == -1)
				throw std::runtime_error("epoll_wait");
			std::cout << "event count : " << event_count << std::endl;
			for (int i = 0; i < event_count; i++)
			{
				fdset.insert(serv._events[i].data.fd);
				std::cout << "fd[" << serv._events[i].data.fd << "]" << std::endl;
				if (serv._events[i].data.fd == serv.get_sockfd())
					serv.add_client();
				else
				{
					serv.pool_client[serv._events[i].data.fd]->epollout = !!(serv._events[i].events & EPOLLOUT);
					int fd_client = serv._events[i].data.fd;
					try{
						if (serv._events[i].events & EPOLLIN)
						{
							std::cout << "EPOLLIN" << std::endl;
							serv.pool_client[fd_client]->get_msg();
							size_t t = serv.pool_client[fd_client]->get_buffer().find("\r\n");
							if (t != std::string::npos)
							{
								if (NC_EASY_TEST)
									serv.send_all_msg(fd_client);
								else {
								std::cout << "exec de ses mort" << std::endl;
									cmd.exec(serv.pool_client[fd_client]->get_buffer(), serv, *serv.pool_client[fd_client]);
								}
							}
						}
					} catch (Client::LostConnExceptions & e){
						std::cerr << e.what() << std::endl;
						serv.del_client(fd_client);
						serv.check_channels();
						continue ;
					}
				}
			}
			std::cout << RED << "####### GROS ZIZI #######" << RESET << std::endl;
			std::cout << "msg_map size : " << serv.msg_map.size() << std::endl;
			for (
				std::map<int, std::string>::iterator it = serv.msg_map.begin();
				it != serv.msg_map.end();
			)
			{
				std::map<int, std::string>::iterator tmp = it;
				struct epoll_event tmp_event;

				if(fdset.find(it->first) != fdset.end() && serv.pool_client[it->first]->epollout == true )
				{
					if (send(it->first, it->second.c_str(), it->second.size(), 0) == -1)
						throw std::runtime_error("send");
					std::cout << BLUE << "message sent to : " << serv.pool_client[it->first]->_nickname << " [" << it->first << "]" << RESET << std::endl;

					tmp_event.events = EPOLLIN;
					tmp_event.data.fd = it->first;
					if (epoll_ctl(serv.get_epollfd(), EPOLL_CTL_MOD, it->first, &tmp_event)==-1)
						throw std::runtime_error("epoll_ctl");

					it++;
					serv.msg_map.erase(tmp);
				}
				else {
					tmp_event.events = EPOLLIN | EPOLLOUT;
					tmp_event.data.fd = it->first;
					if (epoll_ctl(serv.get_epollfd(), EPOLL_CTL_MOD, it->first, &tmp_event)==-1)
						throw std::runtime_error("epoll_ctl");
					it++;
				}
				// std::cout << "pool_client : " << it->first << " " << it->second->_nickname << std::endl;
			}
			// for (std::vector<std::pair <int, std::string > >::iterator it = serv.msg_list.begin(); it != serv.msg_list.end(); ++it)
			// {
			// 	struct epoll_event tmp_event;
			// 	// serv.msg_list
			// 	if(isInEnvent(serv._events, it->first, event_count) && serv.pool_client[it->first]->epollout == true )
			// 	{
			// 		if (send(it->first, it->second.c_str(), it->second.size(), 0) == -1)
			// 			throw std::runtime_error("send");
			// 		std::cout << BLUE << "message sent to : " << serv.pool_client[it->first]->_nickname << " [" << it->first << "]" << RESET << std::endl;

			// 		tmp_event.events = EPOLLIN;
			// 		tmp_event.data.fd = it->first;
			// 		if (epoll_ctl(serv.get_epollfd(), EPOLL_CTL_MOD, it->first, &tmp_event)==-1)
			// 			throw std::runtime_error("epoll_ctl");
			// 	}
			// 	else {
			// 		tmp_event.events = EPOLLIN | EPOLLOUT;
			// 		tmp_event.data.fd = it->first;
			// 		if (epoll_ctl(serv.get_epollfd(), EPOLL_CTL_MOD, it->first, &tmp_event)==-1)
			// 			throw std::runtime_error("epoll_ctl");
			// 	}
				
			// }
		}
	} catch (std::exception & e) {
		std::cerr << RED << e.what() << RESET << std::endl;
	}
	return (0);
}
