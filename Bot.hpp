#ifndef BOT_HPP
#define BOT_HPP

#include "Client.hpp"
#include "Server.hpp"

class Client;
class Server;

class Bot
{
public :
	Bot();
	~Bot();

	std::map<int, std::vector<std::string> > memory;
	std::string id;
	void send_welcome(Client &clt, Server &serv);
	void handler(Client &clt, Server &serv, std::string);
	std::vector<std::string> new_board();
	void print_msg(Client &clt, Server &serv, std::string);
	void print_board(Client &clt, Server &serv);
	int add_piece(Client &clt, int pos, char car, Server &serv);
	int check_victory(char, Client &);
	int meilleur_coup(Client &clt);
	int possibilites(int, Client &clt);
	int check_full(Client &clt);
};

#endif