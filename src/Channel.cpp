#include "../includes/Channel.hpp"


Channel::Channel(const std::string& n): name(n), Ch_pwd(""), inviteOnly(false), topicRestricted(true), userLimits(100), topic("")  // limits set to?
{
}

const std::string& Channel::getName() const
{
    return (name);
}

void Channel::addClient(const Client &client)
{
    if (clients.size() >= static_cast<std::size_t>(userLimits))
    {
        client.sendMessage(":localhost 471 " + client.getNickname() + " " + name + " :Cannot join channel (+l)\r\n");
        return ;
    }
    clients.push_back(client);
    std::cerr << "Client" << client.getNickname() << " joined channel: " << name << std::endl;
}

// void Channel::removeClient(Client client)
// {
//     for (std::vector<Client >::iterator it = clients.begin(); it != clients.end(); ++it)
//     {
//         if (it == client)
//         {
//             clients.erase(it);
//             std::cout << "Client" << client->getNickname() << " left channel: " << name << std::endl;
//             break ;
//         }
//     }
//     std::cerr << "Client not found in the channel!" << std::endl;;
// }


void Channel::broadcastMessage(const std::string& message)
{
	int i = 0;
    for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
    {
		std::cout << RED << i << ": " << it->getNickname() << RESET <<  std::endl;
        it->sendMessage(message);
		i++;
    }
}

void Channel::broadcastMessage(const std::string &message, const Client &excludeClient)
{
	for (std::vector<Client >::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (*it != excludeClient)
		{
			it->sendMessage(message);
		}
	}
}

void Channel::changeCh_pwd(std::string newCh_pwd)
{
    Ch_pwd = newCh_pwd;
    std::cout << "Channel password updated.\n";
}

bool Channel::isOperator(const Client &client) const
{
    for (std::vector<Client>::const_iterator it = operators.begin(); it != operators.end(); ++it)
    {
        if (*it == client)
            return true;
    }
    return false;
}

void Channel::addOperator(const Client &client)
{
    if (!isOperator(client))
    {
        operators.push_back(client);
        std::cout << "Added operator: " << client.getName() << std::endl;
    }
    else
    {
        std::cout << "Client is already an operator: " << client.getName() << std::endl;
    }
}

void Channel::removeOperator(const Client &client)
{
    for (std::vector<Client>::iterator it = operators.begin(); it != operators.end(); ++it) {
        if (*it == client) {
            operators.erase(it);
            break;
        }
    }
}

bool Channel::isEmpty() const
{
    return clients.empty();
}

bool Channel::isClientInChannel(const Client &client) const
{
    return std::find(clients.begin(), clients.end(), client) != clients.end();
}

std::vector<Client> Channel::getClients(void)
{
	return clients;
}

bool Channel::getTopicRestricted() const
{
    return (topicRestricted);
}
