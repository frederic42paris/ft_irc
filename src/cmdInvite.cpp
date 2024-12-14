#include "../includes/Irc.hpp"

void	Server::cmdInvite(int i, std::vector<std::string> string_array)
{
	if (this->isRegistered(i) == 0)
	{
		this->_clients[i - 1].sendMessage(":localhost 451  :You have not registered");
		return ;
	}

    if (string_array.size() < 3)
    {
        this->_clients[i - 1].sendMessage(":localhost 461 INVITE :Not enough parameters");
        return;
    }

    std::string targetNickname = string_array[1];
    std::string channelName = string_array[2];

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
        this->_clients[i - 1].sendMessage(":localhost 403 " + channelName + " :No such channel");
        return;
    }

    // Channel channel = findChannelByName(channelName);
    // if (channel == Channel())
    // {
    //     this->_clients[i - 1].sendMessage(":localhost 403 " + channelName + " :No such channel");
    //     return;
    // }

    std::cout << "Checking if client " << this->_clients[i - 1].getName() << " is an operator in channel " << channelName << std::endl;
    // std::cout << "Client address in cmdInvite: " << &this->_clients[i - 1] << std::endl;
    if (!channel->isOperator(this->_clients[i - 1]))
    {
        this->_clients[i - 1].sendMessage(":localhost 482 " + channelName + " :You're not a channel operator");
        return;
    }

    Client targetClient = findClientByNickname(targetNickname);
    if (targetClient == Client())
    {
        this->_clients[i - 1].sendMessage(":localhost 401 " + targetNickname + " :No such nick/channel");
        return;
    }
    channel->inviteClient(targetClient);
    this->_clients[i - 1].sendMessage(":localhost 341 " + this->_clients[i - 1].getNickname() + " " + targetNickname + " " + channelName);
    targetClient.sendMessage(":localhost :You have been invited to join " + channelName + " by " + this->_clients[i - 1].getNickname());
}
