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
			serv.check_channels();
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
						if (serv._events[i].events & EPOLLERR)
						{
							serv.pool_client[fd_client]->leave_process(serv);
							serv.switch_pollout();
							throw Client::LostConnExceptions("EPOLLERR");
						}
						else if (serv._events[i].events & EPOLLHUP) 
						{
							serv.pool_client[fd_client]->leave_process(serv);
							serv.switch_pollout();
							throw Client::LostConnExceptions("EPOLLHUP");
						}
						else if (serv._events[i].events & EPOLLRDHUP)	
						{
							serv.pool_client[fd_client]->leave_process(serv);
							serv.switch_pollout();
							throw Client::LostConnExceptions("EPOLLRDHUP");
						}
						else if (serv._events[i].events & EPOLLIN) {
							serv.pool_client[fd_client]->get_msg();
							size_t t = serv.pool_client[fd_client]->get_buffer().find("\r\n");
							if (t != std::string::npos)
							{
								cmd.exec(serv.pool_client[fd_client]->get_buffer(), serv, *serv.pool_client[fd_client]);
								serv.switch_pollout();
							} 
						} else if (serv._events[i].events & EPOLLOUT) {
							serv.switch_pollin(fd_client);
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
