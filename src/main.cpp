#include "../includes/Irc.hpp"

int close_and_ret(std::string msg, int srv, int cli){
	if (srv > 0)
		close(srv);
	if (cli > 0)
		close(cli);
	std::cout << YELLOW << msg << RED << " error" << RESET << std::endl;
	return 1;
}

int main(int ac, char **av){
	if (ac != 3){
		std::cout << "Error: input format: ./ircserv <port> <password>" << std::endl;
		return 1;
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

