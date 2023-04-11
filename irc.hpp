#ifndef IRC_HPP
# define IRC_HPP

#include <iostream>
#include <exception>
#include <string>
#include <algorithm>
#include <vector>
#include <list>
#include <map>

#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fstream>
#include <unistd.h>
#include <sys/epoll.h>
#include <string.h> 
#include <fcntl.h>
#include  <iostream>
#include  <bits/stdc++.h>

#define GREEN "\033[32m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define PURPLE "\033[35m"
#define RESET "\033[0m"

#define BUFFER_SIZE 10000
#define MAX_EVENTS 1000
#define	NC_EASY_TEST 0

#define U_AWAY 'a'
#define U_INVISIBLE 'i'
#define U_WALLOPS 'w'
#define U_RESTRICTED 'r'
#define U_OPERATOR 'o'

#define C_INVITE 'i'
#define C_KEYLOCKED 'k'

#define CU_OPERATOR 'o'
//#define CU_VOICE 'v'

extern bool gtrl_c;

#endif