#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Channel.hpp"
#include "Commands.hpp"
#include "Server.hpp"
#include "Macros.hpp"

class Channel;

class Client{
    private:
        std::string nick;
        std::string usr;
        std::string host;
        std::string buffer; // take the message until \r\n
        std::vector<Channel*> channels;
        std::vector<Channel*> joinedChannels; // 用戶加入的頻道
        int client_fd;
        std::string recv_buf;
        std::string getSystemHostname();
		int _passwordIsCorrect;
    public:
        Client(){};
        ~Client(){};
        Client(int fd);
        void setFd(int fd);
        void setNickname(const std::string& nickname);
        void setUsername(const std::string& username);
        void setHostname(const std::string& hostname);
        const std::string& getNickname() const;
        const std::string& getUsername() const;
        std::string getHostname() const;
        void sendMessage(const std::string &message);
        void addChannel(Channel* ch);
        void removeChannel(Channel* channel);
        void Send();
        int getFd() const;
        bool isInvited(Client* client, Channel* channel);
        
		void setPasswordIsCorrect(void);
		int getPasswordIsCorrect(void);
        // void Recv();

		std::string getName(void);
};

#endif
