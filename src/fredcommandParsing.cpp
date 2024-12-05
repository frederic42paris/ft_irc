#include "../includes/Irc.hpp"

// FRED PART --------------------------------------------------------

void printvector (std::string str)
{
  std::cout << str << std::endl;
}

void	Server::commandParsing(int i, std::string string)
{
	std::string buffer = string;
	int length = buffer.size();
	buffer.resize(length - 1);
	std::stringstream ss(buffer);
	std::string tmp;
	std::vector<std::string> string_array;

	while (std::getline(ss, tmp, ' '))
	{
		string_array.push_back(tmp);
	}

	if (string_array.empty()) // empty command
	{
		std::cerr << "Error: Empty command received" << std::endl;
		return;
	}

	std::string cmd_array[] = {
		"/NICK",
		"/PASS",
		"/USER",
		"/QUIT",
		"/PRIVMSG",
		"/JOIN",
		"/PART",
		"/KICK",
		"/INVITE",
		"/TOPIC",
		"/MODE"
	};

	int len = sizeof(cmd_array) / sizeof(cmd_array[0]);
	int j = 0;
	while (j < len)
	{
		if (cmd_array[j] == string_array[0])
			break;
		j++;
	}

	if (j >= len)
    {
        std::cerr << "Error: Unrecognized command [" << string_array[0] << "]." << std::endl;
        return;
    }
	
	if (string_array.size() > 1)
    	std::cout << YELLOW << string_array[1] << RESET << std::endl;
	else
	{
    	string_array.push_back("");
	}

	switch(j)
	{
		case 0: this->cmdNick(i, string_array); break;
		case 1: this->cmdPass(i, string_array); break;
		case 2: this->cmdUser(i, string_array); break;
		case 3: this->cmdQuit(i, string_array); break;
		case 4: this->cmdPrivmsg(i, string_array, string); break;
		case 5: this->cmdJoin(i, string_array); break;
		case 6: this->cmdPart(i, string_array); break;
		case 7: this->cmdKick(i, string_array); break;
		case 8: this->cmdInvite(i, string_array); break;
		case 9: this->cmdTopic(i, string_array); break;
		case 10: this->cmdMode(i, string_array); break;
    default : break;
	}
}

int		Server::isRegistered(int i)
{
	if (this->_clients[i - 1].getNickname().empty()
		|| this->_clients[i - 1].getUsername().empty()
		|| this->_clients[i - 1].getPasswordIsCorrect() == 0)
	{
		return (0);
	}
	else
	{
		return (1);
	}
}

// YUMI PART --------------------------------------------------------

Channel* Server::findChannelByName(const std::string& channelName)
{
    for (size_t j = 0; j < this->_channels.size(); j++)
    {
        if (this->_channels[j]->getName() == channelName)
            return (this->_channels[j]);
    }
    return (NULL);
}


Client* Server::findClientByNickname(const std::string& nickname)
{
    for (size_t j = 0; j < this->_clients.size(); j++)
	{
        if (this->_clients[j].getNickname() == nickname)
            return (&this->_clients[j]);
    }
    return NULL;
}

bool Server::isValidChannelName(const std::string& channelName)
{
    // start with # and no space
    return !channelName.empty() && channelName[0] == '#' && channelName.find(' ') == std::string::npos;
}

void	Server::cmdJoin(int i, std::vector<std::string> string_array) // parameter: 1channel, pwd
{
    if (this->isRegistered(i) == 0)
    {
        std::cout << "Client not registered" << std::endl;
        return;
    }

    std::cout << "cmdJoin: " << string_array[0] << std::endl;

    if (string_array.size() < 2)
    {
        this->_clients[i - 1].sendMessage("[Error 461] JOIN : Not enough parameters\n");
        return;
    }

    std::string channelName = string_array[1];
    std::string pwd = (string_array.size() > 2) ? string_array[2] : ""; // 預設密碼為空

    if (!isValidChannelName(channelName))
    {
        this->_clients[i - 1].sendMessage("[Error 403] " + channelName + " : Invalid channel name\n");
        return;
    }

    Channel *channel = findChannelByName(channelName);
    if (!channel)
    {
        channel = new Channel(channelName, *this);
        channel->addOperator(&this->_clients[i - 1]);
        this->_channels.push_back(channel);
    }
    channel->joinChannel(&this->_clients[i - 1], pwd);
}

