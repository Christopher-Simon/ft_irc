#include "Command.hpp"

Command::Command()
{
	ircrep = new Code();

	//CAP
	//NORMAL CMD SETUP
	cmd_repertory["PING"] = &Command::PING;
	cmd_repertory["MODE"] = &Command::MODE;
	cmd_repertory["NICK"] = &Command::NICK;
	cmd_repertory["PASS"] = &Command::PASS;
	cmd_repertory["JOIN"] = &Command::JOIN;
	cmd_repertory["LIST"] = &Command::LIST;
	cmd_repertory["PART"] = &Command::PART;
	cmd_repertory["USER"] = &Command::USER;
	cmd_repertory["INVITE"] = &Command::INVITE;
	cmd_repertory["PRIVMSG"] = &Command::PRIVMSG;
	cmd_repertory["NAMES"] = &Command::NAMES;
	cmd_repertory["NOTICE"] = &Command::NOTICE;
	cmd_repertory["TOPIC"] = &Command::TOPIC;
	cmd_repertory["OPER"] = &Command::OPER;
	cmd_repertory["KILL"] = &Command::KILL;
	cmd_repertory["KICK"] = &Command::KICK;
	cmd_repertory["QUIT"] = &Command::QUIT;
}

Command::~Command()
{
	delete ircrep;
}

void Command::exec(std::string &msg, Server &serv, Client &clt)
{
	//aucune gestion des prefix pour le moment
	//integrer les commandes d'identification dans ce process
	std::cout<<"MESSAGE RECU"<<std::endl<<msg<<std::endl;
	size_t index = 0;
	while(index != std::string::npos)
	{
		this->exec_line(msg.substr(index, msg.find_first_of("\r\n", index) - index), serv, clt);
		index = msg.find_first_of("\r\n", index + 1);
		if (index == std::string::npos)
			break;
		while (msg[index] == '\r' || msg[index] == '\n')
			index++;
	}

}

void Command::exec_line(std::string msg, Server &serv, Client &clt)
{
	std::string cmd = msg.substr(0, msg.find_first_of(" \r\n",0));
	std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);
	if (cmd_repertory.find(cmd) != cmd_repertory.end())
		(this->*cmd_repertory[cmd])(cmd, ft_split(msg, ' '), serv, clt);
}

// //nous avions signe un contrat le ... pour : listing
// //il a ete confirme que tout ces elements avaient ete livres le ... 
// comment envisager de s'engager dans un nouveau contrat si le premier n'est pas honore 