#include <stdio.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

int main()
{
	char server_message[256] = "You have reached the server!";

	// create server socket
	int socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);

	// define the server address
	struct sockaddr_in addr = {
		.sin_family = AF_INET,
		.sin_port = htons(9002),
		.sin_addr = { .s_addr = INADDR_ANY },
	};

	// bind the socket to our specified IP and port
	int bind_state = bind(socketfd, (struct sockaddr*)&addr, sizeof(addr));
	if(bind_state == -1)
	{
		printf("Failed to bind the socket\n");
		return 1;
	}

	int listen_state = listen(socketfd, 5);
	if(listen_state == -1)
	{
		printf("Failed to listen to socket\n");
	}

	int client_socket = accept(socketfd, NULL, NULL);
	if(client_socket == -1)
	{
		printf("Failed to accept socket\n");
		return 1;
	}

	send(client_socket, server_message, sizeof(server_message), 0);

	close(socketfd);
	return 0;
}
