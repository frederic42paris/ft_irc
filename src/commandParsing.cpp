#include "../includes/Irc.hpp"

void	Server::lineParsing(std::string line, int i)
{
	std::vector<std::string> line_splitted;

	// std::stringstream ss(line);
	// std::string tmp;
	// char del = ' ';
	

	// std::string str = line;
	// std::string del = "\r\n";
	// size_t pos = line.find_first_of("\n ");
	// std::cout  << pos << std::endl;
	// while (pos != std::string::npos)
	// {
	// 	std::cout  << line << std::endl;
	// 	line_splitted.push_back(line.substr(0, pos));
    //     line.erase(0, pos + 1);
    //     pos = line.find("\r\n");
	// 	std::cout  << pos << std::endl;

    // }



	std::cout << line.length() << std::endl;
	std::string str = line;
	// std::string del = "\r\n ";
	// size_t pos = 0;

	std::size_t pos = str.find_first_of("\r\n ");
	std::cout  << pos << std::endl;
 	if (pos!=std::string::npos)
	{
		line_splitted.push_back(str.substr(0, pos));
	}
	pos++;

	std::size_t newpos = str.find_first_of("\r\n ", pos);
	std::cout  << newpos << std::endl;
 	if (newpos!=std::string::npos)
	{
		std::cout << newpos  << std::endl;
		line_splitted.push_back(str.substr(pos, newpos - pos));
	}

	// while(str[pos] != '\r')
	// {
	// 	std::cout << pos << " " << str[pos] << std::endl;
	// 	pos++;
	// }

	// while (pos != std::string::npos)
	// {
	// 	std::cout  << "1 " << line.length() << std::endl;
	// 	line_splitted.push_back(str.substr(0, pos));
    //     str.erase(0, pos);
    //     pos = str.find_first_of(del);
	// 	std::cout  << "2 " << line.length() << std::endl;
    // }


	std::cout  << "TAILLE " <<line_splitted.size() << std::endl;

	std::cout  << "one " <<line_splitted[0] << std::endl;
	std::cout  << "TAILLE " <<line_splitted[0].size() << std::endl;

	std::cout  << "two " <<line_splitted[1] << std::endl;
	std::cout  << "TAILLE " <<line_splitted[1].size() << std::endl;

 	// while(std::getline(ss, tmp, del))
	// {
	// 	line_splitted.push_back(tmp);
	// }
	if (line == "CAP LS")
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

void	Server::bufferParsing(int i, std::string string)
{
	this->_clients[i - 1].setBuffer(string);
	if (this->_clients[i - 1].getBuffer().find("\r\n") == std::string::npos)
		return;
	
	std::vector<std::string> string_array;
	// std::stringstream ss(this->_clients[i - 1].getBuffer());
	// std::string line;
	// std::string tmp;
	// std::cout << "ICI" << this->_clients[i - 1].getBuffer() << std::endl;
	

	std::string str = this->_clients[i - 1].getBuffer();
	std::cout << str.length() << std::endl;
	std::string del = "\r\n";
	size_t pos = str.find(del);

	while (pos != std::string::npos)
	{
		string_array.push_back(str.substr(0, pos + 2));
		std::cout << string_array[0].length() << std::endl;
        str.erase(0, pos + del.length());
        pos = str.find(del);
    }


 	// while(std::getline(ss, line, del))
	// {
	// 	size_t pos = line.find("\r");
	// 	std::cout  << "POS " << pos << std::endl;
	// 	string_array.push_back(line.substr(0,pos));
	// }
	// std::cout  << "TAILLE " <<string_array.size() << std::endl;
	// std::cout  << "STRING : [" <<string_array[0] << "]" << std::endl;
	for (size_t j =0; j < string_array.size(); j++)
	{
		lineParsing(string_array[j], i);
	}
	if (string_array[0].compare(0, 4, "QUIT") != 0)
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


bool Server::isValidChannelName(const std::string& channelName)
{
	int j = 0;
	while (channelName[j])
	{
		if (channelName[0] != '#')
		{
			return 0;
		}
		if (channelName[j] == ',' || channelName[j] == ' ' || channelName[j] == 7)
		{
			return 0;
		}
		j++;
	}
	return 1;
}
