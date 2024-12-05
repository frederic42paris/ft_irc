#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"
#include "Commands.hpp"
#include "Macros.hpp"
#include "Server.hpp"

class Client;
class Server;

class Channel{
    private:
        Server& server;
        std::string name;
        std::vector<Client*> clients;
        std::vector<Client*> invitedClients;
        std::vector<Client*> operators;
        std::string Ch_pwd;
        bool inviteOnly;
        bool topicRestricted;
        int userLimits; // -1 for none
        std::string topic;
        std::map<std::string, std::string> modes;
    public:
        // Channel(){};
        ~Channel(){};
        // Channel(const Channel &src);
        // Channel &operator=(const Channel &src);
        Channel(const std::string& n, Server& srv);
        const std::string& getName() const;
        void addClient(Client* client);
        void removeClient(Client* client);
        void broadcastMessage(std::string message);
        void broadcastMessage(const std::string &message, Client *excludeClient);
        void changeCh_pwd(std::string newCh_pwd);
        //client join&leave channel(s)
        void joinChannel(Client* client, const std::string& password);
        void partChannel(Client* client, const std::string& message = "");
        //channel operators commands:
        bool isOperator(Client* client) const;
        void addOperator(Client* client);
        void removeOperator(Client* client);
        void kickClient(Client* client);
        void inviteClient(Client* client);
        void addInvitedClient(Client* client);
        void removeInvitedClient(Client* client);
        bool isClientInChannel(Client* client) const;
        bool isEmpty() const;
        const std::vector<Client*>& getInvitedClients() const;
        void setTopic(const std::string& newTopic);
        std::string getTopic() const;
        void setMode(const std::string& modeStr, const std::string& value, Client* client);
        std::string getMode(const std::string& mode) const;


		std::vector<Client*> getClients(void);
};


#endif
