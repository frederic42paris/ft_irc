#include "../includes/Irc.hpp"

void	Server::cmdPart(int i, std::vector<std::string> string_array) //parameter: 1channel, reason
{
	if (this->isRegistered(i) == 0)
	{
		std::cout << "Client not registered"  << std::endl;
		return ;
	}
	if (string_array.size() < 2) {
        this->_clients[i - 1].sendMessage(":localhost 461 " + _clients[i - 1].getNickname() + " PART :Not enough parameters\r\n");
        return;
    }
	std::string &channelName = string_array[1]; //for now only 1 channel as parameter allowed
	std::string reason = string_array.size() > 2 ? string_array[2] : "";
	Channel* channel = NULL;
    for (std::vector<Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it)
    {
        if (it->getName() == channelName)
        {
            channel = &(*it);
            break;
        }
    }

    if (channel == NULL)
	{
		this->_clients[i - 1].sendMessage(":localhost 403 " + _clients[i - 1].getNickname() + " " +  channelName + " :No such channel\r\n");
		return ;
	}
	if (!channel->isClientInChannel(_clients[i - 1]))
	{
		this->_clients[i - 1].sendMessage(":localhost 442 " + _clients[i - 1].getNickname() + " " + channelName + " :You aren't on that channel\r\n");
		return ;
	}
	channel->partChannel(_clients[i - 1], reason);
	channel->broadcastMessage(
    USER_ID(_clients[i - 1].getNickname(), _clients[i - 1].getUsername()) + " PART " + channelName + (reason.empty() ? "\r\n" : " :" + reason + "\r\n"),
    _clients[i - 1]);


	if (channel->isEmpty())
	{
		for (std::vector<Channel>::iterator it = this->_channels.begin(); it != this->_channels.end(); ) 
		{
    		if ((*it) == *channel)
			{
        		it = this->_channels.erase(it);
        		break;
    		}
			else
				++it;
		}
	}
	std::cout << "Client "<< _clients[i - 1].getNickname() << " parted from channel " << channelName << std::endl;
}

void Channel::partChannel(Client& client, const std::string& reason)
{
    std::vector<Client>::iterator it = std::find(clients.begin(), clients.end(), client);
    if (it != clients.end())
    {
		std::string Reason = reason.empty() ? "No reason provided" : reason;

        if (client.getNickname().empty() || client.getUsername().empty()) {
            std::cerr << "Error: Client " << client.getNickname() << " has invalid nickname or username.\n";
            return;
        }
        client.removeChannel(*this);
        client.sendMessage("You have parted from channel " + name + "\n");
        client.sendMessage(USER_ID(client.getNickname(), client.getUsername()) 
                   + " PART " + name 
                   + (reason.empty() ? "\r\n" : " :" + reason + "\r\n"));
        std::cout << "Client " << client.getNickname() << " has parted from channel " << name << std::endl;
        clients.erase(it);
		removeInvitedClient(client);
    }
}
