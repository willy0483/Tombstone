#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netdb.h>
#include <netinet/in.h>

#define PORT "9002"
#define BACKLOG 5

int main()
{
	int yes = 1;

	int socketfd;
	struct addrinfo hints;
	struct addrinfo* servinfo;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	int addr_info_state = getaddrinfo(NULL, PORT, &hints, &servinfo);
	if(addr_info_state != 0)
	{
		printf("getaddrinfo: %s\n", gai_strerror(addr_info_state));
		return 1;
	}

	socketfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
	if(socketfd == -1)
	{
		printf("Failed to create socket: %s\n", gai_strerror(socketfd));
		return 1;
	}

	if(setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
	{
		printf("Failed to setsockopt\n");
		return 1;
	}

	if(bind(socketfd, servinfo->ai_addr, servinfo->ai_addrlen) != 0)
	{
		printf("Failed to bind socket\n");
		return 1;
	}

	if(listen(socketfd, BACKLOG) != 0)
	{
		printf("Failed to listen to socket\n");
		return 1;
	}

	int client_socket = accept(socketfd, NULL, NULL);
	if(client_socket == -1)
	{
		printf("Failed to accept connecten from client\n");
		return 1;
	}

	send(client_socket, "You have reached the server!", 29, 0);

	close(client_socket);
	close(socketfd);

	freeaddrinfo(servinfo);
	return 0;
}