void	Server::cmdPart(int i, std::vector<std::string> string_array) //parameter: 1channel, reason
{
	if (this->isRegistered(i) == 0)
	{
		std::cout << "Client not registered"  << std::endl;
		return ;
	}
	if (string_array.size() < 2) {
        this->_clients[i - 1].sendMessage(":server 461 PART :Not enough parameters\n");
        return;
    }
	std::string &channelName = string_array[1]; //for now only 1 channel as parameter allowed
	std::string reason = string_array.size() > 2 ? string_array[2] : "";
	if (!isValidChannelName(channelName))
	{
		this->_clients[i - 1].sendMessage(":server 403 " + channelName + " :No such channel\n");
		return ;
	}
	Channel *channel = findChannelByName(channelName);
	if (!channel)
	{
		this->_clients[i - 1].sendMessage(":server 403 " + channelName + " :No such channel\n");
		return ;
	}
	if (!channel->isClientInChannel(&_clients[i - 1]))
	{
		this->_clients[i - 1].sendMessage(":server 442 " + channelName + " :You are not on that channel\n");
		return ;
	}
	channel->partChannel(&_clients[i], reason); //handle in channel
	if (channel->isEmpty())
	{
		for (std::vector<Channel*>::iterator it = this->_channels.begin(); it != this->_channels.end(); ) 
		{
    		if (*it == channel)
			{
        		this->_channels.erase(it);
        		break;
    		}
			else
				++it;
		}
		delete channel;
	}
	std::cout << "Client[" << i - 1 << "] parted from channel " << channelName << std::endl;
}

//

void	Server::cmdKick(int i, std::vector<std::string> string_array)
{
	if (this->isRegistered(i) == 0)
	{
		std::cout << "Client not registered"  << std::endl;
		return ;
	}
	std::cout << "cmdKick" << " : " << string_array[0] << std::endl;
	if (string_array.size() < 3)
	{
		this->_clients[i - 1].sendMessage("Error: KICK command requires at least 2 arguments: <channel> <nickname>");
		return ;
	}
	std::string channelName = string_array[1];
    std::string targetNickname = string_array[2];
	std::string reason = string_array.size() > 3 ? string_array[3] : "No reason provided.";
	Channel *channel = findChannelByName(channelName);
	if (!channel)
	{
		this->_clients[i - 1].sendMessage("Error: Channel " + channelName + " not found.");
		return ;
	}
	Client *operatorClient = &_clients[i - 1];
	if (!channel->isOperator(operatorClient))
	{
		operatorClient->sendMessage("Error: You are not an operator in the channel " + channelName + ".");
        return ;
	}
	Client *targetClient = findClientByNickname(targetNickname);
	if (!targetClient)
	{
		operatorClient->sendMessage("Error: Client with nickname " + targetNickname + " not found.");
        return ;
	}
	if (!channel->isClientInChannel(targetClient))
	{
		operatorClient->sendMessage("Error: Client " + targetNickname + " is not in the channel " + channelName + ".");
        return ;
	}//for irssi
	std::string kickMessage = ":" + operatorClient->getNickname() + "!" + operatorClient->getUsername() + "@" + operatorClient->getHostname() +
							  " KICK " + channelName + " " + targetNickname + " :" + (reason.empty() ? "No reason" : reason);
	targetClient->sendMessage(kickMessage);

	// 向頻道內其他成員廣播踢人消息
	channel->broadcastMessage(kickMessage, targetClient);

	// 從頻道中移除用戶
	channel->kickClient(targetClient);

	std::cout << "Client " << targetNickname << " has been kicked from channel " << channelName
			  << " by operator " << operatorClient->getNickname() << ". Reason: " << (reason.empty() ? "No reason" : reason) << std::endl;
	// channel->kickClient(targetClient, reason);
    // channel->broadcastMessage(targetNickname + " has been kicked from the channel " + channelName + " for: " + reason);
    // std::cout << "Client " << targetNickname << " has been kicked from channel " << channelName << " by operator " 
    //           << operatorClient->getNickname() << ". Reason: " << reason << std::endl;
}

