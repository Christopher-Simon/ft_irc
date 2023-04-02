#include "Command.hpp"

std::vector<std::string> ft_split(std::string &msg)
{
	size_t index = 0;
	int flag = 0;
	std::vector<std::string> vect;
	while (index != std::string::npos)
	{
		while(msg[index] != '\0' && msg[index] == ' ')
			index++;
		if (flag == 1 && msg[msg.find_first_of(" \0", index)] == '\0')
			break;
		std::string morceau = msg.substr(index, msg.find_first_of(" \0", index) - index);
		vect.push_back(morceau);
		flag = 1;
		index = msg.find_first_of(" \0", index);
	}
	return (vect);
}

Command::Command()
{
	ircrep = new Code();
	cmd_repertory["PING"] = &Command::PING;
	//CAP
	cmd_repertory["MODE"] = &Command::MODE;
	
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
		(this->*cmd_repertory[cmd])(cmd, ft_split(msg), serv, clt);

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


void Command::MODE(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt)
{
	if (vect.size() < 2)
		return; //THROW ERR_NEEDMOREPARAMS
	if (vect[1][0] == '#')
	{
		//MODE CHANNEL
		std::string target = vect[1].substr(1, vect[1].size());
		if (serv.pool_channel.find(target) == serv.pool_channel.end())
			return; //THROW ERR_NOSUCHCHANNEL
		if (vect.size() == 2)
			return; //RPL_CHANNELMODEIS
		if (serv.pool_channel.find(target)->second->_members.find(&clt)->second.find('o', 0) == std::string::npos)
			return; //ERR_CHANOPRIVSNEEDED
		//TBC
	}
	std::cout<<cmd<<std::endl;
	return;
}

//A REVOIR COMPLETEMENT
void Command::PING(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt)
{
	if (vect.size() != 2) //a tester
		serv.send_msg(ircrep->ERR_NEEDMOREPARAMS(cmd, clt),clt.getfd()); //ajouter l'erreur no origin ?
	//std::string arg = msg.substr(cmd.size()+1, msg.size());
	// if (arg != clt._servername)
	// 	serv.send_msg
	std::string reply = "PONG ";
	std::string space = " :";
	std::cout<<reply + clt._servername + space + vect[1]<<std::endl;
	serv.send_msg(reply + clt._servername + space + vect[1], clt.getfd());
}