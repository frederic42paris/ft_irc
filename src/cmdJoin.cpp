#include "../includes/Irc.hpp"

void	Server::cmdJoin(int i, std::vector<std::string> string_array)
{
	if (this->_clients[i - 1].getIsRegistered() == 0)
	{
		std::string msg = "Error : Client is not registered\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
		return ;
	}

	if (string_array.size() < 2)
    {
        this->_clients[i - 1].sendMessage("[Error 461] JOIN : Not enough parameters\n");
        return;
    }

    std::string channelName = string_array[1];
    std::string pwd = (string_array.size() > 2) ? string_array[2] : "";
    if (!isValidChannelName(channelName))
    {
        this->_clients[i - 1].sendMessage("[Error 403] " + channelName + " : Invalid channel name\n");
        return;
    }

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
        std::cout << "Channel " << channelName << " not found. Creating new channel." << std::endl;
        this->_channels.push_back(Channel(channelName));
        channel = &_channels.back();
        channel->addClient(this->_clients[i - 1]);
        channel->addOperator(this->_clients[i - 1]);
        std::cout << "Client " << this->_clients[i - 1].getNickname() << " added as operator to channel " << channelName << std::endl;
        
        std::string msg = ":" + this->_clients[i - 1].getNickname() + " JOIN " + string_array[1] + "\r\n";
        
        std::cout << msg << std::endl;
        send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);

        std::string msg2 = ":localhost 353 " + this->_clients[i - 1].getNickname() + " = " + string_array[1] + " :@" + this->_clients[i - 1].getNickname() + "\r\n";
        std::cout << msg2 << std::endl;
        send(this->_clients[i - 1].getFd(), msg2.c_str(), msg2.length(), 0);

        std::string msg3 = ":localhost 366 " + this->_clients[i - 1].getNickname() + " " + string_array[1] + " :End of /NAMES list\r\n";
        std::cout << msg3 << std::endl;
        send(this->_clients[i - 1].getFd(), msg3.c_str(), msg3.length(), 0);
    }
    else
    {
        std::cout << "Channel " << channelName << " found. Joining channel." << std::endl;
        channel->joinChannel(this->_clients[i - 1], pwd);
    }

	// Channel channel = findChannelByName(channelName);
    // if (channel == Channel())
    // {
    //     // Channel newChannel(channelName);
    //     // newChannel.addClient(&this->_clients[i - 1]);
    //     // newChannel.addOperator(&this->_clients[i - 1]);
    //     // _channels.push_back(newChannel);
    //     // channel = &_channels.back();
    //     std::cout << "Channel " << channelName << " not found. Creating new channel." << std::endl;
    //     this->_channels.push_back(Channel(channelName));
    //     channel = _channels.back();
    //     channel.addClient(this->_clients[i - 1]);
    //     channel.addOperator(this->_clients[i - 1]);
    //     std::cout << "Client " << this->_clients[i - 1].getName() << " added as operator to channel " << channelName << std::endl;
    //     // std::cout << "Client address in cmdJoin: " << &this->_clients[i - 1] << std::endl;
    // }
    // else
    // {
    //     std::cout << "Channel " << channelName << " found. Joining channel." << std::endl;
    //     channel.joinChannel(this->_clients[i - 1], pwd);
    // }
}
