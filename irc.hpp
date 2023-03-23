#ifndef IRC_HPP
# define IRC_HPP

#include <iostream>
#include <exception>
#include <string>
#include <algorithm>
#include <vector>
#include <list>
#include <map>

#define GREEN "\033[32m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define PURPLE "\033[35m"
#define RESET "\033[0m"

class irc 
{
	public:
		irc();
		irc(irc const & raw);
		~irc();
		irc & operator=(irc const & rhs);

	private:
		
};

//faire une classe client

// typedef struct	t_env
// {
// 	struct sockaddr_storage	their_addr;
// 	socklen_t		addr_size;
// 	int			newfd;
// 	int			sockfd;
// 	int			fdmax;
// 	fd_set			master;
// 	fd_set			fdreads;
// }		s_env;


#endif