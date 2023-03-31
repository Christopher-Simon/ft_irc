#include "irc.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include <iomanip>
#include <iterator>

bool gtrl_c(1);

void	sighandler(int signum) {
	if (signum == SIGINT)
		gtrl_c = false;
}

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << RED << "Wrong argument number." << RESET << std::endl;
		return 1;
	}
	try {
		Server	serv(argv[1]);
		int event_count;
		signal(SIGINT, &sighandler);
		while (gtrl_c)
		{
			std::cout << GREEN << "####### On epoll wait #######" << RESET << std::endl;
			event_count = epoll_wait(serv.get_epollfd(), serv._events, MAX_EVENTS, 30000);
			std::cout << "event count : " << event_count << std::endl;
			for (int i = 0; i < event_count; i++)
			{
				std::cout << "fd[" << serv._events[i].data.fd << "]" << std::endl;
				if (serv._events[i].data.fd == serv.get_sockfd())
				{
					std::cout<<"client trying connect"<<std::endl;
					serv.add_client();
				}
				else
				{
					int fd_client = serv._events[i].data.fd;
					std::string msg = serv.pool_client[fd_client]->get_msg();
					if (msg == "exit\n")
						serv.del_client(fd_client);
					else
						serv.send_msg(msg, fd_client);
				}
			}
		}
	} catch (std::exception & e) {
		std::cerr << RED << e.what() << RESET << std::endl;
	}
	return (0);
}
