#include "Command.hpp"

void print_vect(std::vector<std::string> vect)
{
	for (size_t i = 0; i < vect.size(); i++)
		std::cout << "vect["<<i<<"]: "<<vect[i]<<std::endl;
}

std::vector<std::string> ft_split(std::string &msg, char sep)
{
	std::vector<std::string> vect;
	std::istringstream f(msg);
	std::string s;
	if (msg.find(sep) == std::string::npos)
		return (vect.push_back(msg), vect);
	while (getline(f, s, sep))
		vect.push_back(s);
	print_vect(vect);
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
	cmd_repertory["JOIN"] = &Command::JOIN;
	cmd_repertory["LIST"] = &Command::LIST;
	cmd_repertory["PART"] = &Command::PART;
}

Command::~Command()
{
	delete ircrep;
}

void Command::exec(std::string &msg, Server &serv, Client &clt)
{
	//aucune gestion des prefix pour le moment
	//integrer les commandes d'identification dans ce process
	size_t index = 0;
	while(index != std::string::npos)
	{
		this->exec_line(msg.substr(index, msg.find_first_of("\r\n", index) - index), serv, clt);
		index = msg.find_first_of("\r\n", index + 1);
		while (msg[index] == '\r' || msg[index] == '\n')
			index++;
		if (index == std::string::npos)
			break;
	}

}

void Command::exec_line(std::string msg, Server &serv, Client &clt)
{
	//std::cout<<"Ligne reperee : "<<msg<<std::endl;
	std::string cmd = msg.substr(0, msg.find_first_of(" \r\n",0));
	std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);
	if (cmd == "USER")
		USER(cmd, msg, serv, clt);
	else if (cmd_repertory.find(cmd) != cmd_repertory.end())
		(this->*cmd_repertory[cmd])(cmd, ft_split(msg, ' '), serv, clt);
}

//integrer le cas chiant du changement de nick une fois enregistre
void Command::NICK(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt)
{
	if (vect.size() != 2) //le code erreur renvoie le nick actuel, quid dans le cas du setup initial ?
	{
		serv.send_msg(ircrep->ERR_NONICKNAMEGIVEN(clt),clt.getfd());
		return;
	}
	std::string ptl_nick = vect[1];
	std::transform(ptl_nick.begin(), ptl_nick.end(), ptl_nick.begin(), ::toupper);
	Server::mapClient::iterator it;
	// for (it = serv.pool_client.begin(); it != serv.pool_client.end(); ++it)
	// {
	// 	if (ptl_nick == it->second->_intern_nick)
	// 	{
	// 		serv.send_msg(ircrep->ERR_NICKNAMEINUSE(vect[1], clt),clt.getfd());
	// 		return;
	// 	}
	// }
	if (serv.check_exist(ptl_nick) == 1)
	{
		serv.send_msg(ircrep->ERR_NICKNAMEINUSE(vect[1], clt),clt.getfd());
		return;
	}
	clt._nickname = vect[1];
	clt._intern_nick = ptl_nick;
	if (clt._identified < 3)
	{
		clt._identified++;
		std::cout<<clt._identified<<std::endl;
		clt.check_registered(serv, *this);
	}
	cmd.empty(); 
	//Ajout du check pour les caracteres interdits
}

void Command::USER(std::string cmd, std::string line, Server &serv, Client &clt)
{
	if (clt._identified == 3)
	{
		serv.send_msg(ircrep->ERR_ALREADYREGISTERED(clt),clt.getfd());
		return;
	}
	//PARSING PAS PARFAIT
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
	{
		serv.send_msg(ircrep->ERR_NEEDMOREPARAMS(cmd, clt),clt.getfd());
		return;
	}
	clt._username = vect[0];
	clt._hotsname = vect[1];
	clt._servername = vect[2];
	clt._realname = vect[3];
	clt._identified++;
	std::cout<<clt._identified<<std::endl;
	clt.check_registered(serv, *this);
	std::cout<<cmd<<std::endl;
}

