#include "../includes/Irc.hpp"

Client::Client(int fd): host(getSystemHostname()), client_fd(fd), _passwordIsCorrect(0)
// Client::Client(int fd): client_fd(fd)
{
    recv_buf = "";
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

std::string Client::getHostname() const
{
    return (host);
}

void Client::sendMessage(const std::string &message)
{
    if (client_fd <= 0)
    {
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

void Client::removeChannel(Channel* channel) {
    std::vector<Channel*>::iterator it = std::find(channels.begin(), channels.end(), channel);
    if (it != channels.end()) {
        channels.erase(it);
    }
}

// void Client::Recv()
// {
//     char buf[1024];
//     int bytes_received = recv(client_fd, buf, sizeof(buf) - 1, 0);
//     if (bytes_received > 0)
//     {
//         buf[bytes_received] = '\0';
//         recv_buf += buf;
//     }
//     else if (bytes_received == 0)
//     {
//         close(client_fd);
//         client_fd = -1;
//     }
//     else
//         perror("recv");
// }

void Client::Send()
{
    if (!recv_buf.empty())
    {
        int bytes_sent = send(client_fd, recv_buf.c_str(), recv_buf.length(), 0);
        if (bytes_sent > 0)
            recv_buf.erase(bytes_sent); //Erases part of the string
        else
            perror("send");
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

std::string Client::getName(void)
{
	return this->nick;
}
