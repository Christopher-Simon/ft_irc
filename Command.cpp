#include "Command.hpp"

Command::Command()
{
	ircrep = new Code();
	cmd_repertory["PING"] = &Command::ping;
	//CAP
	//MODE
	
}

Command::~Command()
{
	delete ircrep;
}

void Command::exec(std::string &msg, Server &serv, Client &clt)
{
	//aucune gestion des prefix pour le moment
	//integrer les commandes d'identification dans ce process
	size_t index = 0; //msg.find_first_of("\r\n", 0);
	if (clt.get_status() == 0)
	{
		//std::cout<<"Request from a client connected but not idenfitied"<<serv.get_sockfd()<<std::endl;
		identify(msg, serv, clt);
		return;
	}
	while(index != std::string::npos)
	{
		this->exec_line(msg.substr(index, msg.find_first_of("\r\n", index) - index), serv, clt);

		index = msg.find_first_of("\r\n", index + 1);
		if (index == std::string::npos)
			break;
		index++;
	}

}

void Command::exec_line(std::string msg, Server &serv, Client &clt)
{
	std::cout<<"Ligne reperee : "<<msg<<std::endl;

	std::string cmd = msg.substr(0, msg.find_first_of(" \r\n",0));
	std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);
	if (cmd_repertory.find(cmd) != cmd_repertory.end())
		(this->*cmd_repertory[cmd])(cmd, msg, serv, clt);

}

//Temp
void Command::identify(std::string &msg, Server &serv, Client &clt)
{
	if (msg.substr(0, 6) != "CAP LS")
		return;
	size_t index = msg.find_first_of("\r\n", 0);
	std::cout<<"good 1 -"<<index<<std::endl;
	//while (index < msg.size() && (msg[index] == '\r' ||)
	index++;
	if (index == std::string::npos)
		return;
	std::cout<<msg.substr(index + 1, 4)<<std::endl;
	if (msg.substr(index + 1, 4) != "NICK")
		return;
	clt._nickname = msg.substr(index + 6, msg.find_first_of("\r\n", index + 6)- index - 6);
	std::cout<<"NICK = "<<clt._nickname<<std::endl;
	index = msg.find_first_of("\r\n", index + 6)+ 1;
	if (index == std::string::npos)
		return;
	if (msg.substr(index + 1, 4) != "USER")
		return;
	clt._username = msg.substr(index + 6, msg.find_first_of(" ", index + 6)- index - 6);
	index = msg.find_first_of(" ", index + 6) + 1;
	std::cout<<"USER = "<<clt._username<<std::endl;
	clt._hotsname = msg.substr(index, msg.find_first_of(" ", index + 1)- index);
	std::cout<<"HOSTNAME = "<<clt._hotsname<<std::endl;
	index = msg.find_first_of(" ", index + 1) + 1;
	clt._servername = msg.substr(index, msg.find_first_of(" ", index + 1)- index);
	std::cout<<"SERVERNAME = "<<clt._servername<<std::endl;
	index = msg.find_first_of(" ", index + 1) + 2;
	clt._realname = msg.substr(index, msg.find_first_of("\r\n", index + 1)- index);
	std::cout<<"REALNAME = "<<clt._realname<<std::endl;
	//serv.send_msg("<client> :Welcome to the <networkname> Network, <nick>[!<user>@<host>]", _fd);
	serv.send_msg(ircrep->RPL_WELCOME(clt), clt.getfd());
	serv.send_msg(ircrep->RPL_YOURHOST(clt), clt.getfd());
	serv.send_msg(ircrep->RPL_CREATED(clt), clt.getfd());
	serv.send_msg(ircrep->RPL_MYINFO(clt), clt.getfd());
	serv.send_msg(ircrep->RPL_ISUPPORT(clt), clt.getfd());
	clt._identified = 1;
	return;
}

//A REVOIR COMPLETEMENT
void Command::ping(std::string cmd, std::string msg, Server &serv, Client &clt)
{
	//std::cout<<"PING received"<<serv.get_sockfd()<<clt.get_status()<<msg<<std::endl;
	size_t index = msg.find_first_of(" \r\n", cmd.size()) + 1;
	if (index == std::string::npos) //a tester
		serv.send_msg(ircrep->ERR_NEEDMOREPARAMS(cmd, clt),clt.getfd()); //ajouter l'erreur no origin ?
	std::string arg = msg.substr(cmd.size()+1, msg.size());
	// if (arg != clt._servername)
	// 	serv.send_msg
	std::string reply = "PONG ";
	std::string space = " :";
	std::cout<<reply + clt._servername + space + arg<<std::endl;
	serv.send_msg(reply + clt._servername + space + arg, clt.getfd());
}