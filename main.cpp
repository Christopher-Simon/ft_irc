#include "irc.hpp"
#include "Server.hpp"

//utiliser un container map et ne pas distinguer les nickname du style Dan et dan

int	main(int argc, char **argv)
{
	if (argc != 2)
		std::cerr << RED << "Wrong argument number." << RESET << std::endl;

	try {
		Server	serv(argv[1]);
	} catch (std::exception & e) {
		std::cerr << RED << e.what() << RESET << std::endl;
	}
	return (0);
}
