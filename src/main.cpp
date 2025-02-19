#include "../includes/Irc.hpp"

int main(int ac, char **av){
	if (ac != 3){
		std::cout << "Error: input format: ./ircserv <port> <password>" << std::endl;
		return 1; // handle return msg
	}
	std::string str(av[2]);
	Server server(std::atoi(av[1]), str);
	try {
    signal(SIGINT, Server::SigHandler);
    signal(SIGQUIT, Server::SigHandler);
		server.InitServer();
	}
	catch (ErrThrow &e){
		server.CloseServer();
		std::cout<<BRED<<"Server error: "<<YELLOW<<e.what()<<RESET<<std::endl;
	}
	(void)av;
	(void)ac;
	return 0;
}

