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
		serv.jo = new Bot(); //temp

		int event_count;
		signal(SIGINT, &sighandler);
		while (gtrl_c)
		{
			fdset.clear();
			std::cout << GREEN << "####### On epoll wait #######" << RESET << std::endl;
			serv.print_status();
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
					int fd_client = serv._events[i].data.fd;
					try{
						struct epoll_event tmp_event;
						if (serv._events[i].events & EPOLLERR)

							throw Client::LostConnExceptions("EPOLLERR");
						else if (serv._events[i].events & EPOLLHUP) 
							throw Client::LostConnExceptions("EPOLLHUP");
						else if (serv._events[i].events & EPOLLRDHUP)	
							throw Client::LostConnExceptions("EPOLLRDHUP");
						else if (serv._events[i].events & EPOLLIN) {
							serv.pool_client[fd_client]->get_msg();
							size_t t = serv.pool_client[fd_client]->get_buffer().find("\r\n");
							if (t != std::string::npos)
							{
								std::cout << "exec de ses mort" << std::endl;
								cmd.exec(serv.pool_client[fd_client]->get_buffer(), serv, *serv.pool_client[fd_client]);
								for (std::map<int, std::string>::iterator it = serv.msg_map.begin(); it != serv.msg_map.end(); ++it)
								{
									if (serv.pool_client.empty() == false
										&& serv.pool_client.find(it->first) != serv.pool_client.end())
									{
										if (serv.pool_client[it->first]->epollout == false){
											tmp_event.events = EPOLLIN | EPOLLOUT | EPOLLHUP | EPOLLERR | EPOLLRDHUP;
											tmp_event.data.fd = it->first;
											if (epoll_ctl(serv.get_epollfd(), EPOLL_CTL_MOD, it->first, &tmp_event)==-1)
												throw std::runtime_error("epoll_ctl");
											serv.pool_client[it->first]->epollout = true;
										}
									} else {
										std::cout << RED << it->first << " not found in pool_client" << RESET << std::endl;
										getwchar();
									}
								}
							} 
						} else if (serv._events[i].events & EPOLLOUT) {
							if (serv.msg_map.find(fd_client) != serv.msg_map.end())
							{
								if (send(fd_client, serv.msg_map[fd_client].c_str(), serv.msg_map[fd_client].size(), 0) == -1)
									throw std::runtime_error("send");
								std::cout << "msg sent to" << "[" << fd_client << "]" << " : " << serv.msg_map[fd_client] << std::endl;
								serv.msg_map.erase(fd_client);
								tmp_event.events = EPOLLIN | EPOLLHUP | EPOLLERR | EPOLLRDHUP;
								tmp_event.data.fd = fd_client;
								if (epoll_ctl(serv.get_epollfd(), EPOLL_CTL_MOD, fd_client, &tmp_event) == -1)
									throw std::runtime_error("epoll_ctl");
								serv.pool_client[fd_client]->epollout = false;
								if (serv.pool_client[fd_client]->_todel == true)
									serv.del_client(fd_client);
							} else {
								std::cout << RED << fd_client << " not found in msg_map" << RESET << std::endl;
								getwchar();
							}
						}
					} catch (Client::LostConnExceptions & e){
						std::cerr << ORANGE << e.what() << RESET << std::endl;
						serv.del_client(fd_client);
						serv.check_channels();
						continue ;
					}
				}
			}
		}
	} catch (std::exception & e) {
		std::cerr << RED << e.what() << RESET << std::endl;
	}
	return (0);
}