void Command::PASS(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt)
{
	//cas ou le mdp possede des espaces -> verifier la validite dans la doc
	if (clt._identified == 3)
	{
		serv.send_msg(ircrep->ERR_ALREADYREGISTERED(clt),clt.getfd());
		return;
	}
	if (vect.size() != 2)
	{
		serv.send_msg(ircrep->ERR_NEEDMOREPARAMS(cmd, clt),clt.getfd());
		return;
	}
	if (vect[1] != serv.password)
	{
		serv.send_msg(ircrep->ERR_PASSWDMISMATCH(clt),clt.getfd());
		return;
	}
	clt._identified++;
	std::cout<<clt._identified<<std::endl;
	clt.check_registered(serv, *this);
}

void Command::MODE(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt)
{
	if (clt._identified < 3)
	{
		serv.send_msg(ircrep->ERR_NOTREGISTERED(clt),clt.getfd());
		return;
	}
	if (vect.size() < 2)
	{
		serv.send_msg(ircrep->ERR_NEEDMOREPARAMS(cmd, clt),clt.getfd());
		return;
	}
	if (vect[1][0] == '#')
	{
		//MODE CHANNEL IMCOMPLET
		//ajout du check du nb de parametres
		std::string target = vect[1].substr(1, vect[1].size());
		if (serv.pool_channel.find(target) == serv.pool_channel.end())
			return; //THROW ERR_NOSUCHCHANNEL
		if (vect.size() == 2)
			return; //RPL_CHANNELMODEIS
		if (serv.pool_channel.find(target)->second->_members.find(&clt)->second.find('o', 0) == std::string::npos)
			return; //ERR_CHANOPRIVSNEEDED
		//TBC
	}
	else
	{
		//question si le mode sort un nickname deja existant mais sans maj par exemple
		std::string ptl_nick = vect[1].substr(0, vect[1].find_first_of(" \r\n",0));
		std::transform(ptl_nick.begin(), ptl_nick.end(), ptl_nick.begin(), ::toupper);
		if (serv.check_exist(ptl_nick)== 0)
		{
			return; //ERR_NOSUCHNICK
		}
		if (ptl_nick != clt._intern_nick)
		{
			//ERR_USERSDONTMATCH
			return;
		}
		if (vect.size() == 2)
		{
			serv.send_msg(ircrep->RPL_UMODEIS(clt), clt.getfd());
			return;
		}
		//aucun flag gere pour le moment
		else if (vect.size() > 3)
		{
			serv.send_msg(ircrep->RPL_UMODEIS(clt), clt.getfd());
			return;
		}
		std::string mods = "airwro";
		if (!((vect[2][0] == '+' || vect[2][0] == '-') && vect[2].length() == 2 && mods.find(vect[2][1]) != std::string::npos))
		{
			return;
		}
		if (vect[2][0] == '+')
			clt.add_mod(vect[2][1]);
		else if (vect[2][0] == '-')
			clt.rem_mod(vect[2][1]);
	}
	cmd.empty();
	return;
}

//A REVOIR COMPLETEMENT
void Command::PING(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt)
{
	if (clt._identified < 3)
	{
		serv.send_msg(ircrep->ERR_NOTREGISTERED(clt),clt.getfd());
		return;
	}
	if (vect.size() != 2) //a tester
		serv.send_msg(ircrep->ERR_NEEDMOREPARAMS(cmd, clt),clt.getfd()); //ajouter l'erreur no origin ?
	//std::string arg = msg.substr(cmd.size()+1, msg.size());
	// if (arg != clt._servername)
	// 	serv.send_msg
	std::string reply = "PONG ";
	std::string space = " :";
	//std::cout<<reply + clt._servername + space + vect[1]<<std::endl;
	serv.send_msg(reply + clt._servername + space + vect[1], clt.getfd());
	//std::cout<<cmd<<serv.get_sockfd()<<std::endl;
}

