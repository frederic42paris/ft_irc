#include "../includes/Irc.hpp"

void	check_return_zero(std::string string, int return_value)
{
	if (return_value != 0)
        std::cout << "error : " << string << std::endl;
	else
		std::cout << "success : " << string <<  std::endl;
}

void	check_return_fd(std::string string, int return_value)
{
	if (return_value == -1)
		std::cout << "error : " << string << "setup failed : " << return_value << std::endl;
	else
		std::cout << "success : " << string << "setup fd is : " << return_value << std::endl;
}

int main(void){
	
	struct addrinfo hints;
	struct addrinfo *res;

	memset(&hints, 0, sizeof hints);

	hints.ai_family = AF_UNSPEC;        // IPv4 ou IPv6, indifférent
    hints.ai_socktype = SOCK_STREAM;    // Connexion TCP
    hints.ai_flags = AI_PASSIVE;		// Remplit l'IP automatiquement

	check_return_zero("getaddrinfo", getaddrinfo(NULL, "6667", &hints, &res));

	// 1. Create the socket
	int socket_fd;
	socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	check_return_fd("server socket", socket_fd);

	// 2. Bind the socket to hte port
	check_return_zero("bind", bind(socket_fd, res->ai_addr, res->ai_addrlen));

	// 3. Listen
	check_return_zero("listen", listen(socket_fd, 5));

	// 4. Accept
	struct sockaddr_storage client_addr;
    socklen_t addr_size;
	addr_size = sizeof client_addr;
	check_return_fd("client socket",  accept(socket_fd, (struct sockaddr *)&client_addr, &addr_size));

	return 0;
}
