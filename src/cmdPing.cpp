#include "../includes/Irc.hpp"

void Server::cmdPing(int i, std::vector<std::string> string_array){
	if (string_array.size() < 2)
	{
		std::string msg = ":localhost 461 " + this->_clients[i - 1].getNickname() + " PING :Not enough parameters\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
		return;
	}
	else if (string_array[1].empty())
	{
		std::string msg = ":localhost " + this->_clients[i - 1].getNickname() + " :Parameter incorrect\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
		return;
	}	
	else
	{
        cmdPong(i, string_array[1]);
	}	
}
