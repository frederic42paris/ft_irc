#include "../includes/Irc.hpp"

Client::Client(int fd): nick("*"),host(getSystemHostname()), client_fd(fd), _passwordIsCorrect(0), _isRegistered(0)
{
    recv_buf = "";
}

void Client::updateBuffer(std::string str){
	std::cout << "STRING UPDATE: " RED << str << RESET << std::endl;
	_buffer = str;	
	std::cout << "UPDATEBUF: " << _buffer << std::endl;
}

// std::string Client::getBuffer(){
// 	std::cout << "GETBUF: " << _buffer << std::endl;
// 	return _buffer;
// }

void Client::clearBuffer()
{
	std::cout << "CLEARBUF: " << _buffer << std::endl;
	_buffer.empty();
	std::cout << "AFTER: " << _buffer << std::endl;
}
std::string Client::getSystemHostname() {
    char hostname[1024];
    if (gethostname(hostname, sizeof(hostname)) == 0)
        return std::string(hostname);
    return "unknown";
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

void Client::setHostname(const std::string& hostname)
{
    host = hostname;
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

void Client::sendMessage(const std::string &message)
{
    if (client_fd <= 0){
        std::cerr << "Error: Invalid client_fd for client." << std::endl;
        return;
    }
    std::string formattedMessage = message + "\r\n";
    if (send(client_fd, formattedMessage.c_str(), formattedMessage.length(), 0) == -1)
        perror("send");
}

void Client::addChannel(Channel* ch)
{
    channels.push_back(ch);
}

void Client::removeChannel(Channel* channel)
{
    std::vector<Channel*>::iterator it = std::find(channels.begin(), channels.end(), channel);
    if (it != channels.end()) {
        channels.erase(it);
    }
}

int Client::getFd() const
{
    return client_fd;
}

bool Client::isInvited(Client* client, Channel* channel)
{
    if (!client || !channel) {
        return false;
    }

    const std::vector<Client*>& invitedClients = channel->getInvitedClients();

    for (std::vector<Client*>::const_iterator it = invitedClients.begin(); it != invitedClients.end(); ++it) {
        if (*it == client) {
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
std::string Client::getName(void)
{
	return this->nick;
}
