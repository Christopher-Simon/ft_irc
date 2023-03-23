#include <sys/socket.h>
#include <netinet/in.h>
#include <fstream>
#include "irc.hpp"
#include <unistd.h>

#include <sys/epoll.h> // for epoll_create1(), epoll_ctl(), struct epoll_event
#include <string.h> 
#include <fcntl.h>

#define MAX_EVENTS 1

void talk(int connfd)
{
	char prout[50];

	while (1)
	{
		read(connfd, &prout, sizeof(prout));
		std::cout << connfd << std::endl;
		send(connfd, "zizi", 5, 0);
	}
}

int make_socket_non_blocking (int sfd)
{
	int flags, s;

	flags = fcntl(sfd, F_GETFL, 0);
	if (flags == -1)
	{
		//perror ("fcntl");
		return -1;
	}

	flags |= O_NONBLOCK;
	s = fcntl (sfd, F_SETFL, flags);
	if (s == -1)
	{
		//perror ("fcntl");
		return -1;
	}

	return 0;
}

int	main(void)
{
	int	sockfd;
	struct sockaddr_in address;
	int addrlen = sizeof(address);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		std::cout << RED << "Socket creation failed" << RESET << std::endl;
		return (1);
	} else
		std::cout << GREEN << "Socket creation successful" << RESET << std::endl;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(4342);
	make_socket_non_blocking(sockfd);
	if (bind(sockfd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		std::cout << RED << "Socket bind failed" << RESET << std::endl;
		return (1);
	} else
		std::cout << GREEN << "Socket bind successful" << RESET << std::endl;
	if (listen(sockfd, 2) < 0)
	{
		std::cout << RED << "Socket listen failed" << RESET << std::endl;
		return (1);
	}
	std::cout << GREEN << "Socket listen successful" << RESET << std::endl;
	// int connfd = accept(sockfd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
	// if (connfd < 0)
	// {
	// 	std::cout << RED << "Socket listen failed" << RESET << std::endl;
	// 	return (1);
	// }
	// std::cout << GREEN << "Client accepted successful" << RESET << std::endl;
	// talk(connfd);
	struct epoll_event event;
	struct epoll_event events[MAX_EVENTS];
	int epoll_fd = epoll_create1(0);
	if (epoll_fd == -1)
	{
		std::cout<<RED<<"Failed to create epoll fd"<<std::endl;
		return 1;
	}
	event.events = EPOLLIN;
	event.data.fd = sockfd;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sockfd, &event))
	{
		std::cout<<"epoll fail"<<std::endl;
		return 1;
	}
	int event_count;
	//int max_elem = 0;
	//char prout[50];
	int nb_client = 4;
	while (1)
	{
		//std::cout<<"listening"<<std::endl;
		event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, 30000);
		std::cout << event_count << std::endl;
		for (int i = 0; i < event_count; i++) 
		{
			//std::cout<< "event["<<i<<"].data.fd="<<events[i].data.fd<<std::endl;
			if (events[i].data.fd == sockfd)
			{
				std::cout<<"client trying connect"<<std::endl;
				int newfd;
				newfd = accept(sockfd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
				if (newfd < 0)
				{
					std::cout << RED << "Socket listen failed" << RESET << std::endl;
				}
				else
				{
					std::cout << GREEN << "Client accepted successful : FD = " << newfd<< RESET << std::endl;
				}
				if (make_socket_non_blocking(newfd) == -1)
				{
					std::cout << RED << "fcntl failed" << RESET << std::endl;
				}
				event.events = EPOLLIN | EPOLLET;
				event.data.fd = newfd;
				if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, newfd, &event))
				{
					std::cout<<"epoll fail"<<std::endl;
					return 1;
				}
				nb_client++;
			}
			else
			{
				// read(events[i].data.fd, &prout, sizeof(prout));
				// std::cout << events[i].data.fd << std::endl;
				// send(events[i].data.fd, "zizi", 5, 0);
				int done = 0;
				while (1)
				{
					ssize_t count;
					char buf[512];

					count = read (events[i].data.fd, buf, sizeof buf);
					if (count == -1)
					{
						/* If errno == EAGAIN, that means we have read all
						data. So go back to the main loop. */
						// if (errno != EAGAIN)
						// {
						// 	//perror ("read");
						// 	done = 1;
						// }
						//std::cout << "read = 0" << std::endl;
						break;
					}
					else if (count == 0)
					{
						/* End of file. The remote has closed the
						connection. */
						done = 1;
						break;
					}
					/* Write the buffer to standard output */
					// write (1, buf, count);
					for (int ok = 4;ok <= nb_client;ok++)
					{
						if (ok != events[i].data.fd)
							send(ok, buf, count, 0);
						// write(ok, buf, count);
					}
					// {

					// }
					// if (s == -1)
					// {
					// perror ("write");
					// abort ();
					// }
				}
			}
		}
	}




	// fd_set fdreads;
	// fd_set master;
	// std::vector<int> pep;
	// FD_ZERO(&fdreads);
	// FD_ZERO(&master);
	// FD_SET(sockfd, &master);
	// int maxfd = sockfd;
	// while (1)
	// {
	// 	std::cout<<"Debut boucle"<<std::endl;
	// 	select(sockfd, &fdreads, NULL, NULL, NULL);
	// 	int i = 0;
	// 	while (i <= maxfd)
	// 	{
	// 		if (FD_ISSET(i, &fdreads))
	// 		{
	// 			if (i == sockfd)
	// 			{
	// 				int connfd = accept(sockfd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
	// 				if (connfd < 0)
	// 				{
	// 					std::cout << RED << "Socket listen failed" << RESET << std::endl;
	// 					//return (1);
	// 				}
	// 				else
	// 				{
	// 					std::cout << GREEN << "Client accepted successful" << RESET << std::endl;
	// 				}
	// 				if (connfd > maxfd)
	// 					maxfd=connfd;
	// 				pep.push_back(connfd);
	// 				std::cout<<"Connecting..."<<std::endl;
	// 				FD_SET(connfd, &master);
	// 			}
	// 		}
	// 		i++;
	// 	}
	// }
	close(sockfd);
	return (0);
}
