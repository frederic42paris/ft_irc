#include "../includes/Irc.hpp"

void	Server::lineParsing(std::string line, int i)
{
	std::vector<std::string> line_splitted;

	std::stringstream ss(line);
	std::string tmp;
	char del = ' ';

 	while(std::getline(ss, tmp, del))
	{
		line_splitted.push_back(tmp);
	}
	std::cout << line_splitted.size() << std::endl;
	if (line == "CAP LS")
		cmdCap(i);
	else if (line_splitted[0] == "PASS")
		cmdPass(i, line_splitted);
	else if (line_splitted[0] == "USER")
		cmdUser(i, line_splitted);
	else if (line_splitted[0] == "NICK")
		cmdNick(i, line_splitted);
	else if (line_splitted[0] == "QUIT")
		cmdQuit(i, line_splitted);
	else if (line_splitted[0] == "PRIVMSG")
		cmdPrivmsg(i, line_splitted, line);
	else if (line_splitted[0] == "JOIN")
		cmdJoin(i, line_splitted);
	else if (line_splitted[0] == "INVITE")
		cmdInvite(i, line_splitted);
	else if (line_splitted[0] == "PART")
		cmdPart(i, line_splitted);
	else if (line_splitted[0] == "TOPIC")
		cmdTopic(i, line_splitted);
	else if (line_splitted[0] == "MODE")
		cmdMode(i, line_splitted);
	else if (line_splitted[0] == "KICK")
		cmdKick(i, line_splitted);
}

// void	Server::commandParsing(int i, std::string buf)
// {
// 	Client *current = getClient(_fds[i].fd);
// 	std::vector<std::string> string_array;
// 	std::string::size_type pos = 0;
// 	std::string string = current->getBuffer();
// 	std::cout << "Before append: " << string << " | Client " << current->getFd() << std::endl;
// 	string.append(buf);
// 	std::cout << "After append: " << string << std::endl;
// 	pos = string.rfind("\r\n");
//     if (last_pos != std::string::npos && last_pos + 2 == string.length()) {
//         std::string::iterator start = string.begin();
//         std::string::iterator end = string.begin() + last_pos;
//         std::string word;
//         for (std::string::iterator it = start; it != end; ++it) {
//             if (*it == ' ') {
//                 if (!word.empty()) {
//                     string_array.push_back(word);
//                     word.clear();
//                 }
//             }
// 			else
//                 word += *it;
//         }
//         if (!word.empty()) {
//             string_array.push_back(word);
//         }
//         string.clear();
//     } 
// 	else {
//         current.updateBuffer(string);
//         string.clear();
// 		return;
//     }
// }
// 	std::cout<<GREEN "Full message received by " YELLOW "Client "<<current->getFd() <<GREEN ": " RESET << string << std::endl;
// 	if (string_array.empty()) // empty command
// 	{
// 		std::cerr << "Error: Empty command received" << std::endl;
// 		return;
// 	}

// 	std::string cmd_array[] = {
// 		"NICK",
// 		"PASS",
// 		"USER",
// 		"QUIT",
// 		"PRIVMSG",
// 		"JOIN",
// 		"PART",
// 		"KICK",
// 		"INVITE",
// 		"TOPIC",
// 		"MODE"
// 	};

// 	int len = sizeof(cmd_array) / sizeof(cmd_array[0]);
// 	int j = 0;
// 	while (j < len)
// 	{
// 		if (cmd_array[j] == string_array.front())
// 			break;
// 		j++;
// 	}

// 	if (j >= len)
//     {
//         std::cerr << "Error: Unrecognized command [" << string_array[0] << "]." << std::endl;
//         return;
//     }
	
// 	if (string_array.size() > 1)
//     	std::cout << YELLOW << string_array[1] << RESET << std::endl;
// 	else
// 	{
//     	string_array.push_back("");
// 	}

// 	switch(j)
// 	{
// 		case 0: this->cmdNick(i, string_array); break;
// 		case 1: this->cmdPass(i, string_array); break;
// 		case 2: this->cmdUser(i, string_array); break;
// 		case 3: this->cmdQuit(i, string_array); break;bool isValidChannelName(const std::string& channelName);
//         Channel* findChannelByName(const std::string& channelName);
//         Client* findClientByNickname(const std::string& nickname);
//         void setMode(const std::string& mode, const std::string& value);
// 		case 3: this->cmdQuit(i, string_array); break;
// 		case 4: this->cmdPrivmsg(i, string_array, string); break;
// 		case 5: this->cmdJoin(i, string_array); break;
// 		case 6: this->cmdPart(i, string_array); break;
// 		case 7: this->cmdKick(i, string_array); break;
// 		case 8: this->cmdInvite(i, string_array); break;
// 		case 9: this->cmdTopic(i, string_array); break;
// 		case 10: this->cmdMode(i, string_array); break;
//     default : break;
// 	}
// }

// int		Server::isRegistered(int i)
// {
// 	if (this->_clients[i - 1].getNickname().empty()
// 		|| this->_clients[i - 1].getUsername().empty()
// 		|| this->_clients[i - 1].getPasswordIsCorrect() == 0)
// 	{
// 		return (0);
// 	}
// 	else
// 		std::cout << "/r/n present"<< std::endl;
// Test when ctrl+D (eof), it should accumulate buffer
void	Server::bufferParsing(int i, std::string string)
{
	this->_clients[i - 1].setBuffer(string);
	if (this->_clients[i - 1].getBuffer().find("\r\n") == std::string::npos)
		return;
	
	std::vector<std::string> string_array;
	std::stringstream ss(this->_clients[i - 1].getBuffer());
	std::string line;
	std::string tmp;
 	while(std::getline(ss, line))
	{
		size_t pos = line.find_first_of("\r\n");
		string_array.push_back(line.substr(0,pos));
	}

	for (size_t j =0; j < string_array.size(); j++)
	{
		lineParsing(string_array[j], i);
	}
	this->_clients[i - 1].getBuffer().clear();
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

Channel* Server::findChannelByName(const std::string& channelName)
{
    for (size_t j = 0; j < this->_channels.size(); j++)
    {
        if (this->_channels[j].getName() == channelName)
            return (&this->_channels[j]);
    }
    return NULL;
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
