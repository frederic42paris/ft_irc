#ifndef SERVER_HPP
#define SERVER_HPP
#include "Macros.hpp"
#include "Client.hpp"

class Channel;
class Client;

class Server{
    public:
        // Server(){};
        Server(int, std::string);
        ~Server(){};

        // INITS:
        void InitServer();
        void InitSockets();

        // FD:
        void CloseServer();
        void CloseClients(int);
        static void SigHandler(int);

        // CLIENT:
        void AcceptClient();
        void ReceiveData(int, int);
	    
        // GTRS/STRS
        std::vector<Channel > getChannels();
        std::vector<Client> getClients();
		Client *getClient(int);
        std::vector<struct pollfd> getfds(void);

        // METHODS:
        void 	CloseMessage(std::string);
        void	bufferParsing(int i, std::string buffer);
		void	cmdCap(int i);
	    void	cmdNick(int i, std::vector<std::string> string_array);
	    void	cmdUser(int i, std::vector<std::string> string_array);
	    void	cmdPass(int i, std::vector<std::string> string_array);
	    void	cmdQuit(int i);
	    void	cmdPrivmsg(int, std::vector<std::string>, std::string);
	    void	cmdJoin(int i, std::vector<std::string> string_array);
	    void	cmdPart(int i, std::vector<std::string> string_array);
	    void	cmdKick(int i, std::vector<std::string> string_array);
	    void	cmdInvite(int i, std::vector<std::string> string_array);
	    void	cmdTopic(int i, std::vector<std::string> string_array);
	    void	cmdMode(int i, std::vector<std::string> string_array);
        void    cmdPing(int i, std::vector<std::string> string_array);
        void    cmdPong(int i, std::string);
		int		isRegistered(int i);
        void	lineParsing(std::string line, int i);
		void	checkRegistration(int i);
		int		nicknameUsed(std::string nickname);
		int		checkStringFormat(std::string str);
		void 	channelMsg(int i, std::vector<std::string> string_array, std::string buffer);
		void 	userMsg(int i, std::vector<std::string> string_array, std::string buffer);
        //YUMI
        bool isValidChannelName(const std::string& channelName);
        // Channel findChannelByName(const std::string& channelName);
        // Client findClientByNickname(const std::string& nickname);
        void setMode(const std::string& mode, const std::string& value);
		// std::vector<Channel> getChannels(void);

    private:
        // VARIABLES:
        int _port;
        std::string _pwd;
        int _fd;
        std::string _lastPing;
        static int _sig;

        // VECTORS:
        std::vector<struct pollfd> _fds;
        std::vector<Client> _clients;
        std::vector<Channel> _channels;
		// std::vector<Channel *> _channels;
};

#endif /*SERVER_HPP*/
