#include "../includes/Irc.hpp"

void	Server::cmdJoin(int i, std::vector<std::string> string_array)
{
	Client *current = &_clients[i - 1];
	if (!current->getIsRegistered()){
		return current->sendMessage("Error: Client is not registered");}
	if (string_array.size() < 2){
        return current->sendMessage("[Error 461] JOIN : Not enough parameters");}
    std::string chanName = string_array[1];
    std::string pwd = (string_array.size() > 2) ? string_array[2] : "";
    if (!isValidChannelName(chanName)){
        return current->sendMessage("[Error 403] " + chanName + " : Invalid channel name");}

	Channel *channel = findChannelByName(chanName);
	if (channel == NULL){
		_channels.push_back(Channel(chanName));
		_channels.back().joinChannel(current, pwd);
	}
	else
		channel->joinChannel(current, pwd);

	std::string msgs = RPL_JOIN(USER_ID(current->getNickname(), current->getUsername()), chanName) + \
						":localhost 353 " + current->getNickname() + " = " + chanName + " :@" + current->getNickname() + "\r\n" + \
						":localhost 366 " + current->getNickname() + " " + chanName + " :End of /NAMES list\r\n";
    std::cout << msgs << std::endl;
    send(current->getFd(), msgs.c_str(), msgs.length(), 0);
}
