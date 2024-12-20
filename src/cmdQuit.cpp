#include "../includes/Irc.hpp"

void	Server::cmdQuit(int i)
{
	// std::cout << "cmdQuit" << " : "  << i << string_array[0] << std::endl;
	int fd = _fds[i].fd;
	// std::string reason;
	// Client *curr = getClient(fd);
	// std::vector<Client> clients;
	// for (size_t i = 1; i < string_array.size(); i++){
	// 	reason += string_array[i];
	// }
    // for (std::vector<Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it)
    // {
	// 	clients = it->getClients();
	// 	for (std::vector<Client>::iterator iter = clients.begin(); iter != clients.end(); ++it){
	// 		if (curr->getNickname() == iter->getNickname()){
				// it->broadcastMessage(USER_ID(curr->getNickname(), curr->getUsername())
	// 			+ " QUIT :Quit: " + reason + "\r\n");
	// 			break;
	// 		}
	// 	}
    // }
	CloseClients(fd);
    close(fd);
	std::cout<<PURPLE<<"Client["<< fd <<"]"<<RED<<" has disconnected"<<RESET<<std::endl;
	// (void)i;
	// (void)string_array;
}