void	Server::cmdInvite(int i, std::vector<std::string> string_array)
{
	if (this->isRegistered(i) == 0)
	{
		std::cout << "Client not registered"  << std::endl;
		return ;
	}
	std::cout << "cmdInvite" << " : " << string_array[0] << std::endl;

    if (string_array.size() < 3)
    {
        this->_clients[i - 1].sendMessage("Error: INVITE command requires at least two arguments: <nickname> <channel>");
        return;
    }

    std::string targetNickname = string_array[1];
    std::string channelName = string_array[2];

    Channel* channel = findChannelByName(channelName);
    if (!channel)
    {
        this->_clients[i - 1].sendMessage("Error: Channel " + channelName + " not found.");
        return;
    }

    if (!channel->isOperator(&this->_clients[i - 1]))
    {
        this->_clients[i - 1].sendMessage("Error: You are not an operator in this channel.");
        return;
    }

    Client* targetClient = findClientByNickname(targetNickname);
    if (!targetClient)
    {
        this->_clients[i - 1].sendMessage("Error: Target client " + targetNickname + " not found.");
        return;
    }
    channel->inviteClient(targetClient);
}

void Server::cmdTopic(int i, std::vector<std::string> string_array)
{
    if (this->isRegistered(i) == 0)
    {
        std::cout << "Client not registered" << std::endl;
        return;
    }

    if (string_array.size() < 2)
    {
        this->_clients[i - 1].sendMessage("Error: TOPIC command requires a channel name.");
        return;
    }

    std::string channelName = string_array[1];
    Channel* channel = findChannelByName(channelName);
    if (!channel)
    {
        this->_clients[i - 1].sendMessage("Error: Channel " + channelName + " not found.");
        return;
    }

    if (!channel->isOperator(&this->_clients[i - 1]))
    {
        this->_clients[i - 1].sendMessage("Error: You are not an operator in this channel.");
        return;
    }

    // check topic
    if (string_array.size() == 2)
    {
        std::string currentTopic = channel->getTopic();
        if (currentTopic.empty())
        {
            this->_clients[i - 1].sendMessage("Current topic is: No topic set.");
        }
        else
        {
            this->_clients[i - 1].sendMessage("Current topic: " + currentTopic);
        }
        return;
    }

	//join multiple parameters as new topic
    if (string_array.size() > 2)
    {
        std::string newTopic;
        for (size_t j = 2; j < string_array.size(); ++j)
        {
            newTopic += string_array[j] + " ";
        }
        newTopic = newTopic.substr(0, newTopic.size() - 1);
        channel->setTopic(newTopic);
        this->_clients[i - 1].sendMessage("Topic updated successfully.");
    }
    else
    {
        this->_clients[i - 1].sendMessage("Error: Incorrect number of parameters.");
    }
}

void	Server::cmdMode(int i, std::vector<std::string> string_array)
{
    if (this->isRegistered(i) == 0)
    {
        std::cout << "Client not registered" << std::endl;
        return;
    }
    std::cout << "cmdMode" << " : " << string_array[0] << std::endl;
    if (string_array.size() < 3)
    {
        this->_clients[i - 1].sendMessage("Error: MODE command requires a channel name, mode, and value.");
        return;
    }
    std::string channelName = string_array[1];
    std::string mode = string_array[2];
    std::string extra_cmd = (string_array.size() > 3) ? string_array[3] : "";

    Channel* channel = findChannelByName(channelName);
    if (!channel)
    {
        this->_clients[i - 1].sendMessage("Error: Channel " + channelName + " not found.");
        return;
    }
    if (!channel->isOperator(&this->_clients[i - 1]))
    {
        this->_clients[i - 1].sendMessage("Error: You are not an operator in this channel.");
        return;
    }
    channel->setMode(mode, extra_cmd, &_clients[i - 1]);
    this->_clients[i - 1].sendMessage("Mode updated successfully.");
}