//ajout du droit de channel operator quand le client cree un channel qui existait pas
void Command::JOIN(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt)
{
	if (clt._identified < 3)
	{
		serv.send_msg(ircrep->ERR_NOTREGISTERED(clt),clt.getfd());
		return;
	}
	if (vect.size() == 1)
	{
		serv.send_msg(ircrep->ERR_NEEDMOREPARAMS(cmd, clt),clt.getfd());
		return;
	}
	std::vector<std::string> list_channel = ft_split(vect[1], ',');
	if (vect.size() == 3)
	{
		std::vector<std::string> list_key = ft_split(vect[2], ',');
		// for(size_t i = 0; i < list_channel.size(); i++)
		// {

		// }
		//TBC
		return;
	}
	//gerer le cas ou les noms sont des memes avec des majuscules
	//faire gaffe a ce que les messages d'erreur ne bloquent pas l'execution du reste de la liste
	for(size_t i = 0; i < list_channel.size(); i++)
	{
		if (serv.pool_channel.find(list_channel[i]) != serv.pool_channel.end()) //Channel trouve
		{
			if (serv.pool_channel.find(list_channel[i])->second->_members.find(&clt) != serv.pool_channel.find(list_channel[i])->second->_members.end()) //client deja present dans le channel
				return;
			serv.pool_channel.find(list_channel[i])->second->_members[&clt] = "i";
			serv.pool_channel.find(list_channel[i])->second->nb_memb++;
			std::cout<<clt.get_nick()<<" added to channel "<<list_channel[i]<<std::endl;
		}
		else //channel pas existante
		{
			serv.pool_channel[list_channel[i]] = new Channel(list_channel[i]);
			serv.pool_channel.find(list_channel[i])->second->_members[&clt] = "O";
			std::cout<<clt.get_nick()<<" added to created channel "<<list_channel[i]<<std::endl;
		}
	}
	return;
}

void Command::LIST(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt)
{
	if (clt._identified < 3)
	{
		serv.send_msg(ircrep->ERR_NOTREGISTERED(clt),clt.getfd());
		return;
	}
	if (vect.size() == 1)
	{
		serv.send_msg(ircrep->RPL_LISTSTART(clt), clt.getfd());
		Server::mapChannel::iterator it2;
		for (it2 = serv.pool_channel.begin(); it2 != serv.pool_channel.end(); ++it2)
			serv.send_msg(ircrep->RPL_LIST(clt, it2->second->_name, it2->second->nb_memb), clt.getfd());
		serv.send_msg(ircrep->RPL_LISTEND(clt), clt.getfd());
	}
	cmd.empty();
}

//Ajouter l'option reason 
void Command::PART(std::string cmd, std::vector<std::string> vect, Server &serv, Client &clt)
{
	if (clt._identified < 3)
	{
		serv.send_msg(ircrep->ERR_NOTREGISTERED(clt),clt.getfd());
		return;
	}
	if (vect.size() == 1)
	{
		serv.send_msg(ircrep->ERR_NEEDMOREPARAMS(cmd, clt),clt.getfd());
		return;
	}
	std::vector<std::string> list_channel = ft_split(vect[1], ',');
	for(size_t i = 0; i < list_channel.size(); i++)
	{
		if (serv.pool_channel.find(list_channel[i]) == serv.pool_channel.end())
			serv.send_msg(ircrep->ERR_NOSUCHCHANNEL(clt, list_channel[i]),clt.getfd());
		else if (serv.pool_channel.find(list_channel[i])->second->_members.find(&clt) == serv.pool_channel.find(list_channel[i])->second->_members.end()) //client pas dans le channel
			serv.send_msg(ircrep->ERR_NOTONCHANNEL(clt, list_channel[i]),clt.getfd());
		else
		{
			std::cout<<"Temp : leaving channel"<<std::endl;
			serv.pool_channel.find(list_channel[i])->second->_members.erase(&clt);
			serv.pool_channel.find(list_channel[i])->second->nb_memb--;
			std::string reply = ":";
			std::string r2 = "!~";
			std::string r3 = "@";
			std::string r5 = " PART ";
			std::string r4 = " :Leaving the channel";
			std::cout<<reply + clt.get_nick() + r2 + clt.get_nick() + r3 + clt._servername + r5 + list_channel[i] + r4<<std::endl;
			serv.send_msg(reply + clt.get_nick() + r2 + clt.get_nick() + r3 + clt._servername + r5 + list_channel[i] + r4, clt.getfd());
		}
	}

}