#include "../includes/Irc.hpp"

void	Server::bufferParsing(int i, std::string string)
{
	this->_clients[i - 1].setBuffer(string);
	if (this->_clients[i - 1].getBuffer() == "\r\n")
		return(this->_clients[i - 1].getBuffer().clear());
	if (this->_clients[i - 1].getBuffer().find("\r\n") == std::string::npos)
		return;
	std::vector<std::string> string_array;
	std::string line = this->_clients[i - 1].getBuffer();
	size_t pos = line.find_first_of("\r\n");
	int len;
    while (pos != std::string::npos)
	{
		len = 0;
		if (pos > 0)
			string_array.push_back(line.substr(0,pos));
		if (line[pos] == '\n')
			len = 1;
		else
			len = 2;
        line.erase(0, pos + len);
        pos = line.find_first_of("\r\n");
    }
	for (size_t j =0; j < string_array.size(); j++)
	{
		lineParsing(string_array[j], i);
	}
	if (string_array.size() > 0 && string_array[0].compare(0, 4, "QUIT") != 0)
		this->_clients[i - 1].getBuffer().clear();
}

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
	if (line == "CAP LS 302")
		cmdCap(i);
	else if (line_splitted[0] == "PASS")
		cmdPass(i, line_splitted);
	else if (line_splitted[0] == "USER")
		cmdUser(i, line_splitted);
	else if (line_splitted[0] == "NICK")
		cmdNick(i, line_splitted);
	else if (line_splitted[0] == "QUIT")
		cmdQuit(i);
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
	else if (line_splitted[0] == "PING")
		cmdPing(i, line_splitted);
}

// Channel Server::findChannelByName(const std::string& channelName)
// {
//     for (size_t j = 0; j < this->_channels.size(); j++)
//     {
//         if (this->_channels[j].getName() == channelName)
//             return (this->_channels[j]);
//     }
//     return Channel();
// }
// Client Server::findClientByNickname(const std::string& nickname)
// {
//     for (size_t j = 0; j < this->_clients.size(); j++)
// 	{
//         if (this->_clients[j].getNickname() == nickname)
//             return (this->_clients[j]);
//     }
//     return Client();
// }

bool Server::isValidChannelName(const std::string& channelName)
{
    // start with # and no space
	// std::cout << "NOT OK 2" << std::endl;
    // return !channelName.empty() && channelName[0] == '#' && channelName.find(' ') == std::string::npos;
	int j = 0;
	while (channelName[j])
	{
		// std::cout << "OK 3" << channelName[j] << std::endl;
		if (channelName[0] != '#')
		{
			return 0;
		}
		if (channelName[j] == ',' || channelName[j] == ' ' || channelName[j] == 7)
		{

			// std::cout <<  "OK 4" <<  channelName[j] << std::endl;
			return 0;

		}
		j++;
	}
	return 1;
}

// --------------------------LOIC and FRED Parsing

// void	Server::lineParsing(std::string str, int i)
// {
// 	std::vector<std::string> line_splitted;
// 	// std::cout << line.length() << std::endl;
// 	// std::string str = line;

// 	// std::size_t pos = str.find_first_of("\n ");
// 	// std::cout  << pos << std::endl;
//  	// if (pos!=std::string::npos)
// 	// {
// 	// 	line_splitted.push_back(str.substr(0, pos));
// 	// }
// 	// pos++;

// 	// std::size_t newpos = str.find_first_of("\n ", pos);
// 	// std::cout  << newpos << std::endl;
//  	// if (newpos!=std::string::npos)
// 	// {
// 	// 	std::cout << newpos  << std::endl;
// 	// 	line_splitted.push_back(str.substr(pos, newpos - pos));
// 	// }
// 	std::stringstream stringStream(str);
// 	std::string line;
// 	while(std::getline(stringStream, line)) 
// 	{
//     	std::size_t prev = 0, pos;
//     	while ((pos = line.find_first_of("\n ", prev)) != std::string::npos)
//     	{
//         	if (pos > prev){
//             	line_splitted.push_back(line.substr(prev, pos-prev));
// 				prev = pos+1;
// 			}
// 		}
// 		if (prev < line.length())
// 			line_splitted.push_back(line.substr(prev, std::string::npos));
// 	}
// 	for (size_t i = 0; i < line_splitted.size(); i++)
// 		std::cout << GREEN << line_splitted[i] << std::endl;

// 	// while(str[pos] != '\r')
// 	// {
// 	// 	std::cout << pos << " " << str[pos] << std::endl;
// 	// 	pos++;
// 	// }

// 	// while (pos != std::string::npos)
// 	// {
// 	// 	std::cout  << "1 " << line.length() << std::endl;
// 	// 	line_splitted.push_back(str.substr(0, pos));
//     //     str.erase(0, pos);
//     //     pos = str.find_first_of(del);
// 	// 	std::cout  << "2 " << line.length() << std::endl;
//     // }


// 	std::cout  << "TAILLE " <<line_splitted.size() << std::endl;

// 	std::cout  << "one " <<line_splitted[0] << std::endl;
// 	std::cout  << "TAILLE " <<line_splitted[0].size() << std::endl;

// 	std::cout  << "two " <<line_splitted[1] << std::endl;
// 	std::cout  << "TAILLE " <<line_splitted[1].size() << std::endl;

//  	// while(std::getline(ss, tmp, del))
// 	// {
// 	// 	line_splitted.push_back(tmp);
// 	// }
// 	if (line == "CAP LS")
// 		cmdCap(i);
// 	else if (line_splitted[0] == "PASS")
// 		cmdPass(i, line_splitted);
// 	else if (line_splitted[0] == "USER")
// 		cmdUser(i, line_splitted);
// 	else if (line_splitted[0] == "NICK")
// 		cmdNick(i, line_splitted);
// 	else if (line_splitted[0] == "QUIT")
// 		cmdQuit(i);
// 	else if (line_splitted[0] == "PRIVMSG")
// 		cmdPrivmsg(i, line_splitted, line);
// 	else if (line_splitted[0] == "JOIN")
// 		cmdJoin(i, line_splitted);
// 	else if (line_splitted[0] == "INVITE")
// 		cmdInvite(i, line_splitted);
// 	else if (line_splitted[0] == "PART")
// 		cmdPart(i, line_splitted);
// 	else if (line_splitted[0] == "TOPIC")
// 		cmdTopic(i, line_splitted);
// 	else if (line_splitted[0] == "MODE")
// 		cmdMode(i, line_splitted);
// 	else if (line_splitted[0] == "KICK")
// 		cmdKick(i, line_splitted);
// 	else if (line_splitted[0] == "PING")
// 		cmdPing(i, line_splitted);
// }

// void	Server::bufferParsing(int i, std::string string)
// {
// 	std::vector<std::string> string_array;
	
// 	this->_clients[i - 1].setBuffer(string);
// 	std::string str = this->_clients[i - 1].getBuffer();
// 	std::cout << str.length() << std::endl;
// 	std::string del = "\r\n";

// 	size_t pos = str.rfind(del);
// 	if (pos != std::string::npos)
// 	{
// 		if (pos + 2 == std::string::npos)
// 			return;
// 		string_array.push_back(str.substr(0, pos));
// 		std::cout << string_array[0].length() << std::endl;
//     }
// 	else{
// 		return;
// 	}

// 	for (size_t j =0; j < string_array.size(); j++)
// 	{
// 		lineParsing(string_array[j], i);
// 	}
// 	if (string_array[0].compare(0, 4, "QUIT") != 0)
// 		this->_clients[i - 1].getBuffer().clear();
// }
