#include "../includes/Irc.hpp"

void	Server::cmdQuit(int i, std::vector<std::string> string_array)
{
	// std::cout << "cmdQuit" << " : "  << i << string_array[0] << std::endl;
	int fd = _fds[i].fd;
	/*
	std::string reason;
	Client *curr = getClient(fd);
	std::vector<Client> clients;
	for (size_t i = 1; i < string_array.size(); i++){
		reason += string_array[i];
	}
    for (std::vector<Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it)
    {
		clients = it->getClients();
		for (std::vector<Client>::iterator iter = clients.begin(); iter != clients.end(); ++it){
			if (curr->getNickname() == iter->getNickname()){
				it->broadcastMessage(USER_ID(curr->getNickname(), curr->getUsername()) \
				+ " QUIT :Quit: " + reason + "\r\n");
				break;
			}
		}
    }
	*/


	std::string msg;
	std::string reason = string_array.size() > 1 ? string_array[1] : "No reason provided";
	std::string sender = this->_clients[i - 1].getNickname() + "!" + this->_clients[i - 1].getUsername() + "@localhost";
	std::string nickQuit = this->_clients[i - 1].getNickname();
	Client* clientQuit = NULL;
    for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (it->getNickname() == nickQuit)
        {
            clientQuit = &(*it);
            break;
        }
    }
	Channel* channel = NULL;
	for (std::vector<Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		channel = &(*it);
		channel->quitClient(*clientQuit, reason);
	}
	CloseClients(fd);
    close(fd);
	std::cout<<PURPLE<<"Client["<< fd <<"]"<<RED<<" has disconnected"<<RESET<<std::endl;
}


void Channel::quitClient(Client &clientKicked, const std::string& reason)
{
	std::string msg;
	std::string sender = clientKicked.getNickname() + "!" + clientKicked.getUsername() + "@localhost";
	std::vector<Client>::iterator it = std::find(clients.begin(), clients.end(), clientKicked);
	if (it != clients.end())
	{
		clientKicked.removeChannel(*this);
        clients.erase(it);
		removeInvitedClient(clientKicked);
		removeOperator(clientKicked);
		int j = 0;
		while (j < static_cast<int>(clients.size()))
		{
			msg = ":" + sender + " QUIT :Quit " + reason + "\r\n";
			send(clients[j].getFd(), msg.c_str(), msg.length(), 0);
			j++;
		}
	}
}
