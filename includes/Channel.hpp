#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"
#include "Macros.hpp"
#include "Server.hpp"

class Client;
class Server;

class Channel{
    private:
        std::string name;
        std::vector<Client> clients;
        std::vector<Client> invitedClients;
        std::vector<Client> operators;
        std::string Ch_pwd;
        bool inviteOnly;
        bool topicRestricted;
        int userLimits; // -1 for none
        std::string topic;
        std::map<std::string, std::string> modes;
    public:
        Channel(){};
        ~Channel(){};
		Channel(const std::string& n);
        bool operator==(const Channel& other) const
        {
            return this->name == other.name;
        }
        bool operator!=(const Channel& other) const
        {
            return !(*this == other);
        }
        const std::string& getName() const;
        void addClient(const Client &client);
        void removeClient(const Client &client);
        void broadcastMessage(const std::string& message);
        void broadcastMessage(const std::string &message, const Client &excludeClient);
        void changeCh_pwd(std::string newCh_pwd);
        //client join&leave channel(s)
        void joinChannel(Client &client, const std::string& password);
        void partChannel(Client &client, const std::string& message = "");
        //channel operators commands:
        bool isOperator(const Client &client) const;
        void addOperator(const Client &client);
        void removeOperator(const Client &client);
        void kickClient(Client &client);
        void inviteClient(Client &client, Client &targetclient);
        void addInvitedClient(const Client &client);
        void removeInvitedClient(const Client &client);
        bool isClientInChannel(const Client &client) const;
        bool isEmpty() const;
        const std::vector<Client>& getInvitedClients() const;
        void setTopic(const std::string& newTopic);
        std::string getTopic() const;
        void setMode(const std::string& modeStr, const std::string& value, const Client &client);
        std::string getMode(const std::string& mode) const;
        bool getTopicRestricted() const;


		std::vector<Client> getClients(void);
};


#endif
