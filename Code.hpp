#ifndef CODE_HPP
# define CODE_HPP

#include "Server.hpp"
#include "Client.hpp"

class Server;
class Channel;
class Client;

class Code
{
public:
	Code();
	~Code();

	std::string Base(Client &clt, std::string nb);

	std::string RPL_WELCOME(Client &clt);
	std::string RPL_YOURHOST(Client &clt);
	std::string RPL_CREATED(Client &clt);
	std::string RPL_MYINFO(Client &clt);
	std::string RPL_ISUPPORT(Client &clt);
	std::string RPL_UMODEIS(Client &clt);
	std::string RPL_LISTSTART(Client &clt);
	std::string RPL_LIST(Client &clt, std::string name, int nb, std::string);
	std::string RPL_LISTEND(Client &clt);
	std::string RPL_CHANNELMODEIS(Client &clt, std::string title, std::string mods);
	std::string RPL_INVITING(Client &clt, std::string title);
	std::string RPL_ENDOFNAMES(Client &clt, std::string title);
	std::string RPL_NAMREPLY(Client &clt, Server &serv, std::string title);
	std::string RPL_ENDOFWHO(Client &clt);
	std::string RPL_NOTOPIC(Client &clt, std::string chan_name);
	std::string RPL_TOPIC(Client &clt, std::string chan_name, std::string topic);
	std::string RPL_YOUREOPER(Client &clt);

	// RPL_BOUNCE (010)
	// RPL_LUSERCLIENT (251)
	// RPL_LUSEROP (252)
	// RPL_LUSERUNKNOWN (253)
	// RPL_LUSERCHANNELS (254)
	// RPL_LUSERME (255)
	// RPL_ADMINME (256)
	// RPL_ADMINLOC1 (257)
	// RPL_ADMINLOC2 (258)
	// RPL_ADMINEMAIL (259)
	// RPL_TRYAGAIN (263)
	// RPL_LOCALUSERS (265)
	// RPL_GLOBALUSERS (266)
	// RPL_WHOISCERTFP (276)
	// RPL_NONE (300)
	// RPL_AWAY (301)
	// RPL_USERHOST (302)
	// RPL_UNAWAY (305)
	// RPL_NOWAWAY (306)
	// RPL_WHOREPLY (352)
	// RPL_WHOISREGNICK (307)
	// RPL_WHOISUSER (311)
	// RPL_WHOISSERVER (312)
	// RPL_WHOISOPERATOR (313)
	// RPL_WHOWASUSER (314)
	// RPL_WHOISIDLE (317)
	// RPL_ENDOFWHOIS (318)
	// RPL_WHOISCHANNELS (319)
	// RPL_WHOISSPECIAL (320)
	// RPL_CREATIONTIME (329)
	// RPL_WHOISACCOUNT (330)
	// RPL_TOPICWHOTIME (333)
	// RPL_INVITELIST (336)
	// RPL_ENDOFINVITELIST (337)
	// RPL_WHOISACTUALLY (338)
	// RPL_INVEXLIST (346)
	// RPL_ENDOFINVEXLIST (347)
	// RPL_EXCEPTLIST (348)
	// RPL_ENDOFEXCEPTLIST (349)
	// RPL_VERSION (351)
	// RPL_LINKS (364)
	// RPL_ENDOFLINKS (365)
	// RPL_BANLIST (367)
	// RPL_ENDOFBANLIST (368)
	// RPL_ENDOFWHOWAS (369)
	// RPL_INFO (371)
	// RPL_ENDOFINFO (std::string Code::RPL_NOTOPIC(Client &clt, std::string chan_name)374)
	// RPL_MOTDSTART (375)
	// RPL_MOTD (372)
	// RPL_ENDOFMOTD (376)
	// RPL_WHOISHOST (378)
	// RPL_WHOISMODES (379)
	// RPL_YOUREOPER (381)
	// RPL_REHASHING (382)
	// RPL_TIME (391)
	// RPL_HELPSTART (704)
	// RPL_HELPTXT (705)
	// RPL_ENDOFHELP (706)
	// RPL_LOGGEDIN (900)
	// RPL_LOGGEDOUT (901)
	// RPL_SASLSUCCESS (903)
	// RPL_SASLMECHS (908)

	std::string ERR_NEEDMOREPARAMS(std::string, Client &);
	std::string ERR_NONICKNAMEGIVEN(Client &clt);
	std::string ERR_NICKNAMEINUSE(std::string &nick, Client &clt);
	std::string ERR_ALREADYREGISTERED(Client &clt);
	std::string ERR_PASSWDMISMATCH(Client &clt);
	std::string ERR_NOTREGISTERED(Client &clt);
	std::string ERR_UMODEUNKNOWNFLAG(Client &clt);
	std::string ERR_NOSUCHCHANNEL(Client &clt, std::string name);
	std::string ERR_NOTONCHANNEL(Client &clt, std::string name);
	std::string ERR_CHANOPRIVSNEEDED(Client &clt, std::string title);
	std::string ERR_NOSUCHNICK(Client &clt);
	std::string ERR_USERONCHANNEL(Client &clt, std::string name);
	std::string ERR_UNKNOWNCOMMAND(std::string cmd, Client &clt);
	std::string	ERR_TOOMANYTARGETS(Client &clt, std::string channel);
	std::string	ERR_CANNOTSENDTOCHAN(Client &clt, std::string channel);
	std::string	ERR_NOTEXTTOSEND(Client &clt);
	std::string ERR_INVALIDCHANNELNAME(Client &clt, std::string name);
	std::string ERR_USERNOTINCHANNEL(Client &clt, std::string target, std::string channel);
	std::string ERR_NOORIGIN(Client &clt);
	std::string	ERR_BADCHANNELKEY(Client &clt, std::string name);
	std::string ERR_UNKNOWNMODE(Client &clt, std::string mods);
	std::string ERR_NOOPERHOST(Client &clt);
	std::string ERR_NOPRIVILEGES(Client &clt);
	std::string ERR_USERSDONTMATCH(Client &clt);
	std::string ERR_INVITEONLYCHAN(Client &clt, std::string name);
	std::string ERR_TOOMANYCHANNELS(Client &clt, std::string titlechan);

	// std::string ERR_NOMOTD(std::string, Client &);
	// std::string ERR_ERRONEUSNICKNAME(std::string, Client &);
	// std::string ERR_NICKCOLLISION(std::string, Client &);
	// ERR_UNKNOWNERROR (400)
	// ERR_NOSUCHSERVER (402)
	// ERR_TOOMANYCHANNELS (405)
	// ERR_WASNOSUCHNICK (406)
	// ERR_NOORIGIN (409)
	// ERR_INPUTTOOLONG (417)
	// ERR_NOMOTD (422)
	// ERR_YOUREBANNEDCREEP (465)
	// ERR_BANNEDFROMCHAN (474)
	// ERR_BADCHANMASK (476)
	// ERR_NOPRIVILEGES (481)
	// ERR_CANTKILLSERVER (483)
	// ERR_HELPNOTFOUND (524)
	// ERR_INVALIDKEY (525)
	// ERR_NOPRIVS (723)
	// ERR_NICKLOCKED (902)
	// ERR_SASLFAIL (904)
	// ERR_SASLTOOLONG (905)
	// ERR_SASLABORTED (906)
	// ERR_SASLALREADY (907)
};

#endif