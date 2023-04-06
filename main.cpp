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

// TO DO
// Controler que le nickname n'est pas deja celui d'un autre utilisateur
//pb dans le parsing du USER


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

		int event_count;
		signal(SIGINT, &sighandler);
		while (gtrl_c)
		{
			std::cout << GREEN << "####### On epoll wait #######" << RESET << std::endl;
			serv.print_status();
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
					//std::string str = serv.pool_client[fd_client]->get_buffer();
					if (serv.pool_client[fd_client]->get_buffer() == "exit\n") //TODO Replace par la command QUIT ou LEAVE ou DISCONNECT
						serv.del_client(fd_client);
					else if (serv.pool_client[fd_client]->get_buffer().find("\r\n") != std::string::npos) {
						if (NC_EASY_TEST)
							serv.send_all_msg(fd_client);
						else {
							cmd.exec(serv.pool_client[fd_client]->get_buffer(), serv, *serv.pool_client[fd_client]);//serv.send_all_msg(fd_client);
							serv.pool_client[fd_client]->get_buffer().clear();
						}
					}
				}
			}
		}
	} catch (std::exception & e) {
		std::cerr << RED << e.what() << RESET << std::endl;
	}
	return (0);
}
