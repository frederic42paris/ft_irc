#include "../includes/Irc.hpp"

void Server::cmdMode(int i, std::vector<std::string> string_array)
{
    if (this->isRegistered(i) == 0)
    {
        this->_clients[i - 1].sendMessage(":localhost 451 * :You have not registered\r\n");
        return;
    }

    if (string_array.size() < 2)
    {
        this->_clients[i - 1].sendMessage(":localhost 461 MODE :Not enough parameters\r\n");
        return;
    }

	if (string_array[0] == "MODE" && string_array[1] == _clients[i - 1].getNickname() && string_array[2] == "+i" && string_array.size() == 3)
	{
		// this->_clients[i - 1].sendMessage("Error : empty channel \r\n");
		return;
	}

    std::string channelName = string_array[1];
    std::string mode = (string_array.size() > 2) ? string_array[2] : "";
    std::string extra_cmd = (string_array.size() > 3) ? string_array[3] : "";

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
        this->_clients[i - 1].sendMessage(":localhost 403 " + channelName + " :No such channel" + "\r\n");
        return;
    }

    if (mode.empty())
    {
        // Return current channel modes
        this->_clients[i - 1].sendMessage(":localhost 324 " + this->_clients[i - 1].getNickname() +
                                          " " + channelName + " " + channel->getMode("") + "\r\n"); //! check needed
        return;
    }

    if (!channel->isOperator(this->_clients[i - 1]))
    {
        this->_clients[i - 1].sendMessage(":localhost 482 " + channelName + " :You're not a channel operator" + "\r\n");
        return;
    }

    channel->setMode(mode, extra_cmd, _clients[i - 1]);
    this->_clients[i - 1].sendMessage("Mode updated successfully."); // keep?
}

std::string Channel::getMode(const std::string& mode) const
{
    std::map<std::string, std::string>::const_iterator it = modes.find(mode);
    if (it != modes.end())
        return (it->second);
    return ("");
}

std::string intToString(int num)
{
    if (num == 0) return "0";
    std::string result;
    bool isNegative = (num < 0);
    if (isNegative) num = -num;

    while (num > 0)
    {
        result.insert(result.begin(), '0' + (num % 10));
        num /= 10;
    }

    if (isNegative) result.insert(result.begin(), '-');
    return result;
}

void Channel::setMode(const std::string& modeStr, const std::string& value, const Client& client)
{
    if (modeStr.empty())
    {
        client.sendMessage(":localhost 461 " + client.getNickname() + " MODE :Not enough parameters\r\n");
        return;
    }

    char operation = modeStr[0]; // '+' or '-'
    if (operation != '+' && operation != '-')
    {
        client.sendMessage(":localhost 501 " + client.getNickname() + " :Unknown MODE flag\r\n");
        return;
    }

    // Apply modes one by one
    for (size_t i = 1; i < modeStr.size(); ++i)
    {
        char mode = modeStr[i];
        std::string modeMessage;  // Standard IRC MODE message
        std::string feedback;    // Intuitive message for the client
        switch (mode)
        {
            case 'i': // Invite-only
                inviteOnly = (operation == '+');
                modeMessage = ":Server MODE " + name + " " + operation + "i";
                feedback = (operation == '+') ? 
                    "You have set the channel to invite-only mode." : 
                    "You have disabled invite-only mode for the channel.";
                break;

            case 't': // Topic restrictions
                topicRestricted = (operation == '+');
                modeMessage = ":Server MODE " + name + " " + operation + "t";
                feedback = (operation == '+') ? 
                    "You have restricted topic changes to operators only." : 
                    "You have allowed everyone to change the topic.";
                break;

            case 'k': // Channel password
                if (operation == '+')
                {
                    Ch_pwd = value;
                    modeMessage = ":Server MODE " + name + " " + operation + "k " + value;
                    feedback = "You have set the channel password to: " + value;
                }
                else
                {
                    Ch_pwd.clear();
                    modeMessage = ":Server MODE " + name + " " + operation + "k";
                    feedback = "You have removed the channel password.";
                }
                break;

            case 'o': // Operator privileges
            {
                Client* targetClient = NULL;

				// for (size_t j = 0; j < this->getClients().size(); ++j)
				// {
				// 	if (this->getClients()[j].getNickname() == value)
				// 	{
				// 		targetClient = &this->getClients()[j];
				// 		break;
				// 	}
				// }

				for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
				{
					if (it->getNickname() == value)
					{
						targetClient = &(*it);
						break;
					}
				}

				if (!targetClient)
                {
                    client.sendMessage(":localhost 401 " + client.getNickname() + " " + value + " :No such nick/channel\r\n");
                    continue;
                }

                if (operation == '+')
                {
                    if (!isOperator(*targetClient))
                    {
                        addOperator(*targetClient);
                        modeMessage = ":Server MODE " + name + " " + operation + "o " + value + "\r\n";
                        feedback = "You have granted operator privileges to " + value + "." + "\r\n";

                        // Notify targetClient
                        targetClient->sendMessage(":localhost NOTICE " + targetClient->getNickname() + 
                            " :You have been granted operator privileges in channel " + name + "." + "\r\n");
                    }
                    else
                    {
                        client.sendMessage(":localhost 324 " + client.getNickname() + " " + name + " " + value + " " + " :is already an operator\r\n");
                        continue;
                    }
                }
                else
                {
                    if (isOperator(*targetClient))
                    {
                        removeOperator(*targetClient);
                        modeMessage = ":Server MODE " + name + " " + operation + "o " + value + "\r\n";
                        feedback = "You have removed operator privileges from " + value + "." + "\r\n";

                        // Notify targetClient
                        targetClient->sendMessage(":localhost NOTICE " + targetClient->getNickname() + 
                            " :Your operator privileges in channel " + name + " have been revoked." + "\r\n");
                    }
                    else
                    {
                        client.sendMessage(":localhost 324 " + client.getNickname() + " " + name + " " + value + " " + " :is not an operator and cannot be removed" + "\r\n"); //not yet
                        continue;
                    }
                }
                break;
            }

            case 'l': // User limit
                if (operation == '+')
                {
                    userLimits = std::atoi(value.c_str());
                    modeMessage = ":Server MODE " + name + " " + operation + "l " + intToString(userLimits) + "\r\n";
                    feedback = "You have set the channel user limit to " + intToString(userLimits) + "." + "\r\n";
                }
                else
                {
                    userLimits = -1; // Remove limit
                    modeMessage = ":Server MODE " + name + " " + operation + "l" + "\r\n";
                    feedback = "You have removed the user limit for the channel\r\n.";
                }
                break;

            default:
                client.sendMessage(":localhost 472 " + client.getNickname() + " " + std::string(1, mode) + " :is unknown mode char to me" + "\r\n");
                continue;
        }

        // Send standard IRC mode message
        client.sendMessage(modeMessage);
        broadcastMessage(modeMessage, client);

        // Send intuitive feedback to the client
        client.sendMessage(":localhost NOTICE " + client.getNickname() + " :" + feedback + "\r\n");
    }
}
