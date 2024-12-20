#include "../includes/Irc.hpp"

void	Server::cmdKick(int i, std::vector<std::string> string_array)
{
	// if (this->isRegistered(i) == 0)
	// {
	// 	this->_clients[i - 1].sendMessage(":localhost 451 :You have not registered\r\n");
	// 	return ;
	// }
	// std::cout << "cmdKick" << " : " << string_array[0] << std::endl;
	// if (string_array.size() < 3)
	// {
	// 	this->_clients[i - 1].sendMessage(":localhost 461 " + _clients[i - 1].getNickname() + " KICK :Not enough parameters\r\n");
	// 	return ;
	// }
	std::string channelName = string_array[1];
    std::string targetNickname = string_array[2];
	std::string reason = string_array.size() > 3 ? string_array[3] : "No reason provided.";
	Channel* channel = NULL;
    for (std::vector<Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it)
    {
        if (it->getName() == channelName)
        {
            channel = &(*it);
            break;
        }
    }

    // if (channel == NULL)
	// {
	// 	this->_clients[i - 1].sendMessage(":localhost 403 " + _clients[i - 1].getNickname() + " " +  channelName + " :No such channel\r\n");
	// 	return ;
	// }
	Client operatorClient = _clients[i - 1];
	// if (!channel->isOperator(operatorClient))
	// {
	// 	operatorClient.sendMessage(":localhost 482 " + _clients[i - 1].getNickname() + " " + channelName + " :You're not an IRC operator\r\n");
    //     return ;
	// }
	 Client* targetClient = NULL;

    for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (it->getNickname() == targetNickname)
        {
            targetClient = &(*it);
            break;
        }
    }

    // if (targetClient == NULL)
	// {
	// 	operatorClient.sendMessage(":localhost 401 " + _clients[i - 1].getNickname() + " " + targetNickname + " :No such nick/channel\r\n");
    //     return ;
	// }
	// if (!channel->isClientInChannel(*targetClient))
	// {
	// 	operatorClient.sendMessage(":localhost 441 " + _clients[i - 1].getNickname() + " " + targetNickname + " " + channelName + " :They aren't on that channel" + "\r\n");
    //     return ;
	// }//for irssi
	std::string kickMessage = ":" + operatorClient.getNickname() + "!" + operatorClient.getUsername() + "@" + operatorClient.getHostname() +
							  " KICK " + channelName + " " + targetNickname ;
	// targetClient->sendMessage(kickMessage);
	// std::cout << kickMessage << std::endl;
	// channel->broadcastMessage(kickMessage);

	int j = 0;
	while (j < static_cast<int>(channel->getClients().size()))
	{
		// if (this->_clients[i - 1].getNickname() != (*it).getClients()[j].getNickname())
		// {
		std::string msg = ":" + operatorClient.getNickname() + "!" + operatorClient.getUsername() + "@localhost KICK " + string_array[1] + " " + string_array[2] + "\r\n";
		send(channel->getClients()[j].getFd(), msg.c_str(), msg.length(), 0);
		// }
		j++;
	}

	channel->kickClient(*targetClient);

	std::cout << "Client " << targetNickname << " has been kicked from channel " << channelName
			  << " by operator " << operatorClient.getNickname() << ". Reason: " << (reason.empty() ? "No reason" : reason) << std::endl;
}

void Channel::kickClient(Client &client)
{
	std::vector<Client >::iterator it = std::find(clients.begin(), clients.end(), client);
	if (it != clients.end())
	{
		clients.erase(it);
		client.removeChannel(*this);
		std::cout << "Client " << client.getNickname() << " has been removed from channel " << name << std::endl;
	}
	else
	{
		std::cerr << "Client not found in channel " << name << "!" << std::endl;
	}
}
