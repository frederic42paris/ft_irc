#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Channel.hpp"
#include "Server.hpp"
#include "Macros.hpp"

class Channel;

class Client{
    private:
        std::string nick;
		std::string oldnick;
        std::string usr;
        std::string host;
        std::string buffer; // take the message until \r\n
        std::vector<Channel> channels;
        int client_fd;
		int _passwordIsCorrect;
        int _isRegistered;
    public:
        Client(){};
        ~Client(){};
        bool operator==(const Client& other) const
        {
            return this->client_fd == other.client_fd;
        }
        bool operator!=(const Client& other) const
        {
            return !(*this == other);
        }

        Client(int fd);
        void setFd(int fd);
        void setNickname(const std::string& nickname);
        void setUsername(const std::string& username);
        void setIsLogged(void);
        int getIsLogged(void);
		void updateBuffer(const std::string);
		void clearBuffer();
		void setOldNick(const std::string& username);
        void setIsRegistered(void);
        int getIsRegistered(void);
        const std::string& getNickname() const;
        const std::string& getUsername() const;
		const std::string& getOldNick() const;
        std::string getHostname() const;
        void sendMessage(const std::string &message) const;
        void addChannel(const Channel &ch);
        void removeChannel(const Channel &channel);
        void Send();
        int getFd() const;
        bool isInvited(const Client &client, const Channel &channel) const;
        
		void setPasswordIsCorrect(void);
		int getPasswordIsCorrect(void);
		void setBuffer(std::string);
		std::string &	getBuffer(void);

		std::string getName(void) const;
};

#endif
