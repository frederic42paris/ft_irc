#include "../includes/Irc.hpp"

void	Server::cmdQuit(int i)
{
	int fd = _fds[i].fd;
	CloseClients(fd);
    close(fd);
	std::cout<<PURPLE<<"Client["<< fd <<"]"<<RED<<" has disconnected"<<RESET<<std::endl;
}
