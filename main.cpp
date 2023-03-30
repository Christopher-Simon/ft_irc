#include "irc.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include <iomanip>
#include <iterator>

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
		Client NvServ(serv, 0);
		serv.add_client(NvServ);
		while (1)
		{
			event_count = epoll_wait(serv.get_epollfd(), serv._events, MAX_EVENTS, 30000);
			std::cout << event_count << std::endl;
			for (int i = 0; i < event_count; i++) 
			{
				std::cout << serv._events[i].data.fd << std::endl;
				if (serv._events[i].data.fd == serv.get_sockfd())
				{
					std::cout<<"client trying connect"<<std::endl;
					Client Nouv(serv);
					serv.add_client(Nouv);
					//serv.print_client();
				}
				else if (serv._events[i].data.fd == 0)
				{
					std::string msg = serv.pool_client[0].get_msg();
					std::cout<<"Server a dit: "<<msg;
					if (strcmp(msg.substr(0, msg.size()).c_str(), "exit\n") == 0)
					{
						// close all clients
						close(serv.get_sockfd());
						exit(0);
					}
				}
				else
				{
					int fd_client = serv._events[i].data.fd;
					std::string msg = serv.pool_client[fd_client].get_msg();
					if (serv.pool_client[fd_client].get_status() == 1)
						serv.send_msg(msg, fd_client);
					else if (serv.pool_client[fd_client].get_status() == 0)
						serv.pool_client[fd_client].identify(msg);
				}
			}
		}
		close(serv.get_sockfd());
	} catch (std::exception & e) {
		std::cerr << RED << e.what() << RESET << std::endl;
	}
	return (0);
}
