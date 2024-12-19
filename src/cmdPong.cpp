#include "../includes/Irc.hpp"

void Server::cmdPong(int i, std::string token){
        std::string msg = "PONG localhost " + token + "\r\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
}
