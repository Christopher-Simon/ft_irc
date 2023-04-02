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
	//CAP
	//NORMAL CMD SETUP
	cmd_repertory["PING"] = &Command::PING;
	cmd_repertory["MODE"] = &Command::MODE;
	cmd_repertory["NICK"] = &Command::NICK;
	cmd_repertory["PASS"] = &Command::PASS;
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
	if (cmd == "USER")
		USER(cmd, msg, serv, clt);
	else if (cmd_repertory.find(cmd) != cmd_repertory.end())
		(this->*cmd_repertory[cmd])(cmd, ft_split(msg), serv, clt);
}

//integrer le cas chiant du changement de nick une fois enregistre
void Command::NICK(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt)
{
	if (vect.size() != 2)
		return; //ERR_NONICKNAMEGIVEN
	std::string ptl_nick = vect[1];
	std::transform(ptl_nick.begin(), ptl_nick.end(), ptl_nick.begin(), ::toupper);
	mapClient::iterator it;
	for (it = serv.pool_client.begin(); it != serv.pool_client.end(); ++it)
	{
		if (ptl_nick == it->second->_intern_nick)
		{
			return; //ERR_NICKNAMEINUSE
		}
	}
	clt._nickname = vect[1];
	clt._intern_nick = ptl_nick;
	clt._identified++;
	clt.check_registered(serv, *this);
	std::cout<<cmd<<std::endl;
	//Ajout du check pour les caracteres interdits
}

void Command::USER(std::string cmd, std::string line, Server &serv, Client &clt)
{
	if (clt._identified == 3)
		return; //msg erreur ERR_ALREADYREGISTERED
	std::vector<std::string> vect;
	size_t index = 0;
	vect.push_back(line.substr(index + 6, line.find_first_of(" ", index + 6)- index - 6));
	index = line.find_first_of(" ", index + 6) + 1;
	vect.push_back(line.substr(index, line.find_first_of(" ", index + 1)- index));
	index = line.find_first_of(" ", index + 1) + 1;
	vect.push_back(line.substr(index, line.find_first_of(" ", index + 1)- index));
	index = line.find_first_of(" ", index + 1) + 2; //check l'histoire du :
	vect.push_back(line.substr(index, line.find_first_of("\r\n", index + 1)- index));
	if (vect.size() != 4)
		return; //THROW ERR_NEEDMOREPARAMS
	clt._username = vect[0];
	clt._hotsname = vect[1];
	clt._servername = vect[2];
	clt._realname = vect[3];
	clt._identified++;
	clt.check_registered(serv, *this);
	std::cout<<cmd<<std::endl;
}

void Command::PASS(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt)
{
	//cas ou le mdp possede des espaces -> verifier la validite dans la doc
	if (clt._identified == 3)
		return; //msg erreur ERR_ALREADYREGISTERED
	if (vect.size() != 2)
		return; //THROW ERR_NEEDMOREPARAMS
	if (vect[1] != serv.password)
		return; //THROW ERR_PASSWDMISMATCH 
	clt._identified++;
	clt.check_registered(serv, *this);
	std::cout<<cmd<<std::endl;
}

void Command::MODE(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt)
{
	if (clt._identified < 3)
		return; //ERR_NOTREGISTERED
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
	if (clt._identified < 3)
		return; //ERR_NOTREGISTERED
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