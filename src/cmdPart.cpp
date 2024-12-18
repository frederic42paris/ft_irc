#include "../includes/Irc.hpp"

void	Server::cmdPart(int i, std::vector<std::string> string_array) //parameter: 1channel, reason
{
	std::string msg;
	if (this->_clients[i - 1].getIsRegistered() == 0)
	{
		msg = "Error : Client is not registered\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
		return ;
	}
	if (string_array.size() < 2) {
        this->_clients[i - 1].sendMessage(":localhost 461 " + _clients[i - 1].getNickname() + " PART :Not enough parameters\r\n");
        return;
    }
	// std::string channelName = string_array[1];
	// std::string &channelName = string_array[1]; //for now only 1 channel as parameter allowed
	// std::string reason = string_array.size() > 2 ? string_array[2] : "";
	// if (!isValidChannelName(channelName))
	// {
	// 	this->_clients[i - 1].sendMessage(":localhost 403 " + _clients[i - 1].getNickname() + " " + channelName + " :No such channel\r\n");
	// 	return ;
	// }
	// Channel channel = findChannelByName(channelName);
	// Channel* channel = NULL;
    // for (std::vector<Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it)
    // {
    //     if (it->getName() == channelName)
    //     {
    //         // channel = &(*it);
    //         break;
    //     }
    // }


	int k = 0;
	while (k < static_cast<int>(getChannels().size()))
	{
		if (getChannels()[k].getName() == string_array[1])
			break;
		k++;
	}
	// std::cout << "ICI: " << k << " " << getChannels().size() << std::endl;

	if (k == static_cast<int>(getChannels().size()))
	{
		msg = ":localhost 403 " + this->_clients[i - 1].getNickname() + " " + string_array[1] + " :No such channel\r\n";
		std::cout << msg << std::endl;
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
		return;
	}

	std::cout << "ICI: " << k << " " << getChannels()[k].getClients().size() << std::endl;

	// int f = 0;
	// while (f < static_cast<int>(getChannels().size()))
	// {
	// 	if (getChannels()[k].getName() == string_array[1])
	// 		break;
	// 	k++;
	// }

	std::cout << getChannels()[k].getClients().size() << std::endl;

	int p = 0;
	while (p < static_cast<int>(getChannels()[k].getClients().size()))
	{
		// std::cout << getChannels()[k].getClients()[p].getNickname() << " vs " <<  this->_clients[i - 1].getNickname() << std::endl;
		if (getChannels()[k].getClients()[p].getNickname() == this->_clients[i - 1].getNickname())
		{
			getChannels()[k].getClients().erase(getChannels()[k].getClients().begin() + p);
		}
		p++;
	}

	std::cout << getChannels()[k].getClients().size() << std::endl;
	// int p = 0;
	// while (p < static_cast<int>(getChannels()[k].getClients().size()))
	// {
	// 	// std::cout << getChannels()[k].getClients()[p].getNickname() << " vs " <<  this->_clients[i - 1].getNickname() << std::endl;
	// 	if (getChannels()[k].getClients()[p].getNickname() != this->_clients[i - 1].getNickname())
	// 	{
	// 		// std::cout << "ICI: " << getChannels()[k].getClients()[p].getNickname() << std::endl;
	// 		msg = ":" + _clients[i - 1].getNickname() + "!" + _clients[i - 1].getUsername() + "@localhost PART " + string_array[1] + " :the client has left the channel\r\n";
	// 		std::cout << msg << std::endl;
	// 		send(this->_clients[p - 1].getFd(), msg.c_str(), msg.length(), 0);
	// 	}
	// 	p++;
	// }

	// for (std::vector<Channel >::iterator it = this->_channels.begin(); it != _channels.end(); it++)
	// {
	// 	if ((*it).getName() == string_array[1])
	// 	{
	// 		break;
	// 	}
	// }

	// if (k == static_cast<int>((*it).getClients().size()))
	// {
	// 	msg = ":localhost 404 " + this->_clients[i - 1].getNickname() + " " + string_array[1] + " :Cannot send to channel\r\n";
	// 	send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
	// 	return;
	// }




	// for (std::vector<Channel >::iterator it = this->_channels.begin(); it != _channels.end(); it++)
	// {
	// 	if ((*it).getName() == string_array[1])
	// 	{
	// 		int k = 0;
	// 		while (k < static_cast<int>((*it).getClients().size()))
	// 		{
	// 			if ((*it).getClients()[k].getNickname() == this->_clients[i - 1].getNickname())
	// 				break;
	// 			k++;
	// 		}
	// 		if (k == static_cast<int>((*it).getClients().size()))
	// 		{
	// 			msg = ":localhost 442 " + this->_clients[i - 1].getNickname() + " " + string_array[1] + " :You're not on that channel\r\n";
	// 			send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
	// 			return;
	// 		}
	// 		else
	// 		{
	// 			(*it).getClients()[k].removeChannel(string_array[1]);
				
	// 			int j = 0;
	// 			while (j < static_cast<int>((*it).getClients()[k]))
	// 			{
	// 					std::cout << (*it).getClients()[j].getNickname() << std::endl;
	// 				j++;
	// 			}
				
	// 			(*it).getClients().erase(getClients().begin() + k);
				
	// 			int j = 0;
	// 			while (j < static_cast<int>((*it).getClients().size()))
	// 			{
	// 					std::cout << (*it).getClients()[j].getNickname() << std::endl;
	// 				j++;
	// 			}

	// 			// int j = 0;
	// 			// while (j < static_cast<int>((*it).getClients().size()))
	// 			// {
	// 			// 	// if (this->_clients[i - 1].getNickname() != (*it).getClients()[j].getNickname())
	// 			// 	// {
	// 			// 		msg = ":" + _clients[i-1].getNickname() + "!" + _clients[i-1].getUsername() + "@localhost PART " + string_array[1] + " :the client has left the channel\r\n";
	// 			// 		std::cout << msg << std::endl;
	// 			// 		send(this->_clients[j].getFd(), msg.c_str(), msg.length(), 0);
	// 			// 	// }
	// 			// 	j++;
	// 			// }
	// 		}
	// 		// int j = 0;
	// 		// while (j < static_cast<int>((*it).getClients().size()))
	// 		// {
	// 		// 	if (this->_clients[i - 1].getNickname() != (*it).getClients()[j].getNickname())
	// 		// 	{
	// 		// 		msg = ":" + this->_clients[i - 1].getNickname() + "!" + this->_clients[i - 1].getUsername() + "@localhost PRIVMSG " + string_array[1] + " " + message + "\r\n";
	// 		// 		send((*it).getClients()[j].getFd(), msg.c_str(), msg.length(), 0);
	// 		// 	}
	// 		// 	j++;
	// 		// }
	// 	}
	// }




    // if (channel == NULL)
	// {
	// 	this->_clients[i - 1].sendMessage(":localhost 403 " + _clients[i - 1].getNickname() + " " +  channelName + " :No such channel\r\n");
	// 	return ;
	// }
	// if (!channel->isClientInChannel(_clients[i - 1]))
	// {
	// 	this->_clients[i - 1].sendMessage(":localhost 442 " + _clients[i - 1].getNickname() + " " + channelName + " :You aren't on that channel\r\n");
	// 	return ;
	// }
	// channel->partChannel(_clients[i - 1], reason); //handle in channel
	// if (channel->isEmpty())
	// {
	// 	for (std::vector<Channel>::iterator it = this->_channels.begin(); it != this->_channels.end(); ) 
	// 	{
    // 		if ((*it) == *channel)
	// 		{
    //     		it = this->_channels.erase(it);
    //     		break;
    // 		}
	// 		else
	// 			++it;
	// 	}
	// }
	// std::cout << "Client "<< _clients[i - 1].getNickname() << " parted from channel " << channelName << std::endl;
}

void Channel::partChannel(Client& client, const std::string& reason)
{
    std::vector<Client>::iterator it = std::find(clients.begin(), clients.end(), client);
    if (it != clients.end())
    {
        client.removeChannel(*this);
        client.sendMessage("You have parted from channel " + name + "\n");
		client.sendMessage(USER_ID(client.getNickname(), client.getUsername()) + " PART " + name + " :" + reason + "\r\n");

        // Debug statement to check client after parting
        std::cout << "Client " << client.getNickname() << " has parted from channel " << name << std::endl;
        clients.erase(it);
    }
    else
    {
        std::cerr << "Error: Client " << client.getNickname() << " not found in channel " << name << std::endl;
    }
}
