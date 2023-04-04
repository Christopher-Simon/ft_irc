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
					try{
						serv.pool_client[fd_client]->get_msg();
					} catch (Client::LostConnExceptions & e){
						std::cerr << e.what() << std::endl;
						serv.del_client(fd_client);
						break ;
					}
					std::string str = serv.pool_client[fd_client]->get_buffer();
					for (size_t i(0); str[i]; i++ )
					{
						if (str[i]!='\r')
							std::cout << " char : " << str[i] << " int : " << (int)str[i] << std::endl;
						else
							std::cout << " char : \\r " << " int : " << (int)str[i] << std::endl;
					}
					if (serv.pool_client[fd_client]->get_buffer() == "exit\n")
						serv.del_client(fd_client);
					else if (serv.pool_client[fd_client]->get_buffer().find("\r\n") != std::string::npos) {
						serv.send_msg(fd_client);
					}
				}
			}
		}
	} catch (std::exception & e) {
		std::cerr << RED << e.what() << RESET << std::endl;
	}
	return (0);
}
