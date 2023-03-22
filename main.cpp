#include <sys/socket.h>
#include <netinet/in.h>
#include <fstream>
#include "irc.hpp"
#include <unistd.h>

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
	address.sin_port = htons(4343);
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
	int connfd = accept(sockfd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
	if (connfd < 0)
	{
		std::cout << RED << "Socket listen failed" << RESET << std::endl;
		return (1);
	}
	std::cout << GREEN << "Client accepted successful" << RESET << std::endl;
	talk(connfd);
	close(sockfd);
	return (0);
}
