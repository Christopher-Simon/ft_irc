#ifndef COMMAND_HPP
# define COMMAND_HPP

#include "Server.hpp"
#include "Client.hpp"
#include "Code.hpp"

#include <algorithm>

class Server;
class Client;
class Code;

class Command
{
public:
	std::map<std::string, void(Command::*)(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt)> cmd_repertory;
	Code *ircrep;

	Command();
	~Command();

	void exec(std::string &, Server &, Client &);
	void exec_line(std::string msg, Server &serv, Client &clt);

	void USER(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt);
	void NICK(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt);
	void PASS(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt);
	void PING(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt);
	void MODE(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt);
	void JOIN(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt);
	void LIST(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt);
	void PART(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt);
	//void INVITE(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt);
	void PRIVMSG(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt);
	void NAMES(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt);
};

void print_vect(std::vector<std::string> vect);
std::vector<std::string> ft_split(std::string &msg, char sep);

// NOTICE
// PRIVMSG
// LIST 
// PART 
// JOIN 
// MODE 
// NICK 
// PASS 
// USER 
// PING 
// reste a faire :

// mode operator channel
	// KICK
	// BAN
	// UNBAN

// mode operator serv
	//kill(pt)

//mode invisible  (?)

// INVITE a valider 
// TOPIC 

//est ce qu'on garde les keys ?
//protection pour pas d'espace dans le mdp
//pb au niveau des modes, modes internes au channel etc.

//ajout du chanlimit / client limit / client dans channel limit
//gestion des mods
//maintien du serveur en activite constante
//fichiers ?
//bot

#endif
