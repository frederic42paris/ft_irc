#include "../includes/Irc.hpp"

void Server::channelMsg(int i, std::vector<std::string> string_array, std::string buffer)
{
	std::string msg;
	std::string message;
	int pos = buffer.find("#");
	int pos2 = buffer.find(" ", pos) + 1;
	message = buffer.substr(pos2);

	int k = 0;
	while (k < static_cast<int>(getChannels().size()))
	{
		if (getChannels()[k].getName() == string_array[1])
			break;
		k++;
	}
	if (k == static_cast<int>(getChannels().size()))
	{
		msg = ":localhost 401 " + string_array[1] + " :No such channel\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
		return;
	}

	for (std::vector<Channel >::iterator it = this->_channels.begin(); it != _channels.end(); it++)
	{
		if ((*it).getName() == string_array[1])
		{
			int k = 0;
			while (k < static_cast<int>((*it).getClients().size()))
			{
				if ((*it).getClients()[k].getNickname() == this->_clients[i - 1].getNickname())
					break;
				k++;
			}
			if (k == static_cast<int>((*it).getClients().size()))
			{
				msg = ":localhost 404 " + this->_clients[i - 1].getNickname() + " " + string_array[1] + " :Cannot send to channel\r\n";
				send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
				return;
			}
			int j = 0;
			while (j < static_cast<int>((*it).getClients().size()))
			{
				if (this->_clients[i - 1].getNickname() != (*it).getClients()[j].getNickname())
				{
					msg = ":" + this->_clients[i - 1].getNickname() + "!" + this->_clients[i - 1].getUsername() + "@localhost PRIVMSG " + string_array[1] + " " + message + "\r\n";
					send((*it).getClients()[j].getFd(), msg.c_str(), msg.length(), 0);
				}
				j++;
			}
		}
	}
}

void Server::userMsg(int i, std::vector<std::string> string_array, std::string buffer)
{
	std::string msg;
	std::string message;
	int pos = buffer.find(string_array[1]);
	int pos2 = buffer.find(" ", pos) + 1;
	message = buffer.substr(pos2);

	int k = 0;
	while (k < static_cast<int>(getClients().size()))
	{
		if (getClients()[k].getNickname() == string_array[1])
			break;
		k++;
	}
	if (k == static_cast<int>(getClients().size()))
	{
		msg = ":localhost 401 " + this->_clients[i - 1].getNickname() + " :No such nick\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
		return;
	}
	for (std::vector<Client>::iterator it = this->_clients.begin(); it != _clients.end(); it++)
	{
		if ((*it).getNickname() == string_array[1])
		{
			std::string msg = this->_clients[i - 1].getNickname() + " " + message + "\r\n";
			send((*it).getFd(), msg.c_str(), msg.length(), 0);
		}
	}
}

void	Server::cmdPrivmsg(int i, std::vector<std::string> string_array, std::string buffer)
{
	std::string msg;
	if (this->_clients[i - 1].getIsRegistered() == 0)
	{
		msg = "Error : Client is not registered\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
		return ;
	}
	else if (string_array.size() < 3)
	{
		msg = ":localhost 461 " + this->_clients[i - 1].getNickname() + " PRIVMSG :Not enough parameters\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
		return;
	}
	if (string_array[1][0] == '#')
		channelMsg(i, string_array, buffer);
	else
		userMsg(i, string_array, buffer);
}
