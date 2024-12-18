#include "../includes/Irc.hpp"

#include "../includes/Irc.hpp"

void	Server::cmdPart(int i, std::vector<std::string> string_array) //parameter: 1channel, reason
{
	Channel* channel = NULL;
    for (std::vector<Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it)
    {
        if (it->getName() == string_array[1])
        {
            channel = &(*it);
            break;
        }
    }
	channel->partChannel(_clients[i - 1], i); //handle in channel
}

void Channel::partChannel(Client& client, int i)
{
    std::string reason = "left the channel";
	std::vector<Client>::iterator it = std::find(clients.begin(), clients.end(), client);
    // if (it != clients.end())
    // {
	// client.sendMessage("You have parted from channel " + name + "\n");
	// client.sendMessage(USER_ID(client.getNickname(), client.getUsername()) + " PART " + name + " :" + reason + "\r\n");
	std::string msg = ":" + this->clients[i - 1].getNickname() + "!"+ this->clients[i - 1].getUsername() + "@localhost PART " + getName() + "\r\n";
	std::cout << msg;
	send(this->clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);

	std::string msg1 = ":" + this->clients[i - 1].getNickname() + "!"+ this->clients[i - 1].getUsername() + "@localhost PART " + getName() + "\r\n";
	std::cout << msg1;
	send(this->clients[i].getFd(), msg1.c_str(), msg1.length(), 0);

	// std::string msg1 = ":" + this->clients[i - 1].getNickname() + "!"+ this->clients[i - 1].getUsername() + "@localhost PART " + getName() + "\r\n";
	// std::cout << msg1;
	// send(this->clients[i].getFd(), msg1.c_str(), msg1.length(), 0);
	// Debug statement to check client after parting
	// std::cout << "Client " << client.getNickname() << " has parted from channel " << name << std::endl;
	clients.erase(it);
    // }
    // else
    // {
    //     std::cerr << "Error: Client " << client.getNickname() << " not found in channel " << name << std::endl;
    // }
}