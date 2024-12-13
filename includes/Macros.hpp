#ifndef MACROS_HPP
#define MACROS_HPP
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cstring>
#include <csignal>
#include <cstdio>
#include <poll.h> // poll
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h> // socket
#include <sys/types.h> // socket
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <map>
#include <algorithm>
#include <string>

#include <poll.h>
#include <signal.h>
#include <vector>
#include <algorithm>
#include <map>

// Recommended useful macros
#define USER_ID(nickname, username) (":" + nickname + "!" + username + "@localhost")
#define RPL_INVITE(user_id, invited, channel) (user_id + " INVITE " + invited + " #" + channel + "\r\n")
#define RPL_JOIN(user_id, channel) (user_id + " JOIN #" + channel + "\r\n")
#define ERR_INVALIDMODEPARAM(client, channel, mode, password) ("696 " + client + " #" + channel + " " + mode + " " + password + " : password must only contained alphabetic character\r\n")

// Parsing / debugging / testing macros
#define HERE(event) std::cout<<YELLOW<<event<<GREEN<<" success"<<RESET<<std::endl;
#define NEWLINE std::cout<<std::endl;

//Base colors
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define PURPLE "\033[0;35m"

//Bold colors
#define BRED "\033[1;31m"
#define BGREEN "\033[1;32m"
#define BYELLOW "\033[1;33m"
#define BBLUE "\033[1;34m"
#define BPURPLE "\033[1;35m"

#define RESET "\033[0m"

class ErrThrow : public std::exception{
	private:
		std::string errMsg;
	public:
		ErrThrow(const char* msg) : errMsg(msg){}
		~ErrThrow() _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW{};
		const char* what() const throw(){return errMsg.c_str();};
};

#endif /*IRC_HPP*/
