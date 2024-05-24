#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include <stdio.h>

int main()
{
    printf("hell0 server\n");

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket == -1)
    {
        printf("cannot create socket: %d %s\n", errno, strerror(errno));
        return 1;
    }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(8081);
    address.sin_addr.s_addr = INADDR_ANY;

    int bind_result = bind(server_socket, (struct sockaddr *) &address, sizeof(address));

    if (bind_result == -1)
    {
        printf("cannot bind socket: %d %s\n", errno, strerror(errno));
        close(server_socket);
        return 1;
    }

    int listen_result = listen(server_socket, 0);

    if (listen_result == -1)
    {
        printf("cannot listen to socket: %d %s\n", errno, strerror(errno));
        close(server_socket);
        return 1;
    }

    int client_socket = accept(server_socket, NULL, NULL);

    if (client_socket == -1)
    {
        printf("cannot accept: %d %s\n", errno, strerror(errno));
        close(server_socket);
        return 1;
    }

    printf("accepted new connection\n");

    const char *message = "hell0 world!";
    send(client_socket, message, strlen(message), 0);

    close(client_socket);
    close(server_socket);
}
