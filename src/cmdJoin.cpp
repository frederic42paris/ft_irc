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
		std::string msg = ":localhost 461 " + this->_clients[i - 1].getNickname() + " JOIN :Not enough parameters\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
        return;
    }

    std::string channelName = string_array[1];
    std::string pwd = (string_array.size() > 2) ? string_array[2] : "";
    std::cout << "ICI :" <<  channelName << std::endl;
    if (isValidChannelName(channelName) == 0)
    {
        this->_clients[i - 1].sendMessage("Error : " + channelName + " : Invalid channel name\n");
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
        
		std::string msg = ":" + this->_clients[i - 1].getNickname() + "!" + this->_clients[i - 1].getUsername() + "@localhost" +  " JOIN " + string_array[1] + "\r\n";
        std::cout << msg;
        send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);

		std::string msg2 = "353 " + this->_clients[i - 1].getNickname() + "!" + this->_clients[i - 1].getUsername() + "@localhost " + "= " + string_array[1] + " :@" + this->_clients[i - 1].getNickname() + "\r\n";
        std::cout << msg2;
        send(this->_clients[i - 1].getFd(), msg2.c_str(), msg2.length(), 0);

		std::string msg3 = "366 " + this->_clients[i - 1].getNickname() + "!" + this->_clients[i - 1].getUsername() + "@localhost " + string_array[1] + " :End of NAMES list\r\n";
        std::cout << msg3;
        send(this->_clients[i - 1].getFd(), msg3.c_str(), msg3.length(), 0);
    }
    else
    {
        std::cout << "Channel " << channelName << " found. Joining channel." << std::endl;
        channel->joinChannel(this->_clients[i - 1], pwd);
    }
}

void Channel::joinChannel(Client &client, const std::string& password)
{
    if (std::find(clients.begin(), clients.end(), client) != clients.end())
    {
         for (std::size_t k = 0; k < clients.size(); ++k)
		{
			std::string joinMsg = ":" + client.getNickname() + "!" + client.getUsername() + "@localhost" + " JOIN " + name + "\r\n";
			std::cout << joinMsg;
			send(clients[k].getFd(), joinMsg.c_str(), joinMsg.length(), 0);
		}
		if (getTopic().size() != 0)
		{
			std::string endNamesMsg = ":localhost 332 " + client.getNickname() + " " + name + " " + getTopic() + "\r\n";
			std::cout << endNamesMsg;
			send(client.getFd(), endNamesMsg.c_str(), endNamesMsg.length(), 0);
		}
		std::string namesList = "353 " + client.getNickname() + "!" + client.getUsername() + "@localhost " + "= " + name + " :";
		
		for (std::size_t j = 0; j < clients.size(); ++j)
		{
			if (j > 0)
				namesList += " ";
			if (isOperator(clients[j]))
				namesList += "@";
			namesList += clients[j].getNickname();
		}
		namesList += "\r\n";
		std::cout << namesList;
		send(client.getFd(), namesList.c_str(), namesList.length(), 0);

		std::string endNamesMsg = "366 " + client.getNickname() + "!" + client.getUsername() + "@localhost " + name + " :End of NAMES list\r\n";
		std::cout << endNamesMsg;
		send(client.getFd(), endNamesMsg.c_str(), endNamesMsg.length(), 0);
			return;
    }

    if (inviteOnly == true && client.isInvited(client, *this) == false)
    {
        std::string formattedMessage = ":localhost 473 " + client.getNickname() + " " + name + " :Cannot join channel (+i)\r\n";
        std::cout << "Sending message to client_fd " << client.getFd() << ": " << formattedMessage << std::endl;
        if (send(client.getFd(), formattedMessage.c_str(), formattedMessage.length(), 0) == -1)
            perror("send");
        else
            std::cout << "Message sent successfully." << std::endl;
        std::cout << "Client " << client.getNickname() << " needs an invitation to join channel\r\n" << name << std::endl;
        return;
    }

    if (!Ch_pwd.empty() && Ch_pwd != password)
    {
        client.sendMessage(":localhost 475 " + client.getNickname() + " " + name + " :Cannot join channel (+k)\r\n");
        std::cout << "Client " << client.getNickname() << " provided incorrect password for channel\r\n" << name << std::endl;
        return;
    }

    if (userLimits > 0 && clients.size() >= static_cast<std::size_t>(userLimits))
    {
        client.sendMessage(":localhost 471 " + client.getNickname() + " " + name + " :Cannot join channel (+l)\r\n");
        std::cout << "Channel " << name << " is full, client " << client.getNickname() << " cannot join" << std::endl;
        return;
    }
    clients.push_back(client);
    client.addChannel(*this);

    for (std::size_t k = 0; k < clients.size(); ++k)
    {
		std::string joinMsg = ":" + client.getNickname() + "!" + client.getUsername() + "@localhost" + " JOIN " + name + "\r\n";
        std::cout << joinMsg;
        send(clients[k].getFd(), joinMsg.c_str(), joinMsg.length(), 0);
    }
	if (getTopic().size() != 0)
	{
		std::string endNamesMsg = ":localhost 332 " + client.getNickname() + " " + name + " " + getTopic() + "\r\n";
		std::cout << endNamesMsg;
		send(client.getFd(), endNamesMsg.c_str(), endNamesMsg.length(), 0);
	}
	std::string namesList = "353 " + client.getNickname() + "!" + client.getUsername() + "@localhost " + "= " + name + " :";
    
    for (std::size_t j = 0; j < clients.size(); ++j)
    {
        if (j > 0)
            namesList += " ";
		if (isOperator(clients[j]))
			namesList += "@";
        namesList += clients[j].getNickname();
    }
    namesList += "\r\n";
    std::cout << namesList;
    send(client.getFd(), namesList.c_str(), namesList.length(), 0);

    std::string endNamesMsg = "366 " + client.getNickname() + "!" + client.getUsername() + "@localhost " + name + " :End of NAMES list\r\n";
	std::cout << endNamesMsg;
    send(client.getFd(), endNamesMsg.c_str(), endNamesMsg.length(), 0);

    std::cout << "Client " << client.getNickname() << " successfully joined channel " << name << std::endl;
}
