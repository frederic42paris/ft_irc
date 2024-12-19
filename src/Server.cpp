#include "../includes/Server.hpp"

int Server::_sig = 0;

Server::Server(int port, std::string pwd):_port(port),_pwd(pwd),_fd(-1),_lastPing(""){}

void Server::SigHandler(int signum){
  (void)signum;
  _sig = 1;
  std::cout << ": " RED "QUIT input received\n";
}

void Server::InitServer(){
  InitSockets();
  if (_fd > 0)
    std::cout<<PURPLE<<"Server["<<_fd<<"]"<<GREEN<<" connected successfully"<<RESET<<std::endl;
  else
    return;
  while (_sig == 0){
    if ((poll(&_fds[0], _fds.size(), -1) == -1) && _sig == 0)
      throw ErrThrow("Poll error");
    for (size_t i = 0; i < _fds.size(); i++){
      if (_sig == 0 && (_fds[i].revents & POLLIN)){
        if (_fds[i].fd == _fd)
          AcceptClient();
        else
          ReceiveData(_fds[i].fd, i); 
      }
    }
  }
  CloseServer();
}

void Server::InitSockets(){
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(_port);
  _fd = socket(AF_INET, SOCK_STREAM, 0);
  if (_fd < 0){
    perror("Socket error");
    return;}
    
  // SOCKET OPTIONS AND BIND
  int enable = 1;
  if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) < 0) // speeds up restart times
    throw ErrThrow("SetSockOpt error");
  if (bind(_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) // bind to address and port in addr
    throw ErrThrow("Bind error");
  if (fcntl(_fd, F_SETFL, O_NONBLOCK) < 0) // fd operations are reaaed instantly
    throw ErrThrow("Fcntl error");
  if (listen(_fd, SOMAXCONN) < 0)
    throw ErrThrow("Listen error");

  // ADDING TO THE POLL VECTOR
  struct pollfd pollNew;
  pollNew.fd = _fd;
  pollNew.events = POLLIN;
  pollNew.revents = 0;
  _fds.push_back(pollNew); // add to the end of the vector
    
}

void Server::AcceptClient(){
  // CREATING A CLIENT SOCKET (ACCEPT)
  struct sockaddr_in addr;
  struct pollfd pollNew;
  socklen_t len = sizeof(addr);
  int lisFd = accept(_fd, (sockaddr *)&addr, &len);
  if (lisFd < 0)
    perror("listen");
  Client client(lisFd);
  std::cout<<PURPLE<<USER_ID(client.getNickname(), client.getUsername())<<GREEN<<" connected successfully to "<<PURPLE<<"Server["<<_fd<<"]" RESET<<std::endl;  

  // ADDING TO THE POLL VECTOR
  pollNew.fd = lisFd;
  pollNew.events = POLLIN;
  pollNew.revents = 0;
  _fds.push_back(pollNew);
  client.setFd(lisFd);
  _clients.push_back(client);
}

void Server::ReceiveData(int fd, int i){
  char buf[1024] = {0};
  ssize_t recData = recv(fd, buf, sizeof(buf) - 1, 0);
  if (recData <= 0){
    CloseClients(fd);
    close(fd);
  }
  else{

    std::cout << PURPLE << "Client["<<fd<<"]: "<< RESET << buf; 
    bufferParsing(i, buf);
  }
}

void Server::CloseServer(){
  for (size_t i = 0; i < _clients.size(); i++){
    std::cout<<PURPLE<<"Client["<<_clients[i].getFd()<<"]"<<RED<<" has disconnected"<<RESET<<std::endl;
    close(_clients[i].getFd());
  }
  if (_fd > 0){
    std::cout<<PURPLE<<"Server["<<_fd<<"]"<<RED<<" has disconnected"<<RESET<<std::endl;
    close(_fd);
  }
}

void Server::CloseClients(int fd){
  for (size_t i = 0; i < _fds.size(); i++){
    if (_fds[i].fd == fd){
      _fds.erase(_fds.begin() + i); 
      break;}}
  for (size_t i = 0; i < _clients.size(); i++){
    if (_clients[i].getFd() == fd){
      _clients.erase(_clients.begin() + i);
       break;}}
}

void Server::CloseMessage(std::string errMsg){
  if (_fd > 0)
    close(_fd);
  _fd = -1;
  std::cout << RED << "ERROR\n" << YELLOW << errMsg << RESET;
  return;
}

std::vector<Channel > Server::getChannels(void)
{
	return _channels;
}

std::vector<Client> Server::getClients(void)
{
	return this->_clients;
}

Client *Server::getClient(int fd){
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
		if (it->getFd() == fd) {
			return &(*it);
		}
	}	
	std::cout << "Client doesn't exist" << std::endl;
	return &_clients[0];
}
