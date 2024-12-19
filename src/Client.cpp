#include "../includes/Irc.hpp"

Client::Client(int fd): nick("*"),oldnick(""),usr(""),host("localhost"),buffer(""),client_fd(fd), _passwordIsCorrect(0), _isRegistered(0)
{
	std::cout << GREEN << host << std::endl;
}

void Client::setFd(int fd){
    client_fd = fd;
}

void Client::setNickname(const std::string& nickname)
{
    nick = nickname;
}

void Client::setUsername(const std::string& username)
{
    usr = username;
}

void Client::setOldNick(const std::string& oldnickname)
{
    oldnick = oldnickname;
}

const std::string& Client::getNickname() const
{
    return (nick);
}

const std::string& Client::getUsername() const
{
    return (usr);
}

const std::string& Client::getOldNick() const
{
    return (oldnick);
}

std::string Client::getHostname() const
{
    return (host);
}

void Client::sendMessage(const std::string &message) const
{
    if (client_fd <= 0)
    {
        std::cerr << "Error: Invalid client_fd for client." << std::endl;
        return;
    }
    
    std::cout << "Sending message to client_fd " << client_fd << ": " << message << std::endl;
    if (send(client_fd, message.c_str(), message.length(), 0) == -1)
        perror("send");
    else
        std::cout << "Message sent successfully." << std::endl;
}

void Client::addChannel(const Channel &ch)
{
    channels.push_back(ch);
}

void Client::removeChannel(const Channel &channel)
{
    std::vector<Channel>::iterator it = std::find(channels.begin(), channels.end(), channel);
    if (it != channels.end()) {
        channels.erase(it);
    }
}

int Client::getFd() const
{
    return client_fd;
}

bool Client::isInvited(const Client &client, const Channel &channel) const
{
    if (client.getFd() <= 0 || channel.getName().empty()) {
        return false;
    }

    const std::vector<Client>& invitedClients = channel.getInvitedClients();

    for (std::vector<Client>::const_iterator it = invitedClients.begin(); it != invitedClients.end(); ++it) {
        if (it->getFd() == client.getFd()) {
            return true;
        }
    }

    return false;
}

void	Client::setBuffer(std::string str)
{
	this->buffer = this->buffer + str;
}

std::string &	Client::getBuffer(void)
{
	return(this->buffer);
}

void	Client::setPasswordIsCorrect(void)
{
	this->_passwordIsCorrect = 1;
}

int	Client::getPasswordIsCorrect(void)
{
	return(this->_passwordIsCorrect);
}

 void Client::setIsRegistered(void)
 {
    _isRegistered = 1;
 }

 int Client::getIsRegistered(void)
 {
    return _isRegistered;
 }
std::string Client::getName(void) const
{
	return this->nick;
}
